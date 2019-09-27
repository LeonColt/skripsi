#include "stdafx.h"
#include "DirectoryLocal.h"
#include "FileLocal.h"


DirectoryLocal::DirectoryLocal() {
}

std::unique_ptr<tstring> DirectoryLocal::getOwner() {
	return std::make_unique<tstring>( L"Leon Colt" );
	maschinen::file::FileHandler fh( getPath()->c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS );
	return fh.getOwner();
}

ULONGLONG DirectoryLocal::getSize() const {
	ULONGLONG size = 0;
	return size;
}

bool DirectoryLocal::hasChildren() { 
	return PathIsDirectoryEmpty( getPath()->c_str() ) == TRUE;
}

LocalIterator DirectoryLocal::begin(){
	tstring path( getPath()->c_str() );
	path.append( TEXT( "\\*" ) );
	dh.setPath( path.c_str() );
	iterator iter;
	iter.dh = &dh;
	for( dri = dh.begin(); dri != dh.end(); dri++ ) {
		if ( dri.file_name.compare( TEXT( "." ) ) == 0
			 || dri.file_name.compare( TEXT( ".." ) ) == 0
			 || dri.file_name.compare( TEXT( "$RECYCLE.BIN" ) ) == 0
			 || dri.file_name.compare( TEXT( "$Recycle.Bin" ) ) == 0
			 || dri.file_name.compare( TEXT( "bootmgr" ) ) == 0
			 || dri.file_name.compare( TEXT( "BOOTNXT" ) ) == 0
			 || dri.file_name.compare( TEXT( "hiberfil.sys" ) ) == 0
			 || dri.file_name.compare( TEXT( "pagefile.sys" ) ) == 0
			 || dri.file_name.compare( TEXT( "Recovery" ) ) == 0
			 || dri.file_name.compare( TEXT( "swapfile.sys" ) ) == 0
			 || dri.file_name.compare( TEXT( "$SysReset" ) ) == 0
			 ) continue;
		if( dri.file_attributes & FILE_ATTRIBUTE_DIRECTORY ) {
			DirectoryLocal* dir = new DirectoryLocal();
			dir->setName( dri.file_name );
			tstring fpath = getPath()->c_str();
			fpath.append( dri.file_name );
			fpath.append( TEXT( "\\" ) );
			dir->setPath( fpath.c_str() );
			iter = dir;
			iter.dri = &dri;
			iter.parent = this;
			return iter;
		}
		else {
			FileLocal* fc = new FileLocal();
			fc->setName( dri.file_name );
			tstring fpath = getPath()->c_str();
			fpath.append( dri.file_name );
			fc->setPath( fpath.c_str() );
			iter = fc;
			iter.dri = &dri;
			iter.parent = this;
			return iter;
		}
	}
	iter = nullptr;
	return iter;
}
LocalIterator DirectoryLocal::end(){
	return iterator( nullptr );
}

void DirectoryLocal::createNewDirectory() const {
	tstring name( TEXT( "New Folder" ) );
	tstring path;
	int indexer = 0;
	do {
		path.assign( getPath()->c_str() );
		path.append( TEXT( "\\" ) );
		if ( indexer > 0 ) {
			name.assign( TEXT( "New Folder" ) );
			name.append( TEXT( "(" ) );
			name.append( to_tstring( indexer ).c_str() );
			name.append( TEXT( ")" ) );
		}
		path.append( name.c_str() );
		indexer++;
	} while ( CreateDirectoryW( path.c_str(), NULL ) == 0 );
}

void DirectoryLocal::deleteMember( Local* file ) const {
	maschinen::com::ComponentObjectModel com( COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE );
	maschinen::com::FileOperation fo( &com );
	fo.deleteFile( file->getPath()->c_str() );
	fo.commit();
}

void DirectoryLocal::deleteMember( std::vector<Local*>& files ) const {
	std::vector<tstring> paths;
	for ( auto i = files.cbegin(); i != files.cend(); ++i ) {
		paths.push_back( ( *i )->getPath()->c_str() );
	}
	maschinen::com::ComponentObjectModel com( COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE );
	maschinen::com::FileOperation fo( &com );
	fo.deleteFile( &paths );
	fo.commit();
}

void DirectoryLocal::copy( Local* source ) const {
	maschinen::com::ComponentObjectModel com( COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE );
	maschinen::com::FileOperation fo( &com );
	fo.copy( source->getPath()->c_str(), getPath()->c_str() );
	fo.commit();
}

void DirectoryLocal::copy( std::vector<Local*>& sources ) const {
	std::vector<tstring> paths;
	for ( auto i = sources.cbegin(); i != sources.cend(); ++i ) {
		paths.push_back( ( *i )->getPath()->c_str() );
	}
	maschinen::com::ComponentObjectModel com( COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE );
	maschinen::com::FileOperation fo( &com );
	fo.copy( &paths, getPath()->c_str() );
	fo.commit();
}

void DirectoryLocal::move( Local* source ) const {
	maschinen::com::ComponentObjectModel com( COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE );
	maschinen::com::FileOperation fo( &com );
	fo.move( source->getPath()->c_str(), getPath()->c_str() );
	fo.commit();
}
void DirectoryLocal::move( std::vector<Local*>& sources ) const {
	std::vector<tstring> paths;
	for ( auto i = sources.cbegin(); i != sources.cend(); ++i ) {
		paths.push_back( ( *i )->getPath()->c_str() );
	}
	maschinen::com::ComponentObjectModel com( COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE );
	maschinen::com::FileOperation fo( &com );
	fo.move( &paths, getPath()->c_str() );
	fo.commit();
}

DirectoryLocal::~DirectoryLocal() {
}

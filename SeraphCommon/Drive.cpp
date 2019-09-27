#include "stdafx.h"
#include "Drive.h"


Drive::Drive() {}

std::unique_ptr<tstring> Drive::getVolumeName() {
	std::unique_ptr<tstring> res = std::make_unique<tstring>();
	TCHAR buffer[ MAX_PATH + 1];
	tstring path = name.c_str();
	path.append( L"\\" );
	if ( GetVolumeInformation( path.c_str(), buffer, MAX_PATH + 1, NULL, NULL, NULL, NULL, NULL ) == 0 ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING ) ) {
			MessageBox( NULL, buffer, L"ERROR", MB_ICONERROR | MB_OK );
		}
	}
	res->assign( buffer );
	return res;
}

DWORD Drive::getVolumeSerialNumber() {
	DWORD vsn;
	tstring path = name.c_str();
	path.append( L"\\" );
	GetVolumeInformation( path.c_str(), NULL, NULL, &vsn, NULL, NULL, NULL, NULL );
	return vsn;
}

std::unique_ptr<tstring> Drive::getOwner() {
	TCHAR buffer[ MAX_LOADSTRING ];
	DWORD size = MAX_LOADSTRING;
	GetUserNameW( buffer, &size );
	return std::make_unique<tstring>( buffer );
}

std::unique_ptr<tstring> Drive::getFileSystemName() {
	std::unique_ptr<tstring> res = std::make_unique<tstring>();
	TCHAR buffer[ MAX_PATH + 1 ];
	tstring path = name.c_str();
	path.append( L"\\" );
	if ( GetVolumeInformation( path.c_str(), NULL, NULL, NULL, NULL, NULL, buffer, MAX_PATH + 1 ) == 0 ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) ) {
			if ( error_code == 21 ) {
				res->assign( L"CD ROM" );
				return res;
			}
			MessageBox( NULL, error, L"ERROR", MB_ICONERROR | MB_OK );
		}
	}
	res->assign( buffer );
	return res;
}

ULONGLONG Drive::getSize() const noexcept {
	ULARGE_INTEGER size;
	tstring path = name.c_str();
	path.append( L"\\" );
	GetDiskFreeSpaceEx( path.c_str(), NULL, &size, NULL );
	return size.QuadPart;
}

bool Drive::isDeletable() const noexcept {return false;}
bool Drive::isOpenable() const noexcept {return true;}

void Drive::createNewDirectory() const {
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

bool Drive::hasChildren() {return PathIsDirectoryEmpty( getPath()->c_str() ) == TRUE;}

LocalIterator Drive::begin() {
	tstring path( getPath()->c_str() );
	path.append( TEXT( "\\*" ) );
	dh.setPath( path.c_str() );
	iterator iter;
	iter.dh = &dh;
	for ( dri = dh.begin(); dri != dh.end(); dri++ ) {
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
		if ( dri.file_attributes & FILE_ATTRIBUTE_DIRECTORY ) {
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
LocalIterator Drive::end() {
	return iterator( nullptr );
}

Drive::~Drive() {}

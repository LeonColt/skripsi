#include "stdafx.h"
#include "FileExecutor.h"


FileExecutor::FileExecutor( FileControl* const fc, LPCTSTR error_marker, Akey* akey, Setting* setting, UserCredential* uc )  :
	ERROR_MARKER(error_marker),
	OPERATION_NULL(0),
	OPERATION_COPY(1),
	OPERATION_CUT(2),
	OPERATION_DELETE(3),
	OPERATION_RESTORE(4),
	LOCAL( 1 ),
	CLOUD( 2 ),
	CLOUD_BIN( 3 ) {
	this->akey = akey;
	this->setting = setting;
	this->uc = uc;
	this->fc = fc;
}

FileExecutor::~FileExecutor() {
}

void FileExecutor::copy() const {
	std::vector<tstring> cloud_sources;
	std::vector<tstring> local_sources;
	for ( auto i = sources.begin(); i != sources.end(); i++ ) {
		INT type = determineType( i->c_str() );
		if ( type == CLOUD )
			cloud_sources.push_back( i->c_str() );
		else if ( type == LOCAL )
			local_sources.push_back( i->c_str() );
	}
	INT type_target = determineType( target.c_str() );
	if( type_target == LOCAL ) {
		if( local_sources.size() > 0 ) {
			maschinen::com::ComponentObjectModel com( COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE );
			maschinen::com::FileOperation fo( &com );
			fo.copy( &local_sources, target.c_str() );
			fo.commit();
		}
		if( cloud_sources.size() > 0 ) {
			DownloadRequest* req = new DownloadRequest();
			req->setAutoDelete();
			req->setParent( fc );
			req->setAkey( akey );
			req->setUserCredential( uc );
			req->setSetting( setting );
			req->setErrorMarker( ERROR_MARKER.c_str() );
			req->setTarget( target.c_str() );
			for ( auto i = cloud_sources.begin(); i != cloud_sources.end(); ++i )
				req->addSources( *i );
			req->execute();
		}
	}
	else if( type_target == CLOUD ) {
		if ( local_sources.size() > 0 ) {
			UploadFileRequestAT* req = new UploadFileRequestAT();
			req->setAutoDelete();
			req->setParent( fc );
			req->setAkey( akey );
			req->setSetting( setting );
			req->setUserCredential( uc );
			req->setErrorMarker( ERROR_MARKER.c_str() );
			req->setTarget( target.c_str() );
			for ( auto i = local_sources.cbegin(); i != local_sources.cend(); ++i )
				req->addSources( i->c_str() );
			req->execute();
		}
		if ( cloud_sources.size() > 0 ) {
			CopyFileCloudRequest* req = new CopyFileCloudRequest();
			req->setAutoDelete();
			req->setParent( fc );
			req->setAkey( akey );
			req->setSetting( setting );
			req->setUserCredential( uc );
			req->setErrorMarker( ERROR_MARKER.c_str() );
			req->setTarget( target.c_str() );
			for ( auto i = cloud_sources.cbegin(); i != cloud_sources.cend(); ++i )
				req->addSources( i->c_str() );
			req->execute();
		}
	}
}

void FileExecutor::cut() const {
	std::vector<tstring> cloud_sources;
	std::vector<tstring> local_sources;
	for ( auto i = sources.begin(); i != sources.end(); i++ ) {
		INT type = determineType( i->c_str() );
		if ( type == CLOUD )
			cloud_sources.push_back( i->c_str() );
		else if ( type == LOCAL )
			local_sources.push_back( i->c_str() );
	}
	INT type_target = determineType( target.c_str() );
	if( type_target == LOCAL ) {
		if( local_sources.size() > 0 ) {
			maschinen::com::ComponentObjectModel com( COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE );
			maschinen::com::FileOperation fo( &com );
			fo.move( &local_sources, target.c_str() );
			fo.commit();
		}
		if ( cloud_sources.size() > 0 ) {
			DownloadRequest* req = new DownloadRequest();
			req->setAutoDelete();
			req->setParent( fc );
			req->setAkey( akey );
			req->setUserCredential( uc );
			req->setSetting( setting );
			req->setErrorMarker( ERROR_MARKER.c_str() );
			req->setTarget( target.c_str() );
			for ( auto i = cloud_sources.begin(); i != cloud_sources.end(); ++i )
				req->addSources( *i );
			req->execute();
		}
	}
	else if( type_target == CLOUD ) {
		if ( local_sources.size() > 0 ) {
			UploadFileRequestAT* req = new UploadFileRequestAT();
			req->setAutoDelete();
			req->setParent( fc );
			req->setAkey( akey );
			req->setSetting( setting );
			req->setUserCredential( uc );
			req->setErrorMarker( ERROR_MARKER.c_str() );
			req->setTarget( target.c_str() );
			for ( auto i = local_sources.cbegin(); i != local_sources.cend(); ++i )
				req->addSources( i->c_str() );
			req->execute();
		}
		if ( cloud_sources.size() > 0 ) {
			CutFileCloudRequest* req = new CutFileCloudRequest();
			req->setAutoDelete();
			req->setParent( fc );
			req->setAkey( akey );
			req->setSetting( setting );
			req->setUserCredential( uc );
			req->setErrorMarker( ERROR_MARKER.c_str() );
			req->setTarget( target.c_str() );
			for ( auto i = cloud_sources.cbegin(); i != cloud_sources.cend(); ++i )
				req->addSources( i->c_str() );
			req->execute();
		}
	}
}

void FileExecutor::deleteFile() const {
	std::vector<tstring> clouds;
	std::vector<tstring> locals;
	std::vector<tstring> cloud_bin;

	for ( auto i = sources.begin(); i != sources.end(); i++ ) {
		INT type = determineType( i->c_str() );
		if ( type == CLOUD )
			clouds.push_back( i->c_str() );
		else if ( type == LOCAL )
			locals.push_back( i->c_str() );
		else if ( type == CLOUD_BIN )
			cloud_bin.push_back( i->c_str() );
	}

	if( locals.size() > 0 ) {
		maschinen::com::ComponentObjectModel com( COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE );
		maschinen::com::FileOperation fo( &com );
		fo.deleteFile( &locals );
		fo.commit();
	}
	if( clouds.size() > 0 ) {
		CloudDeleteFile* deleter = new CloudDeleteFile();
		deleter->setAutoDelete();
		deleter->setErrorMarker( ERROR_MARKER.c_str() );
		deleter->setAkey( akey );
		deleter->setSetting( setting );
		deleter->setUserCredential( uc );
		deleter->setParent( fc );
		deleter->setSources( clouds );
		deleter->execute();
	}
	if( cloud_bin.size() > 0 ) {
		DeleteCloudRecycleBinObject* deleter = new DeleteCloudRecycleBinObject();
		deleter->setAutoDelete();
		deleter->setErrorMarker( ERROR_MARKER.c_str() );
		deleter->setAkey( akey );
		deleter->setSetting( setting );
		deleter->setUserCredential( uc );
		deleter->setParent( fc );
		deleter->setSources( cloud_bin );
		deleter->execute();
	}
}

void FileExecutor::restoreFile() const {
	std::vector<tstring> cloud_bin;
	for ( auto i = sources.begin(); i != sources.end(); i++ ) {
		INT type = determineType( i->c_str() );
		if ( type == CLOUD_BIN )
			cloud_bin.push_back( i->c_str() );
	}
	if( cloud_bin.size() > 0 ) {
		CloudRestoreFile* cfr = new CloudRestoreFile();
		cfr->setAutoDelete();
		cfr->setErrorMarker( ERROR_MARKER.c_str() );
		cfr->setAkey( akey );
		cfr->setSetting( setting );
		cfr->setUserCredential( uc );
		cfr->setParent( fc );
		cfr->setSources( cloud_bin );
		cfr->execute();
	}
}

void FileExecutor::setTarget( LPCTSTR target ) noexcept {
	this->target.assign( target );
}
void FileExecutor::addSource( LPCTSTR source ) noexcept
{
	sources.push_back( source );
}
void FileExecutor::cleanSource() noexcept { sources.clear(); }

void FileExecutor::execute() {
	if ( operation == OPERATION_COPY ) copy();
	else if ( operation == OPERATION_CUT ) cut();
	else if ( operation == OPERATION_DELETE ) deleteFile();
	else if ( operation == OPERATION_RESTORE ) restoreFile();
}

void FileExecutor::operationCopy() {
	operation = OPERATION_COPY;
}

void FileExecutor::operationCut() {
	operation = OPERATION_CUT;
}

void FileExecutor::operationDelete() {
	operation = OPERATION_DELETE;
}
void FileExecutor::operationRestore() {
	operation = OPERATION_RESTORE;
}

INT FileExecutor::determineType( LPCTSTR path ) const noexcept {
	std::vector<tstring> paths;
	size_t size = tstrSplit( path, &paths, TEXT( "\\" ) );
	if ( paths.size() > 0 ) {
		if ( paths.begin()->compare( setting->getCloudRootName()->c_str() ) == 0 )
			return CLOUD;
		else if ( paths.begin()->compare( setting->getCloudRecycleBinRootName()->c_str() ) == 0 )
			return CLOUD_BIN;
		else return LOCAL;
	}
	return NULL;
}
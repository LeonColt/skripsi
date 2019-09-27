#include "stdafx.h"
#include "MetaDataUpload.h"


MetaDataUpload::MetaDataUpload() {
}

void MetaDataUpload::startUpload( const DWORD buffer_size ) {
	uploader.reset();
	uploader = std::make_unique<Uploader>( buffer_size );
	uploader->setAkey( akey );
	uploader->setSetting( setting );
	uploader->setUserCredential( user_credential );
	uploader->setParent( ui );
	uploader->setMetaData( this );
	uploader->execute();
}

void MetaDataUpload::pause() {
	uploader->pause();
}

//execute only once when file upload is declaring
void MetaDataUpload::write( std::vector<MetaDataUploadDetail>& files_real_path ) {
	{//check if upload cache exists
		DWORD attr = GetFileAttributes( getCacheFileUploadDirectoryName()->c_str() );
		if ( attr == INVALID_FILE_ATTRIBUTES || !( attr & FILE_ATTRIBUTE_DIRECTORY ) )
			CreateDirectoryW( getCacheFileUploadDirectoryName()->c_str(), NULL );
	}
	files_real_path = datas;
	maschinen::file::FileHandler fh( getPath().get(), GENERIC_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL );
	if ( !fh.isValid() ) {
		TCHAR error[ MAX_LOADSTRING ];
		char error_mb[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) ) {
			size_t size_error = wcslen( error );
			wcstombs_s( &size_error, error_mb, error, MAX_LOADSTRING );
			throw std::exception( error_mb, ( int ) error_code );
		}
		else throw std::exception( "an unknown error occured" );
	}

	Poco::JSON::Object json;
	json.set( DISPOSITION, DISPOSITION_VALUE.c_str() );
	json.set( ID, wstrToStr( id.c_str() )->c_str() );

	Poco::JSON::Array arr;
	tstring cache_name;
	tstring cache_path;
	for ( auto i = datas.begin(); i != datas.end(); ++i ) {
		Poco::JSON::Object obj;
		obj.set( FILES_ID, i->getId() );
		obj.set( FILES_NAME, wstrToStr( i->getName()->c_str() )->c_str() );
		int indexer = 0;
		for ( ;; ) {
			cache_name.assign( id.c_str() );
			cache_name.append( TEXT( "-" ) );
			cache_name.append( to_tstring( i->getId() ) );
			if ( indexer > 0 ) {
				cache_name.append( TEXT( "-" ) );
				cache_name.append( to_tstring( indexer++ ).c_str() );
			}
			if ( GetFileAttributes( cache_name.c_str() ) == INVALID_FILE_ATTRIBUTES ) break;
		}
		cache_path.assign( getCacheFileUploadDirectoryName()->c_str() );
		cache_path.append( TEXT( "\\" ) );
		cache_path.append( cache_name.c_str() );
		i->setPath( cache_path );
		obj.set( FILES_PATH, wstrToStr( cache_name.c_str() )->c_str() );
		obj.set( FILES_AS_DIRECTORY, i->isDirectory() );
		arr.add( obj );
	}
	json.set( FILES, arr );
	std::string data_writting( wstrToStr( serialization( &json )->c_str() )->c_str() );
	DWORD number_of_written = 0;
	try {
		fh.write( (LPVOID)data_writting.c_str(), ( DWORD ) data_writting.length(), &number_of_written );
	} catch ( maschinen::exception::Exception& e ) {
		throw e;
	}
}

void MetaDataUpload::read(LPCTSTR meta_name) {
	if ( wcslen( meta_name ) < 1 ) throw SeraphException();
	meta_path.assign( getParentPath()->c_str() );
	meta_path.append( TEXT( "\\" ) );
	meta_path.append( meta_name );
	maschinen::file::FileHandler fh( meta_path, GENERIC_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL );
	if ( !fh.isValid() ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) ) 
			throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), error, error_code );
		else throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str() );
	}
	try {
		std::string temp = "";
		char buffer_mb[ MAX_LOADSTRING ];
		for ( auto i = fh.beginRead( buffer_mb, MAX_LOADSTRING - 1 ); i != fh.endRead(); i++ ) {
			buffer_mb[ i.number_of_byte_read ] = '\0';
			temp.append( buffer_mb );
		}

		Poco::JSON::Object::Ptr json = deserialization( temp );

		if ( !json->has( DISPOSITION ) || !json->has( FILES ) ) throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), L"Error, mata data is corrupt", 443 );
		if ( json->get( DISPOSITION ).extract<std::string>().compare( DISPOSITION_VALUE ) != 0 ) throw MetaDataDoesNotMatchException();
		if ( !json->has( ID ) ) throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), L"Error, mata data is corrupt", 443 );

		id.assign( strToWstr( json->get( ID ).extract<std::string>().c_str() )->c_str() );

		Poco::JSON::Array::Ptr arr = json->get( FILES ).extract<Poco::JSON::Array::Ptr>();
		datas.clear();

		for ( auto i = arr->begin(); i != arr->end(); ++i ) {
			Poco::JSON::Object::Ptr obj = i->extract<Poco::JSON::Object::Ptr>();
			MetaDataUploadDetail mdud;
			mdud.setId( obj->get(FILES_ID).convert<QWORD>() );
			mdud.setName( strToWstr( obj->get( FILES_NAME ).extract<std::string>() )->c_str() );
			tstring path( getCacheFileUploadDirectoryName()->c_str() );
			path.append( TEXT( "\\" ) );
			path.append( strToWstr( obj->get( FILES_PATH ).extract<std::string>() )->c_str() );
			mdud.setPath( path.c_str() );
			if ( obj->get( FILES_AS_DIRECTORY ).extract<bool>() ) mdud.asDirectory();
			datas.push_back( mdud );
		}

	} catch ( MetaDataDoesNotMatchException& e ) {
		throw e;
	} catch ( SeraphException& e ) {
		throw e;
	} catch ( maschinen::exception::Exception& e ) {
		throw e;
	} catch ( std::exception& e ) {
		throw e;
	}
}

std::unique_ptr<tstring> MetaDataUpload::getTargetName() {
	return std::make_unique<tstring>( L"" );
}

MetaDataUpload::~MetaDataUpload() {
}

void MetaDataUpload::setUI( FileUploadProcessWindow* ui ) noexcept {this->ui = ui;}

void MetaDataUpload::setFiles( std::vector<MetaDataUploadDetail>& datas ) {
	this->datas.swap( datas );
}


std::vector<MetaDataUploadDetail>::const_iterator MetaDataUpload::begin() const noexcept { return datas.cbegin(); }
std::vector<MetaDataUploadDetail>::const_iterator MetaDataUpload::end() const noexcept { return datas.cend(); }

std::unique_ptr<tstring> MetaDataUpload::getCacheFileUploadDirectoryName() const noexcept {
	std::unique_ptr<tstring> temp = std::make_unique<tstring>( getDefaultAppDataPath( true )->c_str() );
	temp->append( L"\\UploadCache" );
	return temp;
}

QWORD MetaDataUpload::getSize() const {
	QWORD size = 0;
	for ( auto i = datas.cbegin(); i != datas.cend(); ++i ) {
		size += i->getSize();
	}
	return size;
}

void MetaDataUpload::deleteCache() {
	for ( auto i = datas.begin(); i != datas.end(); ++i ) {
		tstring path( getCacheFileUploadDirectoryName()->c_str() );
		path.append( TEXT( "\\" ) );
		path.append( i->getPath()->c_str() );
#ifdef _UNICODE
		DeleteFileW( path.c_str() );
#else
		DeleteFileA( path.c_str() );
#endif // _UNICODE

	}
}
void MetaDataUpload::deleteMetaDataFile() {
	DeleteFile( meta_path.c_str() );
}
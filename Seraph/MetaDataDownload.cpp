#include "stdafx.h"
#include "MetaDataDownload.h"


MetaDataDownload::MetaDataDownload() {
	root = std::make_unique<MetaDataDownloadDirectory>();
	root->setId( 0 );
	root->setName( L"root" );
	root->setOriginal( true );
	root->setParent( nullptr );
	root->setSize( 0 );
}

void MetaDataDownload::startDownload( const DWORD buffer_size ) {
	downloader.reset();
	downloader = std::make_unique<Downloader>( buffer_size );
	downloader->setMetaData( this );
	downloader->setParent( ui );
	downloader->setAkey( akey );
	downloader->setSetting( setting );
	downloader->setUserCredential( user_credential );
	downloader->execute();
}

void MetaDataDownload::pause() {
	downloader->pause();
}

void MetaDataDownload::cancel() {
	downloader->cancel( true );
}

void MetaDataDownload::write() {
	Poco::JSON::Object json;
	json.set( DISPOSITION, DISPOSITION_VALUE );
	json.set( ID, wstrToStr( id.c_str() )->c_str() );
	json.set( VAR_TARGET_PATH, wstrToStr( target_path.c_str() )->c_str() );

	Poco::JSON::Array arr;
	for ( auto i = root->begin(); i != root->end(); i++ ) {
		if ( MetaDataDownloadDirectory* dir = dynamic_cast< MetaDataDownloadDirectory* >( *i ) )
			MetaDataToJsonArray( &arr, dir );
		else if ( MetaDataDownloadFile* file = dynamic_cast< MetaDataDownloadFile* >( *i ) ) {
			Poco::JSON::Object temp;
			temp.set( FILES_ID, std::to_string( file->getId()).c_str() );
			temp.set( FILES_NAME, wstrToStr( file->getName()->c_str() )->c_str() );
			temp.set( FILES_DIRECTORY, false );
			temp.set( FILES_SIZE, std::to_string( file->getSize() ).c_str() );
			temp.set( FILES_PARENT, "0" );
			temp.set( FILES_ORIGINAL, file->isOriginal() );
			arr.add( temp );
		}
	}
	json.set( FILES, arr );
	std::string seriali( wstrToStr( serialization( &json )->c_str() )->c_str() );
	maschinen::file::FileHandler fh( getPath()->c_str(), GENERIC_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL );
	if ( !fh.isValid() ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), error, error_code );
		else throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str() );
	}
	DWORD number_of_written = 0;
	try {
		fh.write( ( LPVOID ) seriali.c_str(), ( DWORD )seriali.length(), &number_of_written );
	} catch ( maschinen::exception::Exception& e ) {
		throw e;
	}
}

void MetaDataDownload::read(LPCTSTR meta_name) {
	root->clear();
	if ( wcslen( meta_name ) < 1 ) {

	}
	tstring path( getParentPath()->c_str() );
	path.append( TEXT( "\\" ) );
	path.append( meta_name );
	maschinen::file::FileHandler fh( path, GENERIC_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL );
	if ( !fh.isValid() ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), error, error_code );
		else throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str() );
	}
	try {
		std::string temp = "";
		{
			DWORD size_byte_read = 0;
			char buffer_mb[ MAX_LOADSTRING ];
			for ( auto i = fh.beginRead( buffer_mb, MAX_LOADSTRING - 1 ); i != fh.endRead(); i++ ) {
				buffer_mb[ i.number_of_byte_read ] = '\0';
				temp.append( buffer_mb );
			}
		}
		Poco::JSON::Object::Ptr json = deserialization( strToWstr( temp.c_str() )->c_str() );

		if ( !json->has( DISPOSITION ) || !json->has( FILES ) ) throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), L"Error, mata data is corrupt", 443 );
		if(json->get(DISPOSITION).extract<std::string>().compare(DISPOSITION_VALUE) != 0 ) throw MetaDataDoesNotMatchException();
		if(!json->has(ID) ) throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), L"Error, mata data is corrupt", 443 );
		id.assign( strToWstr( json->get( ID ).extract<std::string>().c_str() )->c_str() );
		target_path.assign( strToWstr( json->get( VAR_TARGET_PATH ).extract<std::string>().c_str() )->c_str() );

		Poco::JSON::Array::Ptr json_array = json->get( FILES ).extract<Poco::JSON::Array::Ptr>();
		for ( auto i = json_array->begin(); i != json_array->end(); i++ ) {
			Poco::JSON::Object::Ptr json_file = i->extract<Poco::JSON::Object::Ptr>();
			addFile(
				_strtoui64( json_file->get( FILES_ID ).extract<std::string>().c_str(), nullptr, 10 ),
				strToWstr( json_file->get( FILES_NAME ).extract<std::string>().c_str() )->c_str(),
				strtoull( json_file->get( FILES_SIZE ).extract<std::string>().c_str(), nullptr, 10 ),
				json_file->get(FILES_DIRECTORY).extract<bool>(),
				_strtoui64( json_file->get( FILES_PARENT ).extract<std::string>().c_str(), nullptr, 10 ),
				json_file->get( FILES_ORIGINAL ).extract<bool>()
			);
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

ULONGLONG MetaDataDownload::getByteWritten( MetaDataDownloadBaseFile* meta_data) {
	if ( meta_data == nullptr ) return 0;
	tstring path( target_path.c_str() );
	path.append( TEXT( "\\" ) );
	path.append( meta_data->getPath()->c_str() );
	maschinen::file::FileHandler fh( path.c_str(), GENERIC_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL );
	if ( fh.isValid() ) return fh.getSize();
	else return 0;
}

void MetaDataDownload::setUI( FileProcessWindow* ui ) { this->ui = ui; }
MetaDataDownload::~MetaDataDownload() {}

void MetaDataDownload::remove() const {
	if ( DeleteFile( getPath()->c_str() ) == 0 ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), error, error_code );
		else throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str() );
	}
}

void MetaDataDownload::addFile( QWORD id,
								LPCTSTR name,
								ULONGLONG size, 
								bool is_directory,
								QWORD parent_id, 
								bool original ) {
	MetaDataDownloadBaseFile* bf = nullptr;
	if ( is_directory ) bf = new MetaDataDownloadDirectory();
	else bf = new MetaDataDownloadFile();
	bf->setId( id );
	bf->setName( name );
	bf->setSize( size );
	bf->setOriginal( original );
	if ( parent_id != 0 && !original ) {
		MetaDataDownloadBaseFile* base = getFileById( parent_id );
		if ( MetaDataDownloadDirectory* dir = dynamic_cast< MetaDataDownloadDirectory* >( base ) ) {
			dir->addChildren( bf );
			bf->setParent( dir );
		}
	}
	else root->addChildren( bf );
}
ULONGLONG MetaDataDownload::getSize( MetaDataDownloadBaseFile* meta_data ) {
	return meta_data->getSize();
}

std::unique_ptr<tstring> MetaDataDownload::getFilePathByFileId( QWORD file_id ) {
	MetaDataDownloadBaseFile* bf = getFileById( file_id );
	if ( bf == nullptr ) return std::make_unique<tstring>();
	std::unique_ptr<tstring> res = std::make_unique<tstring>();
	res->assign( target_path.c_str() );
	res->append( TEXT( "\\" ) );
	res->append( bf->getPath()->c_str() );
	return res;
}

ULONGLONG MetaDataDownload::getSize() const noexcept {
	ULONGLONG size = 0;
	std::vector<MetaDataDownloadBaseFile*> files;
	filesToVector( files );
	for ( auto i = files.begin(); i != files.end(); i++ ) 
		size += ( *i )->getSize();
	return size;
}

ULONGLONG MetaDataDownload::getByteWritten() const noexcept {
	ULONGLONG size = 0;
	std::vector<MetaDataDownloadBaseFile*> files;
	filesToVector( files ); 
	for ( auto i = files.begin(); i != files.end(); i++ ) {
		if ( MetaDataDownloadFile* file = dynamic_cast< MetaDataDownloadFile* >( *i ) ) {
			tstring path( target_path.c_str() );
			path.append( TEXT( "\\" ) );
			path.append( ( *i )->getPath()->c_str() );
			maschinen::file::FileHandler fh( path.c_str(), GENERIC_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL );
			if ( fh.isValid() ) size += (ULONGLONG)fh.getSize();
		}
	}
	return size;
}

void MetaDataDownload::MetaDataToJsonArray( Poco::JSON::Array* arr, MetaDataDownloadDirectory* dir ) {
	{
		Poco::JSON::Object::Ptr temp = new Poco::JSON::Object;
		temp->set( FILES_ID, std::to_string( dir->getId() ).c_str() );
		temp->set( FILES_NAME, wstrToStr( dir->getName()->c_str() )->c_str() );
		temp->set( FILES_DIRECTORY, true );
		temp->set( FILES_SIZE, std::to_string( dir->getSize() ).c_str() );
		temp->set( FILES_PARENT, ( dir->getParent() == nullptr ) ? "0" : std::to_string( dir->getParent()->getId() ).c_str() );
		temp->set( FILES_ORIGINAL, dir->isOriginal() );
		arr->add( temp );
	}
	for ( auto i = dir->begin(); i != dir->end(); i++ ) {
		if ( MetaDataDownloadDirectory* dir = dynamic_cast< MetaDataDownloadDirectory* >( *i ) )
			MetaDataToJsonArray( arr, dir );
		else if ( MetaDataDownloadFile* file = dynamic_cast< MetaDataDownloadFile* >( *i ) ) {
			Poco::JSON::Object::Ptr temp = new Poco::JSON::Object;
			temp->set( FILES_ID, std::to_string( file->getId() ).c_str() );
			temp->set( FILES_NAME, wstrToStr( file->getName()->c_str() )->c_str() );
			temp->set( FILES_DIRECTORY, false );
			temp->set( FILES_SIZE, std::to_string( file->getSize() ).c_str() );
			temp->set( FILES_PARENT, ( file->getParent() == nullptr ) ? "0" : std::to_string( file->getParent()->getId() ).c_str() );
			temp->set( FILES_ORIGINAL, file->isOriginal() );
			arr->add( temp );
		}
	}
}

void MetaDataDownload::filesToVector( std::vector<MetaDataDownloadBaseFile*>& files, MetaDataDownloadBaseFile* start_position ) const noexcept {
	if ( start_position == nullptr ) start_position = root.get();
	if ( start_position != root.get() ) files.push_back( start_position );
	if ( MetaDataDownloadDirectory* dir = dynamic_cast< MetaDataDownloadDirectory* >( start_position ) ) {
		for ( auto i = dir->begin(); i != dir->end(); i++ ) {
			if ( MetaDataDownloadDirectory* dir1 = dynamic_cast< MetaDataDownloadDirectory* >( *i ) )
				filesToVector( files, *i );
			else if ( MetaDataDownloadFile* file = dynamic_cast< MetaDataDownloadFile* >( *i ) )
				files.push_back( *i );
		}
	}
}

MetaDataDownloadDirectory* MetaDataDownload::getFiles() const noexcept {return root.get();}
std::unique_ptr<tstring> MetaDataDownload::getTargetPath() const noexcept { return std::make_unique<tstring>( target_path.c_str() ); }
void MetaDataDownload::setTargetPath( LPCTSTR target_path ) noexcept { this->target_path.assign( target_path ); }
void MetaDataDownload::setTargetPath( tstring& target_path ) noexcept { this->target_path.assign( target_path.c_str() ); }
void MetaDataDownload::setTargetPath( tstring* target_path ) noexcept { this->target_path.assign( target_path->c_str() ); }

MetaDataDownloadBaseFile* MetaDataDownload::getFileById( QWORD id, MetaDataDownloadBaseFile* start_position ) const noexcept {
	if ( start_position == nullptr ) start_position = root.get();
	if ( start_position->getId() == id ) return start_position;
	if ( MetaDataDownloadDirectory* dir = dynamic_cast< MetaDataDownloadDirectory* >( start_position ) ) {
		for ( auto i = dir->begin(); i != dir->end(); i++ ) {
			if ( ( *i )->getId() == id ) return *i;
		}
		for ( auto i = dir->begin(); i != dir->end(); i++ ) {
			MetaDataDownloadBaseFile* bf = getFileById( id, *i );
			if ( bf != nullptr ) return bf;
		}
	}
	else if ( MetaDataDownloadFile* file = dynamic_cast< MetaDataDownloadFile* >( start_position ) ) {
		if ( file->getId() == id ) return file;
	}
	return nullptr;
}

bool MetaDataDownload::isFinished() const noexcept {
	return getSize() == getByteWritten();
}

ULONGLONG MetaDataDownload::getSize( QWORD id ) const noexcept {
	MetaDataDownloadBaseFile* file = getFileById( id );
	if ( !file ) return 0; else return file->getSize();
}
ULONGLONG MetaDataDownload::getByteWritten( QWORD id ) const noexcept {
	MetaDataDownloadBaseFile* bf = getFileById( id );
	if ( MetaDataDownloadFile* file = dynamic_cast< MetaDataDownloadFile* >( bf ) ) {
		tstring path( target_path.c_str() );
		path.append( TEXT( "\\" ) );
		path.append( file->getPath()->c_str() );
		maschinen::file::FileHandler fh( path.c_str(), GENERIC_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL );
		if ( fh.isValid() ) return ( ULONGLONG ) fh.getSize();
	}
	return 0;
}

void MetaDataDownload::deleteFiles() const noexcept {
	for ( auto i = root->begin(); i != root->end(); ++i ) {
		tstring path( target_path.c_str() );
		path.append( TEXT( "\\" ) );
		path.append( ( *i )->getPath()->c_str() );
		if ( GetFileAttributes( path.c_str() ) != INVALID_FILE_ATTRIBUTES ) DeleteFileW( path.c_str() );
	}
}
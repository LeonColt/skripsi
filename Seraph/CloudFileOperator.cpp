#include "stdafx.h"
#include "CloudFileOperator.h"


CloudFileOperator::CloudFileOperator() {
	waiter = NULL;
	flag_stop = true;
}

void CloudFileOperator::onPreExecute() {flag_stop = false;}

LPVOID CloudFileOperator::doInBackground( LPVOID input ) {
	waiter = NULL;
	first_load = true;
	for ( ;;) {
		if ( flag_stop ) break;
		if( !cloud_search_name.empty() ) {
			std::list<Cloud*> temp;
			flag_cancelling = false;
			result.assign( TEXT( "" ) );

			tstring cmd_line( TEXT( "SearchCloudSeraph " ) );
			cmd_line.append( ( tcslen( uc->getPhpSessionId().c_str() ) > 0 ) ? uc->getPhpSessionId() : TEXT( "empty" ) );
			cmd_line.append( TEXT( " " ) );
			cmd_line.append( akey->getAppKey()->c_str() );
			cmd_line.append( TEXT( " " ) );
			cmd_line.append( cloud_search_name.c_str() );
			cmd_line.append( TEXT( " " ) );
			cmd_line.append( ( uc->getLoginId()->length() > 0 ) ? uc->getLoginId()->c_str() : TEXT( "empty" ) );

			pipe.reset();
			pipe = std::make_unique<maschinen::pipe::AnonymoudPipe>( cmd_line.c_str() );
			pipe->closeWrite();

			DWORD number_of_bytes_avail = 0;
			while ( number_of_bytes_avail == 0 && !flag_cancelling ) {
				if ( !PeekNamedPipe( pipe->getRead(), NULL, NULL, NULL, &number_of_bytes_avail, NULL ) ) {
					TCHAR buffer[ MAX_LOADSTRING ];
					DWORD error_code;
					if ( GetLastError() == ERROR_BROKEN_PIPE ) break;
					if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
						MessageBox( parent->getWindowHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
					else MessageBox( parent->getWindowHandler(), TEXT( "Unknown Error" ), error_marker, MB_ICONERROR | MB_OK );
				}
				std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
			}
			if ( number_of_bytes_avail > 0 ) {
				char buffer_mb[ MAX_LOADSTRING ];
				std::string res = "";
				for ( auto i = pipe->beginRead( buffer_mb, MAX_LOADSTRING - 1 ); i != pipe->endRead(); ++i ) {
					buffer_mb[ i.number_of_byte_read ] = 0;
					res.append( buffer_mb );
				}
				pipe->closeRead();
				Poco::JSON::Object::Ptr json = deserialization( &res );
				if ( !json->has( "status" ) ) MessageBox( parent->getParent()->getHandler(), TEXT( "Invalid Result" ), error_marker, MB_ICONERROR | MB_OK );
				status = json->get( "status" ).convert<INT>();
				if ( status == 0 ) break;
				if ( !json->has( "result" ) ) MessageBox( parent->getParent()->getHandler(), TEXT( "Invalid Result" ), error_marker, MB_ICONERROR | MB_OK );
				tstring* temp = new tstring( strToWstr( json->get( "result" ).extract<std::string>().c_str() )->c_str() );
				publishProgress( ( LPVOID ) temp );
			}
			std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
		}
		else if ( Cloud* c = dynamic_cast< Cloud* >( ((FileControl*)parent)->getPointer() ) ) {
			flag_cancelling = false;
			result.assign( TEXT( "" ) );

			tstring cmd_line(TEXT("SeraphExecutor "));
			cmd_line.append( ( tcslen( uc->getPhpSessionId().c_str() ) > 0 ) ? uc->getPhpSessionId() : TEXT( "empty" ) );
			cmd_line.append( TEXT(" ") );
			cmd_line.append( akey->getAppKey()->c_str() );
			cmd_line.append( TEXT( " " ) );
			cmd_line.append( ( uc->getLoginId()->length() > 0 ) ? uc->getLoginId()->c_str() : TEXT( "empty" ) );
			cmd_line.append( TEXT( " " ) );
			cmd_line.append( TEXT( "2" ) );
			cmd_line.append( TEXT( " " ) );
			if ( DirectoryCloud* dc = dynamic_cast< DirectoryCloud* >( ( ( FileControl* ) parent )->getPointer() ) )
				cmd_line.append( to_tstring( dc->getId() ) );
			else cmd_line.append( TEXT( "0" ) );
			if ( !first_load ) {
				time_t waktu;
				_time64( &waktu );
				cmd_line.append( TEXT( " " ) );
				cmd_line.append( to_tstring(waktu) );
			}
			else {
				cmd_line.append( TEXT( " " ) );
				cmd_line.append( TEXT( "-1" ) );
			}

			pipe.reset();
			pipe = std::make_unique<maschinen::pipe::AnonymoudPipe>( cmd_line.c_str() );
			pipe->closeWrite();

			DWORD number_of_bytes_avail = 0;
			while ( number_of_bytes_avail == 0 && !flag_cancelling && !flag_stop ) {
				if ( !PeekNamedPipe( pipe->getRead(), NULL, NULL, NULL, &number_of_bytes_avail, NULL ) ) {
					TCHAR buffer[ MAX_LOADSTRING ];
					DWORD error_code;
					if ( GetLastError() == ERROR_BROKEN_PIPE ) break;
					if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
						MessageBox( parent->getWindowHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
					else MessageBox( parent->getWindowHandler(), TEXT( "Unknown Error" ), error_marker, MB_ICONERROR | MB_OK );
				}
				std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
			}
			if ( number_of_bytes_avail > 0 ) {
				char buffer_mb[ MAX_LOADSTRING ];
				std::string res = "";
				for( auto i = pipe->beginRead(buffer_mb, MAX_LOADSTRING -1); i != pipe->endRead(); ++i ) {
					buffer_mb[ i.number_of_byte_read ] = 0;
					res.append( buffer_mb );
				}
				pipe->closeRead();
				Poco::JSON::Object::Ptr json = deserialization( &res );
				if ( !json->has( "status" ) ) MessageBox( parent->getParent()->getHandler(), TEXT( "Invalid Result" ), error_marker, MB_ICONERROR | MB_OK );
				status = json->get( "status" ).convert<INT>();
				if ( status == 0 ) break;
				if ( !json->has( "result" ) ) MessageBox( parent->getParent()->getHandler(), TEXT( "Invalid Result" ), error_marker, MB_ICONERROR | MB_OK );
				tstring* temp = new tstring( strToWstr( json->get( "result" ).extract<std::string>().c_str() )->c_str() );
				publishProgress( ( LPVOID ) temp );
			}
		}
		else if( CloudRecycleBin* crb = dynamic_cast<CloudRecycleBin*>( ((FileControl* )parent )->getPointer() ) ) {
			flag_cancelling = false;
			result.assign( TEXT( "" ) );

			tstring cmd_line( TEXT( "SeraphExecutor " ) );
			cmd_line.append( ( tcslen( uc->getPhpSessionId().c_str() ) > 0 ) ? uc->getPhpSessionId() : TEXT( "empty" ) );
			cmd_line.append( TEXT( " " ) );
			cmd_line.append( akey->getAppKey()->c_str() );
			cmd_line.append( TEXT( " " ) );
			cmd_line.append( ( uc->isCredential() ) ? uc->getLoginId()->c_str() : TEXT( "empty" ) );
			if ( !first_load ) {
				time_t waktu;
				_time64( &waktu );
				cmd_line.append( TEXT( " " ) );
				cmd_line.append( to_tstring( waktu ) );
			}
			else {
				cmd_line.append( TEXT( " " ) );
				cmd_line.append( TEXT( "-1" ) );
			}

			pipe.reset();
			pipe = std::make_unique<maschinen::pipe::AnonymoudPipe>( cmd_line.c_str() );
			pipe->closeWrite();

			DWORD number_of_bytes_avail = 0;
			while ( number_of_bytes_avail == 0 && !flag_stop ) {
				if ( !PeekNamedPipe( pipe->getRead(), NULL, NULL, NULL, &number_of_bytes_avail, NULL ) ) {
					TCHAR buffer[ MAX_LOADSTRING ];
					DWORD error_code;
					if ( GetLastError() == ERROR_BROKEN_PIPE ) break;
					if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
						MessageBox( parent->getHandler(), buffer, TEXT( "Error" ), MB_OK | MB_ICONERROR );
					else MessageBox( parent->getWindowHandler(), TEXT( "Unknown Error" ), error_marker, MB_ICONERROR | MB_OK );
				}
				std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
			}
			if ( number_of_bytes_avail > 0 ) {
				char buffer_mb[ MAX_LOADSTRING ];
				std::string res = "";
				for ( auto i = pipe->beginRead( buffer_mb, MAX_LOADSTRING - 1 ); i != pipe->endRead(); ++i ) {
					buffer_mb[ i.number_of_byte_read ] = 0;
					res.append( buffer_mb );
				}
				pipe->closeRead();
				Poco::JSON::Object::Ptr json = deserialization( &res );
				if ( !json->has( "status" ) ) MessageBox( parent->getParent()->getHandler(), TEXT( "Invalid Result" ), error_marker, MB_ICONERROR | MB_OK );
				status = json->get( "status" ).convert<INT>();
				if ( status == 0 ) break;
				if ( !json->has( "result" ) ) MessageBox( parent->getParent()->getHandler(), TEXT( "Invalid Result" ), error_marker, MB_ICONERROR | MB_OK );
				tstring* temp = new tstring( strToWstr( json->get( "result" ).extract<std::string>().c_str() )->c_str() );
				publishProgress( ( LPVOID ) temp );
			}
		}
		else {
			waiter = CreateEvent( NULL, TRUE, FALSE, NULL );
			if ( waiter == NULL ) continue;
			WaitForSingleObject( waiter, INFINITE );
			CloseHandle( waiter );
			waiter = NULL;
		}
	}
	return nullptr;
}

void CloudFileOperator::onProgressUpdate( LPVOID progress, bool on_task ) {
	std::unique_ptr<tstring> input( (tstring*)progress );
	( ( FileControl* ) parent )->setFileControlLoadEnable();
	if ( status != HTTP_STATUS_OK ) {
		MessageBox( parent->getParent()->getHandler(), result.c_str(), error_marker, MB_ICONERROR | MB_OK );
		first_load = true;
		return;
	}
	try {
		Poco::JSON::Object::Ptr json = deserialization( input->c_str() );
		OutputDebugStringW( input->c_str() );
		OutputDebugStringW( L"\n" );
		if ( !json->has( VAR_JSON_STATUS ) ) {
			MessageBox( parent->getParent()->getHandler(), L"Invalid Json format", error_marker, MB_ICONERROR | MB_OK );
			first_load = true;
			return;
		}
		if ( json->get( VAR_JSON_STATUS ).extract<int>() == 0 ) {
			if ( json->has( VAR_JSON_MESSAGE ) )
				MessageBox( parent->getParent()->getHandler(), strToWstr( json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str() )->c_str(), error_marker, MB_ICONERROR | MB_OK );
			else MessageBox( parent->getParent()->getHandler(), L"Unknown Error Occured", error_marker, MB_ICONERROR | MB_OK );
			first_load = true;
			return;
		}
		if ( !json->has( VAR_JSON_RESULT ) ) {
			if ( json->has( VAR_JSON_MESSAGE ) )
				MessageBox( parent->getParent()->getHandler(), strToWstr( json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str() )->c_str(), error_marker, MB_ICONERROR | MB_OK );
			else MessageBox( parent->getParent()->getHandler(), L"Unknown Error Occured", error_marker, MB_ICONERROR | MB_OK );
			first_load = true;
			return;
		}
		Poco::DynamicAny var_result = json->get( VAR_JSON_RESULT );
		if ( var_result.type() != typeid( Poco::JSON::Array::Ptr ) ) MessageBox( parent->getParent()->getHandler(), L"Invalid Json format", error_marker, MB_ICONERROR | MB_OK );
		Poco::JSON::Array::Ptr arr = var_result.extract<Poco::JSON::Array::Ptr>();
		Poco::JSON::Object::Ptr file;
		bool exists;
		if( !cloud_search_name.empty() ) {
			std::list<Cloud*> clouds;

			for ( auto iter = arr->begin(); iter != arr->end(); ++iter ) {
				exists = false;
				file = iter->extract<Poco::JSON::Object::Ptr>();
				Cloud* cloud = nullptr;
				if ( file->get( GET_FILE_IS_DIRECTORY ).extract<bool>() )
					cloud = new DirectoryCloud();
				else cloud = new FileCloud();
				setCloudProperty( cloud, file, nullptr );
				clouds.push_back( cloud );
			}

			for ( auto i = clouds.cbegin(); i != clouds.cend(); ++i )
				( ( FileControl* ) parent )->addFileToBuffer( *i );
			( ( FileControl* ) parent )->refreshListView();
			cloud_search_name.clear();
			( ( FileControl* ) parent )->setFlagSearchCloudFinish( true );
			( ( FileControl* ) parent )->endSearch();
		}
		else if ( DirectoryCloud* dc = dynamic_cast< DirectoryCloud* >( ( ( FileControl* ) parent )->getPointer() ) ) {
			std::vector<QWORD> existing;
			existing.reserve( arr->size() );
			for ( auto iter = arr->begin(); iter != arr->end(); ++iter ) {
				exists = false;
				file = iter->extract<Poco::JSON::Object::Ptr>();
				Cloud* cloud = nullptr;
				for( auto i = dc->begin(); i != dc->end(); ++i ) {
					if( _strtoui64( file->get( GET_FILE_ID ).extract<std::string>().c_str(), nullptr, 10 ) == (*i)->getId() ) {
						cloud = *i;
						exists = true;
						break;
					}
				}
				if( !exists ) {
					if( file->get(GET_FILE_IS_DIRECTORY).extract<bool>() ) 
						cloud = new DirectoryCloud();
					else cloud = new FileCloud();
				}
				setCloudProperty( cloud, file, dc);
				if( !exists ) {
					dc->addChildren( cloud );
					cloud->setParentId( dc->getId() );
					existing.push_back( cloud->getId() );
				}
				else existing.push_back( cloud->getId() );
			}
			std::list<Cloud*>* children = dc->getChildren();
			bool exists = false;
			for( auto i = children->begin(); i != children->end(); ) {
				exists = false;
				for( auto j = existing.cbegin(); j != existing.cend(); ++j ) {
					if( (*i )->getId() == *j ) {
						exists = true;
						break;
					}
				}
				if ( !exists ) i = children->erase( std::remove( children->begin(), children->end(), *i ), children->end() );
				else ++i;
			}
			( ( FileControl* ) parent )->prosesListView();
		}
		else if( CloudRecycleBin* crb = dynamic_cast<CloudRecycleBin*>( ((FileControl* )parent )->getPointer() ) ){
			for ( auto i = arr->begin(); i != arr->end(); ++i ) {
				file = i->extract<Poco::JSON::Object::Ptr>();
				CloudRecycleBinObject* crbo = new CloudRecycleBinObject();
				crbo->setId( _strtoui64( file->get( GET_RECYCLE_BIN_ID ).extract<std::string>().c_str(), nullptr, 10 ) );
				crbo->setName( strToWstr( file->get( GET_RECYCLE_BIN_NAME ).extract<std::string>().c_str() )->c_str() );
				crbo->setOwner( strToWstr( file->get( GET_RECYCLE_BIN_OWNER ).extract<std::string>().c_str() )->c_str() );
				crbo->setAsDirectory( file->get( GET_RECYCLE_BIN_DIRECTORY ).extract<bool>() );
				bool exists = false;
				for ( auto j = crb->begin(); j != crb->end(); ++j ) {
					if ( crbo->getId() == ( ( CloudRecycleBinObject* ) ( *j ) )->getId() ) {
						exists = true;
						break;
					}
				}
				if ( !exists ) crb->addChildren( crbo );
			}
			first_load = false;
			( ( FileControl* ) parent )->prosesListView();
		}
		first_load = false;
	} catch ( SeraphException& e ) {
		MessageBox( parent->getParent()->getHandler(), e.getMessage()->c_str(), error_marker, MB_ICONERROR | MB_OK );
		first_load = true;
	} catch ( Poco::Exception& e ) {
		MessageBox( parent->getParent()->getHandler(), strToWstr( e.message().c_str() )->c_str(), error_marker, MB_ICONERROR | MB_OK );
		first_load = true;
	}
}

void CloudFileOperator::onPostExecute( LPVOID result ) {return;}

void CloudFileOperator::createNewFolder( DirectoryCloud* dc ) const {
	CloudCreateNewFolder* ccnf = new CloudCreateNewFolder();
	ccnf->setParent( parent );
	ccnf->setAutoDelete();
	ccnf->setAkey( akey );
	ccnf->setSetting( setting );
	ccnf->setUserCredential( uc );
	ccnf->setErrorMarker( error_marker );
	ccnf->setParentId( dc->getId() );
	ccnf->execute();
}


void CloudFileOperator::renameFile( QWORD id, LPCTSTR new_name ) const noexcept {
	CloudFileRenamer* renamer = new CloudFileRenamer( id, new_name );
	renamer->setAutoDelete();
	renamer->setParent( parent );
	renamer->setAkey( akey );
	renamer->setSetting( setting );
	renamer->setUserCredential( uc );
	renamer->setParent( parent );
	renamer->execute();
}

void CloudFileOperator::stop() { flag_cancelling = flag_stop = true; if ( waiter != NULL ) SetEvent( waiter );}
bool CloudFileOperator::isStopped() const noexcept { 
	return flag_stop; }
void CloudFileOperator::restart() {
	flag_cancelling = true;
	first_load = true;
	if ( waiter != NULL ) SetEvent( waiter );
}

CloudFileOperator::~CloudFileOperator() {stop();}
void CloudFileOperator::setSearchName( LPCTSTR cloud_search_name ) {
	this->cloud_search_name.assign( cloud_search_name );
}

void CloudFileOperator::setCloudProperty( Cloud*& cloud, Poco::JSON::Object::Ptr& json, DirectoryCloud* parent ) {
	//id
	cloud->setId( _strtoui64( json->get( GET_FILE_ID ).extract<std::string>().c_str(), nullptr, 10 ) );
	//size
	cloud->setSize( _strtoui64( json->get( GET_FILE_SIZE ).extract<std::string>().c_str(), nullptr, 10 ) );
	//name
	cloud->setName( strToWstr( json->get( GET_FILE_NAME ).extract<std::string>().c_str() )->c_str() );
	//owner
	cloud->setOwner( strToWstr( json->get( GET_FILE_OWNER ).extract<std::string>().c_str() )->c_str() );
	//exists
	if ( json->get( GET_FILE_DOWNLOAD_BY_ALL ).extract<bool>() ) cloud->setDownloadable();
	else if ( json->get( GET_FILE_DOWNLOAD_ALLOWED_CURRENT_USER ).extract<bool>() ) cloud->setDownloadable();
	if ( json->get( GET_FILE_DELETE_ALLOWED_CURRENT_USER ).extract<bool>() ) cloud->setDeletable();
	
	if ( json->get( GET_FILE_SEEN_BY_ALL ).extract<bool>() ) cloud->setSeenByAll();
	if ( json->get( GET_FILE_DOWNLOAD_BY_ALL ).extract<bool>() ) cloud->setDownloadByAll();
	if ( json->get( GET_FILE_MODIFY_BY_ALL ).extract<bool>() ) cloud->setModifyByAll();
	tstring path( parent->getPath()->c_str() );
	path.append( cloud->getName()->c_str() );
	path.append( TEXT( "\\" ) );
	cloud->setPath( path.c_str() );
}
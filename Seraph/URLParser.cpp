#include "stdafx.h"
#include "URLParser.h"


URLParser::URLParser() : LOCAL(1), CLOUD(2) {
	flag_cancelling = false;
}

LPVOID URLParser::doInBackground( LPVOID input ) {
	std::unique_ptr<tstring> path( ( tstring* ) input );
	if ( path->compare( TEXT( "" ) ) == 0 ) return ( ( LPVOID ) new tstring( TEXT( "" ) ) );
	std::vector<tstring> paths;
	size_t size = tstrSplit( path->c_str(), &paths, TEXT( "\\" ) );
	INT type = decideType( paths );

	if ( type == LOCAL ) {
		pointer = loadLocalByPath( path->c_str() ).release();
		return ( ( LPVOID ) new tstring( TEXT( "" ) ) );
	}
	else if ( type == CLOUD ) {
		if(  size == 1 ) {
			if( paths.begin()->compare( setting->getCloudRootName()->c_str() ) == 0 ) {
				pointer = new DirectoryCloud();
				( ( DirectoryCloud* ) pointer )->setId( 0 );
				( ( DirectoryCloud* ) pointer )->setName( setting->getCloudRootName()->c_str() );
				tstring path = setting->getCloudRootName()->c_str();
				path.append( TEXT( "\\" ) );
				( ( DirectoryCloud* ) pointer )->setPath( path.c_str() );
				( ( DirectoryCloud* ) pointer )->setModifyByAll( true );
				( ( DirectoryCloud* ) pointer )->setDownloadByAll( false );
				( ( DirectoryCloud* ) pointer )->setSeenByAll( true );
				( ( DirectoryCloud* ) pointer )->setDownloadable( false );
				return ( ( LPVOID ) new tstring( TEXT( "" ) ) );
			}
			else if( paths.begin()->compare( setting->getCloudRecycleBinRootName()->c_str() ) == 0 ) {
				pointer = new CloudRecycleBin();
				( ( CloudRecycleBin* ) pointer )->setName( setting->getCloudRecycleBinRootName()->c_str() );
				tstring path = setting->getCloudRecycleBinRootName()->c_str();
				path.append( TEXT( "\\" ) );
				( ( CloudRecycleBin* ) pointer )->setPath( path.c_str() );
				return ( ( LPVOID ) new tstring( TEXT( "" ) ) );
			}
		}

		tstring cmd_line( TEXT( "SeraphExecutor " ) );
		cmd_line.append( ( tcslen( uc->getPhpSessionId().c_str() ) > 0 ) ? uc->getPhpSessionId() : TEXT( "empty" ) );
		cmd_line.append( TEXT( " " ) );
		cmd_line.append( akey->getAppKey()->c_str() );
		cmd_line.append( TEXT( " " ) );
		cmd_line.append( ( uc->getLoginId()->length() > 0 ) ? uc->getLoginId()->c_str() : TEXT( "empty" ) );
		cmd_line.append( TEXT( " " ) );
		cmd_line.append( TEXT( "1 " ) );
		cmd_line.append( path->c_str() );

		pipe.reset( new maschinen::pipe::AnonymoudPipe( cmd_line.c_str() ) );
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
			if ( !json->has( "result" ) ) MessageBox( parent->getParent()->getHandler(), TEXT( "Invalid Result" ), error_marker, MB_ICONERROR | MB_OK );
			tstring* temp = new tstring( strToWstr( json->get( "result" ).extract<std::string>().c_str() )->c_str() );
			return ( LPVOID ) temp;
		}
	}
	return ( ( LPVOID ) new tstring( TEXT( "" ) ) );
}
void URLParser::onPostExecute( LPVOID input ){
	std::unique_ptr<tstring> res( ( tstring* ) input );
	( ( FileControl* ) parent )->setFileControlLoadEnable();
	if ( res->empty() ) {
		( ( FileControl* ) parent )->setPointer( pointer );
		( ( FileControl* ) parent )->prosesListView();
		return;
	}
	if ( status != HTTP_STATUS_OK ) {
		MessageBox( parent->getParent()->getHandler(), res->c_str(), error_marker, MB_ICONERROR | MB_OK );
		return;
	}
	try {
		Poco::JSON::Object::Ptr json = deserialization( res->c_str() );
		if ( !json->has( VAR_JSON_STATUS ) ) {
			MessageBox( parent->getParent()->getHandler(), L"Invalid Json format", error_marker, MB_ICONERROR | MB_OK );
			return;
		}
		if ( json->get( VAR_JSON_STATUS ).extract<int>() == 0 ) {
			if ( json->has( VAR_JSON_MESSAGE ) )
				MessageBox( parent->getParent()->getHandler(), strToWstr( json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str() )->c_str(), error_marker, MB_ICONERROR | MB_OK );
			else MessageBox( parent->getParent()->getHandler(), L"Unknown Error Occured", error_marker, MB_ICONERROR | MB_OK );
			return;
		}
		if ( !json->has( VAR_JSON_RESULT ) ) {
			if ( json->has( VAR_JSON_MESSAGE ) )
				MessageBox( parent->getParent()->getHandler(), strToWstr( json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str() )->c_str(), error_marker, MB_ICONERROR | MB_OK );
			else MessageBox( parent->getParent()->getHandler(), L"Unknown Error Occured", error_marker, MB_ICONERROR | MB_OK );
			return;
		}
		Poco::DynamicAny var_result = json->get( VAR_JSON_RESULT );
		Poco::JSON::Object::Ptr file = var_result.extract<Poco::JSON::Object::Ptr>();

		if ( file->get( GET_FILE_IS_DIRECTORY ).extract<bool>() )
			pointer = new DirectoryCloud();
		else pointer = new FileCloud();
		
		//id
		( ( Cloud* ) pointer )->setId( _strtoui64( file->get( GET_FILE_ID ).extract<std::string>().c_str(), nullptr, 10 ) );
		//size
		( ( Cloud* ) pointer )->setSize( _strtoui64( file->get( GET_FILE_SIZE ).extract<std::string>().c_str(), nullptr, 10 ) );
		//name
		( ( Cloud* ) pointer )->setName( strToWstr( file->get( GET_FILE_NAME ).extract<std::string>().c_str() )->c_str() );
		//owner
		( ( Cloud* ) pointer )->setOwner( strToWstr( file->get( GET_FILE_OWNER ).extract<std::string>().c_str() )->c_str() );
		//exists
		
		if ( file->get( GET_FILE_DOWNLOAD_BY_ALL ).extract<bool>() ) ( ( Cloud* ) pointer )->setDownloadable();
		else if ( file->get( GET_FILE_DOWNLOAD_ALLOWED_CURRENT_USER ).extract<bool>() ) ( ( Cloud* ) pointer )->setDownloadable();
		if ( file->get( GET_FILE_DELETE_ALLOWED_CURRENT_USER ).extract<bool>() ) ( ( Cloud* ) pointer )->setDeletable();

		if ( file->get( GET_FILE_SEEN_BY_ALL ).extract<bool>() ) ( ( Cloud* ) pointer )->setSeenByAll();
		if ( file->get( GET_FILE_DOWNLOAD_BY_ALL ).extract<bool>() ) ( ( Cloud* ) pointer )->setDownloadByAll();
		if ( file->get( GET_FILE_MODIFY_BY_ALL ).extract<bool>() ) ( ( Cloud* ) pointer )->setModifyByAll();
		tstring path( ( ( FileControl* ) parent )->getPointer()->getPath()->c_str() );
		path.append( ( ( Cloud* ) pointer )->getName()->c_str() );
		path.append( TEXT( "\\" ) );
		( ( Cloud* ) pointer )->setPath( path.c_str() );
	} catch ( SeraphException& e ) {
		MessageBox( parent->getParent()->getHandler(), e.getMessage()->c_str(), error_marker, MB_ICONERROR | MB_OK );
	} catch ( Poco::Exception& e ) {
		MessageBox( parent->getParent()->getHandler(), strToWstr( e.message().c_str() )->c_str(), error_marker, MB_ICONERROR | MB_OK );
	}
	( ( FileControl* ) parent )->setPointer( pointer );
	( ( FileControl* ) parent )->prosesListView();
}

INT URLParser::decideType( std::vector<tstring>& paths ) {
	if ( paths.size() > 0 ) {
		if ( paths.begin()->compare( setting->getCloudRootName()->c_str() ) == 0
			 || paths.begin()->compare(setting->getCloudRecycleBinRootName()->c_str() ) == 0
			 )
			return CLOUD;
		else return LOCAL;
	}
	return NULL;
}

URLParser::~URLParser() {
}

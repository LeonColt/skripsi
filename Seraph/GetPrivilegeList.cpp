#include "stdafx.h"
#include "GetPrivilegeList.h"


GetPrivilegeList::GetPrivilegeList() :
	TYPE_SEEN( TEXT( "cf-gudlof-34949284982942842748274234" ) ),
	TYPE_DOWNLOAD( TEXT( "cf-gudlof-394829048290348204820482" ) ),
	TYPE_UPLOAD_ON_DIRECTORY( TEXT( "cf-gudlof-93849028402984028402948390" ) ) {
	cloud = nullptr;
	type.assign( TYPE_SEEN.c_str() );
}

LPVOID GetPrivilegeList::doInBackground( LPVOID input ) {
	waiter = NULL;
	for ( ;; ) {
		if ( flag_stop ) break;
		tstring result( TEXT( "" ) );

		tstring cmd_line( TEXT( "SeraphExecutor " ) );
		cmd_line.append( ( tcslen( uc->getPhpSessionId().c_str() ) > 0 ) ? uc->getPhpSessionId() : TEXT( "empty" ) );
		cmd_line.append( TEXT( " " ) );
		cmd_line.append( akey->getAppKey()->c_str() );
		cmd_line.append( TEXT( " " ) );
		cmd_line.append( ( uc->isCredential() ) ? uc->getLoginId()->c_str() : TEXT( "empty" ) );
		cmd_line.append( TEXT( " " ) );
		cmd_line.append( TEXT( "4 " ) );
		cmd_line.append( to_tstring( cloud->getId() ).c_str() );
		cmd_line.append( TEXT( " " ) );
		cmd_line.append( type.c_str() );

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
					OutputDebugStringW( buffer );
				else OutputDebugStringW( L"unknown error" );
			}
			Sleep( 500 );
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
		std::this_thread::sleep_for( std::chrono::milliseconds( 2500 ) );
	}
	return nullptr;
}
void GetPrivilegeList::onProgressUpdate( LPVOID progress, bool on_task ) {
	std::unique_ptr<tstring> input( ( tstring* ) progress );
	if ( status != HTTP_STATUS_OK ) {
		MessageBox( parent->getParent()->getHandler(), input->c_str(), error_marker, MB_ICONERROR | MB_OK );
		return;
	}
	try {
		Poco::JSON::Object::Ptr json = deserialization( input->c_str() );
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
		if ( var_result.type() != typeid( Poco::JSON::Array::Ptr ) ) MessageBox( parent->getParent()->getHandler(), L"Invalid Json format", error_marker, MB_ICONERROR | MB_OK );
		Poco::JSON::Array::Ptr arr = var_result.extract<Poco::JSON::Array::Ptr>();
		Poco::JSON::Object::Ptr file;

		for ( auto i = arr->begin(); i != arr->end(); ++i ) {
			file = i->extract<Poco::JSON::Object::Ptr>();
			std::unique_ptr<User> user = std::make_unique<User>();
			user->setId( _strtoui64( file->get( ID ).extract<std::string>().c_str(), nullptr, 10 ) );
			user->setUsername( strToWstr( file->get( USERNAME ).extract<std::string>() )->c_str() );
			bool exists = false;
			for ( auto j = begin(); j != end(); ++j ) {
				if ( user->compare( ( *j ).get() ) ) {
					exists = true; break;
				}
			}
			if ( !exists ) users.push_back( std::move( user ) );
		}
		( ( CloudPropertyPrivilegeList* ) parent )->setUsers( users.cbegin(), users.cend() );
	} catch ( SeraphException& e ) {
		MessageBox( parent->getParent()->getHandler(), e.getMessage()->c_str(), error_marker, MB_ICONERROR | MB_OK );
	} catch ( Poco::Exception& e ) {
		MessageBox( parent->getParent()->getHandler(), strToWstr( e.message().c_str() )->c_str(), error_marker, MB_ICONERROR | MB_OK );
	}
}
void GetPrivilegeList::onPostExecute( LPVOID result ) { return; }
void GetPrivilegeList::stop() { flag_stop = true; }
void GetPrivilegeList::setCloud( Cloud* cloud ) { this->cloud = cloud; }

std::vector<std::unique_ptr<User>>::const_iterator GetPrivilegeList::begin() {
	return users.cbegin();
}
std::vector<std::unique_ptr<User>>::const_iterator GetPrivilegeList::end() {
	return users.cend();
}

GetPrivilegeList::~GetPrivilegeList() {
}

void GetPrivilegeList::setTypeSeen(){type.assign( TYPE_SEEN.c_str() );}
void GetPrivilegeList::setTypeDownload(){type.assign( TYPE_DOWNLOAD.c_str() );}
void GetPrivilegeList::setTypeUploadOnDirectory() {type.assign( TYPE_UPLOAD_ON_DIRECTORY.c_str() );}

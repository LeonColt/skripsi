// SeraphExecutor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <Poco\JSON\JSON.h>
#include <Poco\JSON\JSONException.h>
#include <Poco\JSON\Parser.h>
#include <Poco\Dynamic\Var.h>
#include "HttpParams.h"
#include "HttpRequest.h"
#include "FileHandler.h"
#include "SeraphCommon.h"
#include "Setting.h"

Poco::JSON::Object::Ptr extract( std::string& str );

#define INSTRUCTION_PATH_PARSER 1
#define INSTRUCTION_GET_META_FILE 2
#define INSTRUCTION_GET_CLOUD_RECYCLE_BIN 3
#define INSTRUCTION_PRIVILEGE_COLLECTOR 4
#define INSTRUCTION_SEARCH 5

#define VAR_JSON_STATUS "nmlf-json-42323"
#define VAR_JSON_RESULT "nmlf-json-24352"
#define VAR_JSON_MESSAGE "nmlf-json-23432"

namespace instruction {
	namespace path_parser {
		const tstring REQUEST = TEXT( "cf-req-3472847249827498274927498274298" );
		const tstring INPUT_PATH = TEXT( "cf-fp-3494829479827428472894" );
		const tstring LOGIN_ID = TEXT( "cf-fp-738917381237819371893719823" );
	}
	namespace get_meta_file {
		const tstring REQUEST = TEXT( "cf-req-858842514588585555" );
		const tstring INPUT_PARENT = TEXT( "cf-gf-23433434" );
		const tstring INPUT_TIME = TEXT( "cf-gf-897531590" );
		const tstring LOGIN_ID = TEXT( "cf-gf-93048903284203" );
	}
	namespace get_cloud_recycle_bin {
		const tstring REQUEST = TEXT( "cf-req-34994058305435035395" );
		const tstring INPUT_TIME = TEXT( "cf-grb-93080328385495305839485" );
		const tstring LOGIN_ID = TEXT( "cf-grb-3948092489024" );
	}
	namespace privilege_collector {
		const tstring REQUEST = TEXT( "cf-req-3490389547348975395834" );
		const tstring INPUT_FILE_ID = TEXT( "cf-gudlof-394834839089348324" );
		const tstring INPUT_PRIVILEGE_TYPE = TEXT( "cf-gudlof-3420894028492048920482904" );
		const tstring LOGIN_ID = TEXT( "cf-gudlof-394834839089348323904230489035495793" );
	}
	namespace search_cloud {
		const tstring REQUEST = TEXT( "cf-req-980948320948039849302809238034980923842378" );
		const tstring FILE_NAME = TEXT( "nmlf-sf-2409308908409274832978138213" );
		const tstring LOGIN_ID = TEXT( "nmlf-sf-385904890284092750248209405980437580354" );
	}
}

void requestPathParser( LPCTSTR session, LPCTSTR akey, LPCTSTR login_id, LPCTSTR path );
void getMetaFile( LPCTSTR session, LPCTSTR akey, QWORD parent_file_id, time_t waktu, LPCTSTR login_id );
void GetUserListOnFilePrivilege( LPCTSTR session, LPCTSTR akey, LPCTSTR login_id, LPCTSTR file_id, LPCTSTR privilege_type );
void getCloudReyclecleBin( LPCTSTR session, LPCTSTR akey, time_t waktu, LPCTSTR login_id );
void searchFile( LPCTSTR session, LPCTSTR akey, LPCTSTR name, LPCTSTR login_id );
/*
max 32767
index 1 = session_id //max 40
index 2 = akey //25 karakter
index 3 = login_id //max 255 character
index 4 = instruction //3 characters
index n = additional // rest max 32443
*/
int _tmain( int argc, TCHAR *argv[] ) {
	HANDLE in, out;
	in = GetStdHandle( STD_INPUT_HANDLE );
	out = GetStdHandle( STD_INPUT_HANDLE );
	if ( out == INVALID_HANDLE_VALUE || out == NULL || in == INVALID_HANDLE_VALUE || in == NULL ) ExitProcess( 1 );
	if ( argc < 4 ) {
		Poco::JSON::Object obj;
		obj.set( "status", 0 );
		obj.set( "result", "Invalid Input" );

		std::unique_ptr<tstring> res = std::make_unique<tstring>();
		std::ostringstream oss;
		Poco::JSON::Stringifier::stringify( obj, oss );
		printf( oss.str().c_str() );
	}
	else {
		INT inst = _ttoi( argv[ 4 ] );
		tstring session = ( wcscmp( argv[ 1 ], TEXT( "empty" ) ) == 0 ) ? TEXT( "" ) : argv[ 1 ];
		tstring login_id = ( wcscmp( argv[ 3 ], TEXT( "empty" ) ) == 0 ) ? TEXT( "" ) : argv[ 3 ];
		if ( inst == INSTRUCTION_PATH_PARSER )
			requestPathParser( session.c_str(), argv[ 2 ], login_id.c_str(), argv[ 5 ] );
		else if ( inst == INSTRUCTION_GET_META_FILE )
			getMetaFile( session.c_str(), argv[ 2 ], _tcstoui64( argv[ 5 ], nullptr, 10 ), _tcstoui64( argv[ 6 ], nullptr, 10 ), login_id.c_str() );
		else if ( inst == INSTRUCTION_GET_CLOUD_RECYCLE_BIN )
			getCloudReyclecleBin( session.c_str(), argv[ 2 ], _tcstoui64( argv[ 5 ], nullptr, 10 ), login_id.c_str() );
		else if ( inst == INSTRUCTION_PRIVILEGE_COLLECTOR )
			GetUserListOnFilePrivilege( session.c_str(), argv[ 2 ], login_id.c_str(), argv[ 5 ], argv[ 6 ] );
		else if ( inst == INSTRUCTION_SEARCH )
			searchFile( session.c_str(), argv[ 2 ], argv[ 5 ], login_id.c_str() );
	}
	return 0;
}

void searchFile( LPCTSTR session, LPCTSTR akey, LPCTSTR name, LPCTSTR login_id ) {
	maschinen::wininet::HttpParams params;
	params.add( internet::variable::VAR_PHPSESSID_SEND, session, params.METHOD_GET );
	params.add( instruction::search_cloud::FILE_NAME, name, params.METHOD_POST );
	params.add( instruction::search_cloud::LOGIN_ID, login_id, params.METHOD_POST );
	maschinen::wininet::HttpRequest req;
	try {
		Setting setting;
		setting.read();
		req.open( TEXT( "" ), req.ACCESS_TYPE_PRE_CONFIG );
		req.connect( setting.getServerName()->c_str() );
		req.openRequest( setting.getUrl()->c_str(), req.VERSION_HTTP_11, NULL, NULL,
						 INTERNET_FLAG_PRAGMA_NOCACHE |
						 INTERNET_FLAG_NO_CACHE_WRITE |
						 INTERNET_FLAG_NO_COOKIES |
						 INTERNET_FLAG_NO_UI |
						 INTERNET_FLAG_NO_AUTO_REDIRECT |
						 INTERNET_FLAG_RELOAD, &params );
		req.setSendTimeOut( req.REQUEST_TIME_OUT_INFINITE );
		req.setReceiveTimeOut( req.REQUEST_TIME_OUT_INFINITE );
		maschinen::wininet::HttpHeaders headers;
		headers.add( TEXT( "Content-Type" ), TEXT( "application/x-www-form-urlencoded" ) );
		if ( akey != nullptr ) headers.add( internet::variable::VAR_APP_KEY, akey );
		headers.add( internet::variable::VAR_REQUEST, instruction::search_cloud::REQUEST );
		headers.add( internet::variable::VAR_REQUEST_TYPE, internet::value::REQUEST_TYPE );
		headers.add( internet::variable::VAR_APP_TYPE, internet::value::APP_TYPE );
		INT status = req.sendRequest( headers );
		if ( status != HTTP_STATUS_OK ) {
			tstring result( TEXT( "" ) );
			{
				char buffer_mb[ MAX_LOADSTRING ];
				DWORD number_of_read;
				for ( auto i = req.beginRead( buffer_mb, MAX_LOADSTRING - 1, &number_of_read ); i != req.endRead(); i++ ) {
					buffer_mb[ number_of_read ] = 0;
					result.append( strToWstr( buffer_mb )->c_str() );
				}
			}
			Poco::JSON::Object obj;
			obj.set( "status", status );
			obj.set( "result", wstrToStr( result )->c_str() );

			std::unique_ptr<tstring> res = std::make_unique<tstring>();
			std::ostringstream oss;
			Poco::JSON::Stringifier::stringify( obj, oss );
			printf( oss.str().c_str() );
		}
		else {
			std::string result = "";
			{
				char buffer_mb[ MAX_LOADSTRING ];
				DWORD number_of_read;
				for ( auto i = req.beginRead( buffer_mb, MAX_LOADSTRING - 1, &number_of_read ); i != req.endRead(); i++ ) {
					buffer_mb[ number_of_read ] = 0;
					result.append( buffer_mb );
				}
			}
			Poco::JSON::Object obj;
			obj.set( "status", status );
			obj.set( "result", result );

			std::unique_ptr<tstring> res = std::make_unique<tstring>();
			std::ostringstream oss;
			Poco::JSON::Stringifier::stringify( obj, oss );
			printf( oss.str().c_str() );
		}
	} catch ( maschinen::exception::Exception& e ) {
		Poco::JSON::Object obj;
		obj.set( "status", 0 );
		obj.set( "result", e.what() );

		std::unique_ptr<tstring> res = std::make_unique<tstring>();
		std::ostringstream oss;
		Poco::JSON::Stringifier::stringify( obj, oss );
		printf( oss.str().c_str() );
	} catch ( std::exception& e ) {
		Poco::JSON::Object obj;
		obj.set( "status", 0 );
		obj.set( "result", e.what() );

		std::unique_ptr<tstring> res = std::make_unique<tstring>();
		std::ostringstream oss;
		Poco::JSON::Stringifier::stringify( obj, oss );
		printf( oss.str().c_str() );
	}
}

void getCloudReyclecleBin( LPCTSTR session, LPCTSTR akey, time_t waktu, LPCTSTR login_id ) {
	maschinen::wininet::HttpParams params;
	params.add( internet::variable::VAR_PHPSESSID_SEND, session, params.METHOD_GET );
	params.add( instruction::get_cloud_recycle_bin::INPUT_TIME, to_tstring( waktu ), params.METHOD_POST );
	params.add( instruction::get_cloud_recycle_bin::LOGIN_ID, login_id, params.METHOD_POST );
	maschinen::wininet::HttpRequest req;
	try {
		Setting setting;
		setting.read();
		req.open( TEXT( "" ), req.ACCESS_TYPE_PRE_CONFIG );
		req.connect( setting.getServerName()->c_str() );
		req.openRequest( setting.getUrl()->c_str(), req.VERSION_HTTP_11, NULL, NULL,
						 INTERNET_FLAG_PRAGMA_NOCACHE |
						 INTERNET_FLAG_NO_CACHE_WRITE |
						 INTERNET_FLAG_NO_COOKIES |
						 INTERNET_FLAG_NO_UI |
						 INTERNET_FLAG_NO_AUTO_REDIRECT |
						 INTERNET_FLAG_RELOAD, &params );
		req.setSendTimeOut( req.REQUEST_TIME_OUT_INFINITE );
		req.setReceiveTimeOut( req.REQUEST_TIME_OUT_INFINITE );
		maschinen::wininet::HttpHeaders headers;
		headers.add( TEXT( "Content-Type" ), TEXT( "application/x-www-form-urlencoded" ) );
		if ( akey != nullptr ) headers.add( internet::variable::VAR_APP_KEY, akey );
		headers.add( internet::variable::VAR_REQUEST, instruction::get_cloud_recycle_bin::REQUEST );
		headers.add( internet::variable::VAR_REQUEST_TYPE, internet::value::REQUEST_TYPE );
		headers.add( internet::variable::VAR_APP_TYPE, internet::value::APP_TYPE );
		INT status = req.sendRequest( headers );
		if ( status != HTTP_STATUS_OK ) {
			tstring result( TEXT( "" ) );
			{
				char buffer_mb[ MAX_LOADSTRING ];
				DWORD number_of_read;
				for ( auto i = req.beginRead( buffer_mb, MAX_LOADSTRING - 1, &number_of_read ); i != req.endRead(); i++ ) {
					buffer_mb[ number_of_read ] = 0;
					result.append( strToWstr( buffer_mb )->c_str() );
				}
			}
			Poco::JSON::Object obj;
			obj.set( "status", status );
			obj.set( "result", wstrToStr( result )->c_str() );

			std::unique_ptr<tstring> res = std::make_unique<tstring>();
			std::ostringstream oss;
			Poco::JSON::Stringifier::stringify( obj, oss );
			printf( oss.str().c_str() );
		}
		else {
			std::string result = "";
			{
				char buffer_mb[ MAX_LOADSTRING ];
				DWORD number_of_read;
				for ( auto i = req.beginRead( buffer_mb, MAX_LOADSTRING - 1, &number_of_read ); i != req.endRead(); i++ ) {
					buffer_mb[ number_of_read ] = 0;
					result.append( buffer_mb );
				}
			}
			Poco::JSON::Object obj;
			obj.set( "status", status );
			obj.set( "result", result );

			std::unique_ptr<tstring> res = std::make_unique<tstring>();
			std::ostringstream oss;
			Poco::JSON::Stringifier::stringify( obj, oss );
			printf( oss.str().c_str() );
		}
	} catch ( maschinen::exception::Exception& e ) {
		Poco::JSON::Object obj;
		obj.set( "status", 0 );
		obj.set( "result", e.what() );

		std::unique_ptr<tstring> res = std::make_unique<tstring>();
		std::ostringstream oss;
		Poco::JSON::Stringifier::stringify( obj, oss );
		printf( oss.str().c_str() );
	} catch ( std::exception& e ) {
		Poco::JSON::Object obj;
		obj.set( "status", 0 );
		obj.set( "result", e.what() );

		std::unique_ptr<tstring> res = std::make_unique<tstring>();
		std::ostringstream oss;
		Poco::JSON::Stringifier::stringify( obj, oss );
		printf( oss.str().c_str() );
	}
}

void GetUserListOnFilePrivilege( LPCTSTR session, LPCTSTR akey, LPCTSTR login_id, LPCTSTR file_id, LPCTSTR privilege_type ) {
	maschinen::wininet::HttpParams params;
	params.add( internet::variable::VAR_PHPSESSID_SEND, session, params.METHOD_GET );
	params.add( instruction::privilege_collector::INPUT_FILE_ID, file_id, params.METHOD_POST );
	params.add( instruction::privilege_collector::INPUT_PRIVILEGE_TYPE, privilege_type, params.METHOD_POST );
	params.add( instruction::privilege_collector::LOGIN_ID, login_id, params.METHOD_POST );
	maschinen::wininet::HttpRequest req;
	try {
		Setting setting;
		setting.read();
		req.open( TEXT( "" ), req.ACCESS_TYPE_PRE_CONFIG );
		req.connect( setting.getServerName()->c_str() );
		req.openRequest( setting.getUrl()->c_str(), req.VERSION_HTTP_11, NULL, NULL,
						 INTERNET_FLAG_PRAGMA_NOCACHE |
						 INTERNET_FLAG_NO_CACHE_WRITE |
						 INTERNET_FLAG_NO_COOKIES |
						 INTERNET_FLAG_NO_UI |
						 INTERNET_FLAG_NO_AUTO_REDIRECT |
						 INTERNET_FLAG_RELOAD, &params );
		req.setSendTimeOut( req.REQUEST_TIME_OUT_INFINITE );
		req.setReceiveTimeOut( req.REQUEST_TIME_OUT_INFINITE );
		maschinen::wininet::HttpHeaders headers;
		headers.add( TEXT( "Content-Type" ), TEXT( "application/x-www-form-urlencoded" ) );
		if ( akey != nullptr ) headers.add( internet::variable::VAR_APP_KEY, akey );
		headers.add( internet::variable::VAR_REQUEST, instruction::privilege_collector::REQUEST );
		headers.add( internet::variable::VAR_REQUEST_TYPE, internet::value::REQUEST_TYPE );
		headers.add( internet::variable::VAR_APP_TYPE, internet::value::APP_TYPE );
		INT status = req.sendRequest( headers );
		if ( status != HTTP_STATUS_OK ) {
			tstring result( TEXT( "" ) );
			{
				char buffer_mb[ MAX_LOADSTRING ];
				DWORD number_of_read;
				for ( auto i = req.beginRead( buffer_mb, MAX_LOADSTRING - 1, &number_of_read ); i != req.endRead(); i++ ) {
					buffer_mb[ number_of_read ] = 0;
					result.append( strToWstr( buffer_mb )->c_str() );
				}
			}
			Poco::JSON::Object obj;
			obj.set( "status", status );
			obj.set( "result", wstrToStr( result )->c_str() );

			std::unique_ptr<tstring> res = std::make_unique<tstring>();
			std::ostringstream oss;
			Poco::JSON::Stringifier::stringify( obj, oss );
			printf( oss.str().c_str() );
		}
		else {
			std::string result = "";
			{
				char buffer_mb[ MAX_LOADSTRING ];
				DWORD number_of_read;
				for ( auto i = req.beginRead( buffer_mb, MAX_LOADSTRING - 1, &number_of_read ); i != req.endRead(); i++ ) {
					buffer_mb[ number_of_read ] = 0;
					result.append( buffer_mb );
				}
			}
			Poco::JSON::Object obj;
			obj.set( "status", status );
			obj.set( "result", result );

			std::unique_ptr<tstring> res = std::make_unique<tstring>();
			std::ostringstream oss;
			Poco::JSON::Stringifier::stringify( obj, oss );
			printf( oss.str().c_str() );
		}
	} catch ( maschinen::exception::Exception& e ) {
		Poco::JSON::Object obj;
		obj.set( "status", 0 );
		obj.set( "result", e.what() );

		std::unique_ptr<tstring> res = std::make_unique<tstring>();
		std::ostringstream oss;
		Poco::JSON::Stringifier::stringify( obj, oss );
		printf( oss.str().c_str() );
	} catch ( std::exception& e ) {
		Poco::JSON::Object obj;
		obj.set( "status", 0 );
		obj.set( "result", e.what() );

		std::unique_ptr<tstring> res = std::make_unique<tstring>();
		std::ostringstream oss;
		Poco::JSON::Stringifier::stringify( obj, oss );
		printf( oss.str().c_str() );
	}
}

void getMetaFile( LPCTSTR session, LPCTSTR akey, QWORD parent_file_id, time_t waktu, LPCTSTR login_id ) {
	maschinen::wininet::HttpParams params;
	params.add( internet::variable::VAR_PHPSESSID_SEND, session, params.METHOD_GET );
	params.add( instruction::get_meta_file::INPUT_TIME, to_tstring( waktu ), params.METHOD_POST );
	params.add( instruction::get_meta_file::LOGIN_ID, login_id, params.METHOD_POST );
	if ( parent_file_id > 0 ) params.add( instruction::get_meta_file::INPUT_PARENT, to_tstring( parent_file_id ), params.METHOD_POST );
	maschinen::wininet::HttpRequest req;
	try {
		Setting setting;
		setting.read();
		req.open( TEXT( "" ), req.ACCESS_TYPE_PRE_CONFIG );
		req.connect( setting.getServerName()->c_str() );
		req.openRequest( setting.getUrl()->c_str(), req.VERSION_HTTP_11, NULL, NULL,
						 INTERNET_FLAG_PRAGMA_NOCACHE |
						 INTERNET_FLAG_NO_CACHE_WRITE |
						 INTERNET_FLAG_NO_COOKIES |
						 INTERNET_FLAG_NO_UI |
						 INTERNET_FLAG_NO_AUTO_REDIRECT |
						 INTERNET_FLAG_RELOAD, &params );
		req.setSendTimeOut( req.REQUEST_TIME_OUT_INFINITE );
		req.setReceiveTimeOut( req.REQUEST_TIME_OUT_INFINITE );
		maschinen::wininet::HttpHeaders headers;
		headers.add( TEXT( "Content-Type" ), TEXT( "application/x-www-form-urlencoded" ) );
		if ( akey != nullptr ) headers.add( internet::variable::VAR_APP_KEY, akey );
		headers.add( internet::variable::VAR_REQUEST, instruction::get_meta_file::REQUEST );
		headers.add( internet::variable::VAR_REQUEST_TYPE, internet::value::REQUEST_TYPE );
		headers.add( internet::variable::VAR_APP_TYPE, internet::value::APP_TYPE );
		INT status = req.sendRequest( headers );
		if ( status != HTTP_STATUS_OK ) {
			tstring result( TEXT( "" ) );
			{
				char buffer_mb[ MAX_LOADSTRING ];
				DWORD number_of_read;
				for ( auto i = req.beginRead( buffer_mb, MAX_LOADSTRING - 1, &number_of_read ); i != req.endRead(); i++ ) {
					buffer_mb[ number_of_read ] = 0;
					result.append( strToWstr( buffer_mb )->c_str() );
				}
			}
			Poco::JSON::Object obj;
			obj.set( "status", status );
			obj.set( "result", wstrToStr( result )->c_str() );

			std::unique_ptr<tstring> res = std::make_unique<tstring>();
			std::ostringstream oss;
			Poco::JSON::Stringifier::stringify( obj, oss );
			printf( oss.str().c_str() );
		}
		else {
			std::string result = "";
			{
				char buffer_mb[ MAX_LOADSTRING ];
				DWORD number_of_read;
				for ( auto i = req.beginRead( buffer_mb, MAX_LOADSTRING - 1, &number_of_read ); i != req.endRead(); i++ ) {
					buffer_mb[ number_of_read ] = 0;
					result.append( buffer_mb );
				}
			}
			Poco::JSON::Object obj;
			obj.set( "status", status );
			obj.set( "result", result );

			std::unique_ptr<tstring> res = std::make_unique<tstring>();
			std::ostringstream oss;
			Poco::JSON::Stringifier::stringify( obj, oss );
			printf( oss.str().c_str() );
		}
	} catch ( maschinen::exception::Exception& e ) {
		Poco::JSON::Object obj;
		obj.set( "status", 0 );
		obj.set( "result", e.what() );

		std::unique_ptr<tstring> res = std::make_unique<tstring>();
		std::ostringstream oss;
		Poco::JSON::Stringifier::stringify( obj, oss );
		printf( oss.str().c_str() );
	} catch ( std::exception& e ) {
		Poco::JSON::Object obj;
		obj.set( "status", 0 );
		obj.set( "result", e.what() );

		std::unique_ptr<tstring> res = std::make_unique<tstring>();
		std::ostringstream oss;
		Poco::JSON::Stringifier::stringify( obj, oss );
		printf( oss.str().c_str() );
	}
}

void requestPathParser( LPCTSTR session, LPCTSTR akey, LPCTSTR login_id, LPCTSTR path ) {
	maschinen::wininet::HttpParams params;
	params.add( internet::variable::VAR_PHPSESSID_SEND, session, params.METHOD_GET );
	params.add( instruction::path_parser::INPUT_PATH, path, params.METHOD_POST );
	params.add( instruction::path_parser::LOGIN_ID, login_id, params.METHOD_POST );
	maschinen::wininet::HttpRequest req;
	try {
		Setting setting;
		setting.read();
		req.open( TEXT( "" ), req.ACCESS_TYPE_PRE_CONFIG );
		req.connect( setting.getServerName()->c_str() );
		req.openRequest( setting.getUrl()->c_str(), req.VERSION_HTTP_11, NULL, NULL,
						 INTERNET_FLAG_PRAGMA_NOCACHE |
						 INTERNET_FLAG_NO_CACHE_WRITE |
						 INTERNET_FLAG_NO_COOKIES |
						 INTERNET_FLAG_NO_UI |
						 INTERNET_FLAG_NO_AUTO_REDIRECT |
						 INTERNET_FLAG_RELOAD, &params );
		req.setSendTimeOut( req.REQUEST_TIME_OUT_INFINITE );
		req.setReceiveTimeOut( req.REQUEST_TIME_OUT_INFINITE );
		maschinen::wininet::HttpHeaders headers;
		headers.add( TEXT( "Content-Type" ), TEXT( "application/x-www-form-urlencoded" ) );
		if ( akey != nullptr ) headers.add( internet::variable::VAR_APP_KEY, akey );
		headers.add( internet::variable::VAR_REQUEST, instruction::path_parser::REQUEST );
		headers.add( internet::variable::VAR_REQUEST_TYPE, internet::value::REQUEST_TYPE );
		headers.add( internet::variable::VAR_APP_TYPE, internet::value::APP_TYPE );
		INT status = req.sendRequest( headers );
		if ( status != HTTP_STATUS_OK ) {
			tstring result( TEXT( "" ) );
			{
				char buffer_mb[ MAX_LOADSTRING ];
				DWORD number_of_read;
				for ( auto i = req.beginRead( buffer_mb, MAX_LOADSTRING - 1, &number_of_read ); i != req.endRead(); i++ ) {
					buffer_mb[ number_of_read ] = 0;
					result.append( strToWstr( buffer_mb )->c_str() );
				}
			}
			Poco::JSON::Object obj;
			obj.set( "status", status );
			obj.set( "result", wstrToStr( result )->c_str() );

			std::unique_ptr<tstring> res = std::make_unique<tstring>();
			std::ostringstream oss;
			Poco::JSON::Stringifier::stringify( obj, oss );
			printf( oss.str().c_str() );
		}
		else {
			std::string result = "";
			{
				char buffer_mb[ MAX_LOADSTRING ];
				DWORD number_of_read;
				for ( auto i = req.beginRead( buffer_mb, MAX_LOADSTRING - 1, &number_of_read ); i != req.endRead(); i++ ) {
					buffer_mb[ number_of_read ] = 0;
					result.append( buffer_mb );
				}
			}
			Poco::JSON::Object obj;
			obj.set( "status", status );
			obj.set( "result", result );

			std::unique_ptr<tstring> res = std::make_unique<tstring>();
			std::ostringstream oss;
			Poco::JSON::Stringifier::stringify( obj, oss );
			printf( oss.str().c_str() );
		}
	} catch ( maschinen::exception::Exception& e ) {
		Poco::JSON::Object obj;
		obj.set( "status", 0 );
		obj.set( "result", e.what() );

		std::unique_ptr<tstring> res = std::make_unique<tstring>();
		std::ostringstream oss;
		Poco::JSON::Stringifier::stringify( obj, oss );
		printf( oss.str().c_str() );
	} catch ( std::exception& e ) {
		Poco::JSON::Object obj;
		obj.set( "status", 0 );
		obj.set( "result", e.what() );

		std::unique_ptr<tstring> res = std::make_unique<tstring>();
		std::ostringstream oss;
		Poco::JSON::Stringifier::stringify( obj, oss );
		printf( oss.str().c_str() );
	}
}

Poco::JSON::Object::Ptr extract( std::string& str ) {
	Poco::JSON::Object::Ptr json;
	Poco::JSON::Parser parser;
	parser.parse( str.c_str() );
	Poco::DynamicAny result = parser.result();
	if ( result.type() == typeid( Poco::JSON::Object::Ptr ) )
		json = result.extract<Poco::JSON::Object::Ptr>();
	return json;
}

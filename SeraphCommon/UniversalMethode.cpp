#include "stdafx.h"
#include "UniversalMethode.h"


SERAPHCOMMON_API void getDefaultHttpParam( maschinen::wininet::HttpParams* params, const tstring* php_session_id ) {
	if ( php_session_id != nullptr ) {
		if ( !php_session_id->empty() ) params->add( internet::variable::VAR_PHPSESSID_SEND, php_session_id, params->METHOD_GET );
	}
}

SERAPHCOMMON_API void getDefaultHttpParam( maschinen::wininet::HttpParams& params, const tstring* php_session_id ) {
	if ( php_session_id != nullptr ) {
		if ( !php_session_id->empty() ) params.add( internet::variable::VAR_PHPSESSID_SEND, php_session_id, params.METHOD_GET );
	}
}

SERAPHCOMMON_API INT sendRequest( LPCTSTR server_name, LPCTSTR url, maschinen::wininet::HttpRequest* comm, LPCTSTR request, LPCTSTR akey, maschinen::wininet::HttpParams* params ) {
	try {
		comm->open( L"", comm->ACCESS_TYPE_PRE_CONFIG );
		comm->connect( server_name );
		comm->openRequest( url, comm->VERSION_HTTP_11, NULL, NULL,
						   INTERNET_FLAG_PRAGMA_NOCACHE |
						   INTERNET_FLAG_NO_CACHE_WRITE |
						   INTERNET_FLAG_NO_COOKIES |
						   INTERNET_FLAG_NO_UI |
						   INTERNET_FLAG_NO_AUTO_REDIRECT |
						   INTERNET_FLAG_RELOAD, params );
		comm->setRequestTimeOut( comm->REQUEST_TIME_OUT_INFINITE );
		comm->setReceiveTimeOut( 950000 );
		maschinen::wininet::HttpHeaders headers;
		headers.add( TEXT( "Content-Type" ), TEXT( "application/x-www-form-urlencoded" ) );
		if ( akey != nullptr ) headers.add( internet::variable::VAR_APP_KEY, akey );
		headers.add( internet::variable::VAR_REQUEST, request );
		headers.add( internet::variable::VAR_REQUEST_TYPE, internet::value::REQUEST_TYPE );
		headers.add( internet::variable::VAR_APP_TYPE, internet::value::APP_TYPE );
		return comm->sendRequest( headers );
	} catch ( maschinen::exception::Exception& e ) {
		throw e;
	} catch ( std::exception& e ) {
		throw e;
	}
}

SERAPHCOMMON_API INT sendRequest( LPCTSTR server_name, LPCTSTR url, maschinen::wininet::HttpRequest* comm, LPCTSTR request, LPCTSTR akey, maschinen::wininet::HttpParams& params ) {
	try {
		comm->open( L"", comm->ACCESS_TYPE_PRE_CONFIG );
		comm->connect( server_name );
		comm->openRequest( url, comm->VERSION_HTTP_11, NULL, NULL,
						   INTERNET_FLAG_PRAGMA_NOCACHE |
						   INTERNET_FLAG_NO_CACHE_WRITE |
						   INTERNET_FLAG_NO_COOKIES |
						   INTERNET_FLAG_NO_UI |
						   INTERNET_FLAG_NO_AUTO_REDIRECT |
						   INTERNET_FLAG_RELOAD, &params );
		comm->setRequestTimeOut( comm->REQUEST_TIME_OUT_INFINITE );
		comm->setReceiveTimeOut( 950000 );
		maschinen::wininet::HttpHeaders headers;
		headers.add( TEXT( "Content-Type" ), TEXT( "application/x-www-form-urlencoded" ) );
		if ( akey != nullptr ) headers.add( internet::variable::VAR_APP_KEY, akey );
		headers.add( internet::variable::VAR_REQUEST, request );
		headers.add( internet::variable::VAR_REQUEST_TYPE, internet::value::REQUEST_TYPE );
		headers.add( internet::variable::VAR_APP_TYPE, internet::value::APP_TYPE );
		return comm->sendRequest( headers.serialize()->c_str() );
	} catch ( maschinen::exception::Exception& e ) {
		throw e;
	} catch ( std::exception& e ) {
		throw e;
	}
}

SERAPHCOMMON_API INT sendRequest( LPCTSTR server_name, LPCTSTR url, maschinen::wininet::HttpRequest* comm, const tstring& request, LPCTSTR akey, maschinen::wininet::HttpParams* params ) {
	try {
		comm->open( L"", comm->ACCESS_TYPE_PRE_CONFIG );
		comm->connect( server_name );
		comm->openRequest( url, comm->VERSION_HTTP_11, NULL, NULL,
						   INTERNET_FLAG_PRAGMA_NOCACHE |
						   INTERNET_FLAG_NO_CACHE_WRITE |
						   INTERNET_FLAG_NO_COOKIES |
						   INTERNET_FLAG_NO_UI |
						   INTERNET_FLAG_NO_AUTO_REDIRECT |
						   INTERNET_FLAG_RELOAD, params );
		comm->setRequestTimeOut( comm->REQUEST_TIME_OUT_INFINITE );
		comm->setReceiveTimeOut( 950000 );
		maschinen::wininet::HttpHeaders headers;
		headers.add( TEXT( "Content-Type" ), TEXT( "application/x-www-form-urlencoded" ) );
		if ( akey != nullptr ) headers.add( internet::variable::VAR_APP_KEY, akey );
		headers.add( internet::variable::VAR_REQUEST, request );
		headers.add( internet::variable::VAR_REQUEST_TYPE, internet::value::REQUEST_TYPE );
		headers.add( internet::variable::VAR_APP_TYPE, internet::value::APP_TYPE );
		return comm->sendRequest( headers.serialize()->c_str() );
	} catch ( maschinen::exception::Exception& e ) {
		throw e;
	} catch ( std::exception& e ) {
		throw e;
	}
}

SERAPHCOMMON_API std::unique_ptr<tstring> getDefaultAppDataPath( bool add_back_slash_in_the_end ) {
	std::unique_ptr<tstring> temp = std::make_unique<tstring>();
	PWSTR path = nullptr;
	HRESULT result = SHGetKnownFolderPath( guid::folder_id::DATA_DEFAULT_PATH, 0, NULL, &path );
	if ( result == S_OK ) {
		temp->append( path );
		temp->append( TEXT( "\\" ) );
		temp->append( string::file::name::DATA_DEFAULT.c_str() );

		DWORD attributes = GetFileAttributes( temp->c_str() );
		if ( attributes == INVALID_FILE_ATTRIBUTES ) {
			if ( CreateDirectoryW( temp->c_str(), NULL ) == 0 ) {
				DWORD error_code;
				TCHAR error[ MAX_LOADSTRING ];
				if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
					throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), error, error_code );
				else throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str() );
			}
		}
		else if ( attributes != INVALID_FILE_ATTRIBUTES && !( attributes & FILE_ATTRIBUTE_DIRECTORY ) ) {
			TCHAR error[ MAX_LOADSTRING ] = TEXT( "AppData already exists, but not a directory, please rename or delete the file, the path is " );
			wcscat_s( error, temp->c_str() );
			throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), error, dword::error_code::APP_DATA_NOT_DIRECTORY );
		}
		if ( add_back_slash_in_the_end ) temp->append( TEXT( "\\" ) );
	}
	else if ( result == E_FAIL )
		throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), TEXT( "AppData dose not have a path, fatal error" ), dword::error_code::APP_DATA_DOES_NOT_HAVE_PATH );
	else if ( result == E_INVALIDARG )
		throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), TEXT( "AppData path does not exists on the System" ), dword::error_code::APP_DATA_DOES_NOT_HAVE_PATH_ON_SYSTEM );
	else throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str() );
	return temp;
}

SERAPHCOMMON_API std::unique_ptr<tstring> fileSizeToStr( ULONGLONG file_size ) {
	std::unique_ptr<tstring> temp = std::make_unique<tstring>();
	if ( file_size < 1000 ) {
		temp->append( to_tstring( file_size ).c_str() );
		temp->append( TEXT( " B" ) );
	}
	else if ( file_size < 1000000 ) {
		double dsize = ( double ) file_size;
		temp->append( to_tstring( dsize / 1000 ).c_str() );
		temp->append( TEXT( " KB" ) );
	}
	else if ( file_size < 1000000000 ) {
		double dsize = ( double ) file_size;
		temp->append( to_tstring( dsize / 1000000 ).c_str() );
		temp->append( TEXT( " MB" ) );
	}
	else {
		double dsize = ( double ) file_size;
		temp->append( to_tstring( dsize / 1000000000 ).c_str() );
		temp->append( TEXT( " GB" ) );
	}
	return temp;
}

SERAPHCOMMON_API ULONGLONG fileSizeToProperSize( ULONGLONG file_size ) {
	if ( file_size < 1000 ) return file_size;
	else if ( file_size < 1000000 ) return file_size / 1000;
	else if ( file_size < 1000000000 ) return file_size / 1000000;
	else return file_size / 1000000000;
}

SERAPHCOMMON_API void systemTimetoWString( LPTSTR text, size_t size, LPCTSTR seperator, LPSYSTEMTIME st ) {
	if ( wcslen( seperator ) > 0 ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		_itow_s( st->wMilliseconds, buffer, 10 );
		wcscat_s( text, size, buffer );
		wcscat_s( text, size, seperator );

		_itow_s( st->wSecond, buffer, 10 );
		wcscat_s( text, size, buffer );
		wcscat_s( text, size, seperator );

		_itow_s( st->wMinute, buffer, 10 );
		wcscat_s( text, size, buffer );
		wcscat_s( text, size, seperator );

		_itow_s( st->wHour, buffer, 10 );
		wcscat_s( text, size, buffer );
		wcscat_s( text, size, seperator );

		_itow_s( st->wDay, buffer, 10 );
		wcscat_s( text, size, buffer );
		wcscat_s( text, size, seperator );

		_itow_s( st->wMonth, buffer, 10 );
		wcscat_s( text, size, buffer );
		wcscat_s( text, size, seperator );

		_itow_s( st->wYear, buffer, 10 );
		wcscat_s( text, size, buffer );
	}
	else {
		TCHAR buffer[ MAX_LOADSTRING ];
		_itow_s( st->wMilliseconds, buffer, 10 );
		wcscat_s( text, size, buffer );

		_itow_s( st->wSecond, buffer, 10 );
		wcscat_s( text, size, buffer );

		_itow_s( st->wMinute, buffer, 10 );
		wcscat_s( text, size, buffer );

		_itow_s( st->wHour, buffer, 10 );
		wcscat_s( text, size, buffer );

		_itow_s( st->wDay, buffer, 10 );
		wcscat_s( text, size, buffer );

		_itow_s( st->wMonth, buffer, 10 );
		wcscat_s( text, size, buffer );

		_itow_s( st->wYear, buffer, 10 );
		wcscat_s( text, size, buffer );
	}
}

SERAPHCOMMON_API std::unique_ptr<tstring> systemTimeTotstr( LPCTSTR seperator, LPSYSTEMTIME st ) {
	std::unique_ptr<tstring> result = std::make_unique<tstring>( L"" );
	TCHAR buffer[ MAX_LOADSTRING ];
	_itow_s( st->wMilliseconds, buffer, 10 );
	result->append( buffer );
	result->append( seperator );

	_itow_s( st->wSecond, buffer, 10 );
	result->append( buffer );
	result->append( seperator );

	_itow_s( st->wMinute, buffer, 10 );
	result->append( buffer );
	result->append( seperator );

	_itow_s( st->wHour, buffer, 10 );
	result->append( buffer );
	result->append( seperator );

	_itow_s( st->wDay, buffer, 10 );
	result->append( buffer );
	result->append( seperator );

	_itow_s( st->wMonth, buffer, 10 );
	result->append( buffer );
	result->append( seperator );

	_itow_s( st->wYear, buffer, 10 );
	result->append( buffer );
	return result;
}

SERAPHCOMMON_API std::unique_ptr<tstring> readHttpRequestToStr( maschinen::wininet::HttpRequest* request ) {
	std::unique_ptr<tstring> res = std::make_unique<tstring>();
	char buffer_mb[ MAX_LOADSTRING ];
	TCHAR buffer[ MAX_LOADSTRING ];
	DWORD number_of_byte_read = 0;
	size_t size;
	for ( auto i = request->beginRead( buffer_mb, MAX_LOADSTRING - 1, &number_of_byte_read ); i != request->endRead(); i++ ) {
		buffer_mb[ number_of_byte_read ] = 0;
		size = strlen( buffer_mb );
		mbstowcs_s( &size, buffer, buffer_mb, MAX_LOADSTRING );
		res->append( buffer );
	}
	return res;
}

SERAPHCOMMON_API std::unique_ptr<tstring> floorSeconds( LONGLONG seconds ) {
	//LONGLONG weeks = seconds / 604800;
	//LONGLONG days = ( seconds % 604800 ) / 86400;
	LONGLONG hours = ( ( seconds % 604800 ) % 86400 ) / 3600;
	LONGLONG minutes = ( ( ( seconds % 604800 ) % 86400 ) % 3600 ) / 60;
	seconds = ( ( ( seconds % 604800 ) % 86400 ) % 3600 ) % 60;
	std::unique_ptr<tstring> temp = std::make_unique<tstring>();
	temp->assign( TEXT( "" ) );
	if ( hours > 0 ) {
		temp->append( to_tstring( hours ) );
		temp->append( TEXT( " Hours " ) );
	}
	if( minutes > 0 ) {
		temp->append( to_tstring( minutes ) );
		temp->append( TEXT( " Minutes " ) );
	}
	if ( seconds > 0 ) {
		temp->append( to_tstring( seconds ) );
		temp->append( TEXT( " Seconds " ) );
	}
	return temp;
}
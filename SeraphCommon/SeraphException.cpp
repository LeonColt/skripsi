#include "stdafx.h"
#include "SeraphException.h"


bool SeraphException::exists() {
	DWORD result = GetFileAttributes( getPath()->c_str() );
	return result != INVALID_FILE_ATTRIBUTES && !( result & FILE_ATTRIBUTE_DIRECTORY );
}

void SeraphException::create() {
	maschinen::file::FileHandler fh( getPath()->c_str(), FILE_APPEND_DATA, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL );
	if ( !fh.isValid() ) {
		DWORD error_code;
		TCHAR error[ MAX_LOADSTRING ];
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) ) {
			OutputDebugStringW( error );
			OutputDebugStringW( TEXT( " error code : " ) );
			_ultow_s( error_code, error, 10 );
			OutputDebugStringW( error );
			OutputDebugStringW( TEXT( "\n" ) );
		}
		else OutputDebugStringW( TEXT( " an unknown error occured " ) );
	}
}

void SeraphException::write( LPCTSTR text ) {
	if ( !exists() ) create();
	maschinen::file::FileHandler fh( getPath()->c_str(), FILE_APPEND_DATA, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL );
	if ( !fh.isValid() ) {
		DWORD error_code;
		TCHAR error[ MAX_LOADSTRING ];
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw SeraphException( error, error_code );
		else SeraphException();
	}
	TCHAR buffer[ MAX_LOADSTRING ] = L"";
	addTime( buffer, MAX_LOADSTRING );
	addPID( buffer, MAX_LOADSTRING );
	wcscat_s( buffer, L" " );
	wcscat_s( buffer, text );
	wcscat_s( buffer, L"\r\n" );

	DWORD number_of_written = 0;

	if ( WriteFile( fh.get(), buffer, ( DWORD ) wcslen( buffer ) * sizeof( TCHAR ), &number_of_written, NULL ) == FALSE ) {
		DWORD error_code;
		TCHAR error[ MAX_LOADSTRING ];
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw SeraphException( error, error_code );
		else SeraphException();
	}
}

void SeraphException::write( LPCTSTR file_name, LPCTSTR text ) {
	this->name.assign( file_name );
	write( text );
}

void SeraphException::write( LPCTSTR file_name, SeraphException& e ) {
	this->name.assign( file_name );
	TCHAR buffer[ MAX_LOADSTRING ];
	TCHAR buffer2[ MAX_LOADSTRING ];
	e.getMessage( buffer, MAX_LOADSTRING );
	_ultow_s( e.getErrorCode(), buffer2, 10 );
	wcscat_s( buffer, TEXT( "  ; Error Code : " ) );
	wcscat_s( buffer, buffer2 );
	write( buffer );
}

void SeraphException::write( LPCTSTR file_name, maschinen::exception::Exception& e ) {
	this->name.assign( file_name );
	TCHAR buffer[ MAX_LOADSTRING ];
	TCHAR buffer2[ MAX_LOADSTRING ];
	e.getMessage( buffer, MAX_LOADSTRING );
	_ultow_s( e.getErrorCode(), buffer2, 10 );
	wcscat_s( buffer, TEXT( "  ; Error Code : " ) );
	wcscat_s( buffer, buffer2 );
	write( buffer );
}

void SeraphException::write( LPCTSTR file_name, std::exception& e ) {
	this->name.assign( file_name );
	TCHAR buffer[ MAX_LOADSTRING ];
	size_t size = strlen( e.what() );
	mbstowcs_s( &size, buffer, e.what(), MAX_LOADSTRING );
	write( buffer );
}

void SeraphException::addPID( LPWSTR str, size_t size ) {
	wcscat_s( str, size, L" PID: " );
	int pid = _getpid();
	TCHAR buffer[ MAX_LOADSTRING ];
	_itow_s( _getpid(), buffer, 10 );
	wcscat_s( str, size, buffer );
}

void SeraphException::addTime( LPWSTR str, size_t size ) {
	SYSTEMTIME local_time, system_time;
	GetSystemTime( &system_time );
	wcscat_s( str, size, L" UTC: " );

	systemTimetoWString( str, size, L",", &system_time );

	GetLocalTime( &local_time );
	wcscat_s( str, size, L" LOCAL: " );
	systemTimetoWString( str, size, L",", &local_time );
}

std::unique_ptr<tstring> SeraphException::getPath() {
	std::unique_ptr<tstring> path = std::make_unique<tstring>();
	path->append( getDefaultAppDataPath( true )->c_str() );
	path->append( name.c_str() );
	return path;
}

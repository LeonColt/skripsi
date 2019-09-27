#include "stdafx.h"
#include "Log.h"


Log::Log() {
	external_local_time = external_system_time = false;
	name.assign( L"log" );
}

void Log::setFileName( LPCTSTR name ) {
	this->name.assign( name );
}

bool Log::exists() {
	DWORD result = GetFileAttributes( getPath()->c_str() );
	return result != INVALID_FILE_ATTRIBUTES && !( result & FILE_ATTRIBUTE_DIRECTORY );
}

void Log::create() {
	maschinen::file::FileHandler fh( getPath()->c_str(), FILE_APPEND_DATA, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL );
	if ( !fh.isValid() ) {
		DWORD error_code;
		TCHAR error[ MAX_LOADSTRING ];
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), error, error_code );
		else SeraphException( string::file::name::ERROR_LOG_NAME.c_str() );
	}
}

void Log::write( LPCTSTR text ) {
	if ( !exists() ) create();
	maschinen::file::FileHandler fh( getPath()->c_str(), FILE_APPEND_DATA, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL );
	if ( !fh.isValid() ) {
		DWORD error_code;
		TCHAR error[ MAX_LOADSTRING ];
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), error, error_code );
		else SeraphException( string::file::name::ERROR_LOG_NAME.c_str() );
	}
	tstring buffer = L"";
	if ( setting->getLogTimeType() == 2 || setting->getLogTimeType() == 0 ) {
		if ( !external_system_time ) GetSystemTime( &system_time );
		buffer.append( L"UTC:" );
		buffer.append( systemTimeTotstr( L",", &system_time )->c_str() );
	}
	if ( setting->getLogTimeType() == 2 || setting->getLogTimeType() == 1 ) {
		if ( !external_local_time ) GetLocalTime( &local_time );
		buffer.append( L" LOCAL: " );
		buffer.append( systemTimeTotstr( L",", &local_time )->c_str() );
	}
	buffer.append( L" PID: " );
	buffer.append( to_tstring( _getpid() ).c_str() );

	buffer.append( L" " );
	buffer.append( text );
	buffer.append( L"\r\n" );

	DWORD number_of_written = 0;

	if ( WriteFile( fh.get(), buffer.c_str(), ( DWORD ) buffer.length() * sizeof( TCHAR ), &number_of_written, NULL ) == FALSE ) {
		DWORD error_code;
		TCHAR error[ MAX_LOADSTRING ];
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), error, error_code );
		else SeraphException( string::file::name::ERROR_LOG_NAME.c_str() );
	}
}

void Log::setLocalTime( LPSYSTEMTIME local_time ) {
	external_local_time = true;
	this->local_time.wDay = local_time->wDay;
	this->local_time.wDayOfWeek = local_time->wDayOfWeek;
	this->local_time.wHour = local_time->wHour;
	this->local_time.wMilliseconds = local_time->wMilliseconds;
	this->local_time.wMinute = local_time->wMinute;
	this->local_time.wMonth = local_time->wMonth;
	this->local_time.wSecond = local_time->wSecond;
	this->local_time.wYear = local_time->wYear;
}

void Log::setSystemTime( LPSYSTEMTIME system_time ) {
	external_system_time = true;
	this->system_time.wDay = system_time->wDay;
	this->system_time.wDayOfWeek = system_time->wDayOfWeek;
	this->system_time.wHour = system_time->wHour;
	this->system_time.wMilliseconds = system_time->wMilliseconds;
	this->system_time.wMinute = system_time->wMinute;
	this->system_time.wMonth = system_time->wMonth;
	this->system_time.wSecond = system_time->wSecond;
	this->system_time.wYear = system_time->wYear;
}

void Log::setSetting( Setting* setting ) {
	this->setting = setting;
}

std::unique_ptr<tstring> Log::getPath() {
	std::unique_ptr<tstring> path = std::make_unique<tstring>();
	path->append( getDefaultAppDataPath( true )->c_str() );
	path->append( name.c_str() );
	return path;
}

Log::~Log() {
}
#pragma once
#include <process.h>
#include "SeraphCommon.h"
#include "Exception.h"
#include "FileHandler.h"
#include "UniversalMethode.h"
class SERAPHCOMMON_API SeraphException : public maschinen::exception::Exception {
private:
	tstring name;
	void addTime( LPWSTR str, size_t size );
	void addPID( LPWSTR str, size_t size );
	bool exists();
	void create();
public:
	SeraphException() throw( ) : maschinen::exception::Exception() {
	}
	SeraphException( char const* const _Message, DWORD error_code ) throw( ) : maschinen::exception::Exception( _Message, error_code ) {
	}
	SeraphException( wchar_t const* const _Message, DWORD error_code ) throw ( ) : maschinen::exception::Exception( _Message, error_code ) {
	}
	SeraphException( LPCTSTR file_name ) throw( ) : maschinen::exception::Exception() {
		this->name.assign( file_name );
		TCHAR buffer[ MAX_LOADSTRING ];
		getMessage( buffer, MAX_LOADSTRING );
		write( buffer );
	}
	SeraphException( LPCTSTR file_name, char const* const _Message, DWORD error_code ) throw( ) : maschinen::exception::Exception( _Message, error_code ) {
		this->name.assign( file_name );
		TCHAR buffer[ MAX_LOADSTRING ];
		TCHAR buffer2[ MAX_LOADSTRING ];
		size_t size = strlen( _Message );
		mbstowcs_s( &size, buffer, _Message, MAX_LOADSTRING );
		_ultow_s( error_code, buffer2, 10 );
		wcscat_s( buffer, TEXT( "  ; Error Code : " ) );
		wcscat_s( buffer, buffer2 );
		write( buffer );
	}
	SeraphException( LPCTSTR file_name, wchar_t const* const _Message, DWORD error_code ) throw ( ) : maschinen::exception::Exception( _Message, error_code ) {
		this->name.assign( file_name );
		TCHAR buffer[ MAX_LOADSTRING ];
		TCHAR buffer2[ MAX_LOADSTRING ];
		wcscpy_s( buffer, _Message );
		_ultow_s( error_code, buffer2, 10 );
		wcscat_s( buffer, TEXT( "  ; Error Code : " ) );
		wcscat_s( buffer, buffer2 );
		write( buffer );
	}
	void write( LPCTSTR text );
	void write( LPCTSTR file_name, LPCTSTR text );
	void write( LPCTSTR file_name, SeraphException& e );
	void write( LPCTSTR file_name, maschinen::exception::Exception& e );
	void write( LPCTSTR file_name, std::exception& e );
	std::unique_ptr<tstring> getPath();
};



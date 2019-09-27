#include "stdafx.h"
#include "FileHandler.h"

maschinen::file::FileHandler::FileHandler() {}
maschinen::file::FileHandler::FileHandler( LPCTSTR file_name, DWORD desired_access,DWORD share_mode,LPSECURITY_ATTRIBUTES security_attribute,DWORD creation_disposition,DWORD flag_and_attributes, HANDLE template_file_handler ) {handle = CreateFile( file_name, desired_access, share_mode, security_attribute, creation_disposition, flag_and_attributes, template_file_handler );}
maschinen::file::FileHandler::FileHandler( tstring& file_name, DWORD desired_access,DWORD share_mode,LPSECURITY_ATTRIBUTES security_attribute,DWORD creation_disposition,DWORD flag_and_attributes,HANDLE template_file_handler ) {handle = CreateFile( file_name.c_str(), desired_access, share_mode, security_attribute, creation_disposition, flag_and_attributes, template_file_handler );}
maschinen::file::FileHandler::FileHandler( tstring* file_name, DWORD desired_access,DWORD share_mode,LPSECURITY_ATTRIBUTES security_attribute,DWORD creation_disposition,DWORD flag_and_attributes,HANDLE template_file_handler ) {handle = CreateFile( file_name->c_str(), desired_access, share_mode, security_attribute, creation_disposition, flag_and_attributes, template_file_handler );}

maschinen::file::FileHandler::FileHandler( LPCTSTR file_name,DWORD desired_access,DWORD share_mode,DWORD creation_disposition,DWORD flag_and_attributes ) {handle = CreateFile( file_name, desired_access, share_mode, NULL, creation_disposition, flag_and_attributes, NULL );}
maschinen::file::FileHandler::FileHandler( tstring& file_name,DWORD desired_access,DWORD share_mode,DWORD creation_disposition,DWORD flag_and_attributes ) {handle = CreateFile( file_name.c_str(), desired_access, share_mode, NULL, creation_disposition, flag_and_attributes, NULL );}
maschinen::file::FileHandler::FileHandler( tstring* file_name,DWORD desired_access, DWORD share_mode, DWORD creation_disposition,DWORD flag_and_attributes ) {handle = CreateFile( file_name->c_str(), desired_access, share_mode, NULL, creation_disposition, flag_and_attributes, NULL );}

void maschinen::file::FileHandler::init( LPCTSTR file_name, DWORD desired_access, DWORD share_mode, LPSECURITY_ATTRIBUTES security_attribute, DWORD creation_disposition, DWORD flag_and_attributes, HANDLE template_file_handler ) {close();handle = CreateFile( file_name, desired_access, share_mode, security_attribute, creation_disposition, flag_and_attributes, template_file_handler );}
void maschinen::file::FileHandler::init( tstring& file_name, DWORD desired_access,DWORD share_mode,LPSECURITY_ATTRIBUTES security_attribute,DWORD creation_disposition,DWORD flag_and_attributes,HANDLE template_file_handler ) {close();handle = CreateFile( file_name.c_str(), desired_access, share_mode, security_attribute, creation_disposition, flag_and_attributes, template_file_handler );}
void maschinen::file::FileHandler::init( tstring* file_name,DWORD desired_access,DWORD share_mode,LPSECURITY_ATTRIBUTES security_attribute,DWORD creation_disposition,DWORD flag_and_attributes,HANDLE template_file_handler ) {close();handle = CreateFile( file_name->c_str(), desired_access, share_mode, security_attribute, creation_disposition, flag_and_attributes, template_file_handler );}

void maschinen::file::FileHandler::init( LPCTSTR file_name,DWORD desired_access,DWORD share_mode,DWORD creation_disposition,DWORD flag_and_attributes ) {close();handle = CreateFile( file_name, desired_access, share_mode, NULL, creation_disposition, flag_and_attributes, NULL );}
void maschinen::file::FileHandler::init( tstring& file_name, DWORD desired_access, DWORD share_mode, DWORD creation_disposition, DWORD flag_and_attributes ) { close(); handle = CreateFile( file_name.c_str(), desired_access, share_mode, NULL, creation_disposition, flag_and_attributes, NULL ); }
void maschinen::file::FileHandler::init( tstring* file_name,DWORD desired_access,DWORD share_mode,DWORD creation_disposition,DWORD flag_and_attributes ) {close();handle = CreateFile( file_name->c_str(), desired_access, share_mode, NULL, creation_disposition, flag_and_attributes, NULL );}

maschinen::iterator::ReaderIterator& maschinen::file::FileHandler::beginRead( LPVOID buffer, DWORD buffer_size ) {
	reader.handle = handle;
	if ( ReadFile( handle, buffer, buffer_size, &reader.number_of_byte_read, NULL ) == TRUE ) {
		if ( reader.number_of_byte_read != 0 ) {
			reader.buffer = buffer;
			reader.buffer_size = buffer_size;
		}
	}
	else {
		TCHAR buffer[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
	return reader;
}

const maschinen::iterator::ReaderIterator& maschinen::file::FileHandler::endRead() const { return finish_reader; }

void maschinen::file::FileHandler::setPointer( LONGLONG pointer ) {
	LARGE_INTEGER lr;
	lr.QuadPart = pointer;
	setPointer( lr );
}
void maschinen::file::FileHandler::setPointer( LARGE_INTEGER pointer ) {
	if ( isValid() ) {
		if ( SetFilePointerEx( handle, pointer, NULL, FILE_BEGIN ) == FALSE ) {
			TCHAR buffer[ MAX_LOADSTRING ];
			DWORD error_code;
			if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
				throw maschinen::exception::Exception( buffer, error_code );
			else throw maschinen::exception::Exception();
		}
	}
}

void maschinen::file::FileHandler::write( const LPVOID buffer, const DWORD buffer_size, const LPDWORD number_of_byte_written, const LPOVERLAPPED lp_overlapped ) const {
	if ( !WriteFile( handle, buffer, buffer_size, number_of_byte_written, lp_overlapped ) ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) ) 
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
}

void maschinen::file::FileHandler::write( const LPVOID buffer, const DWORD buffer_size, const LPDWORD number_of_byte_written ) const {
	if ( !WriteFile( handle, buffer, buffer_size, number_of_byte_written, NULL ) ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
}

HANDLE maschinen::file::FileHandler::get() const {
	return handle;
}

void maschinen::file::FileHandler::set( HANDLE handler ) {
	close();
	this->handle = handler;
}

bool maschinen::file::FileHandler::isValid() const {return handle != INVALID_HANDLE_VALUE;}

LONGLONG maschinen::file::FileHandler::getSize() {
	LARGE_INTEGER size;
	size.QuadPart = 0;

	if( GetFileSizeEx(handle, &size) ) return size.QuadPart;
	else {
		TCHAR buffer[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
	return size.QuadPart;
}

LONGLONG maschinen::file::FileHandler::getSize( LPCTSTR path ) {
	LARGE_INTEGER size;
	size.QuadPart = 0;
	GET_FILEEX_INFO_LEVELS info_level = GetFileExInfoStandard;
	WIN32_FILE_ATTRIBUTE_DATA data;

	if ( GetFileAttributesEx( path, info_level, &data ) ) {
		size.LowPart = data.nFileSizeLow;
		size.HighPart = data.nFileSizeHigh;
		return size.QuadPart;
	}
	else {
		TCHAR buffer[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) ) {
			OutputDebugStringW( buffer );
			throw maschinen::exception::Exception( buffer, error_code );
		}
		else throw maschinen::exception::Exception();
	}
	return size.QuadPart;
}

LONGLONG maschinen::file::FileHandler::getSize( tstring& path ) {
	try {
		return getSize( path.c_str() );
	} catch( maschinen::exception::Exception& e ) {
		throw e;
	} catch( std::exception& e ) {
		throw e;
	}
}
LONGLONG maschinen::file::FileHandler::getSize( tstring* path ){
	try {
		return getSize( path->c_str() );
	} catch ( maschinen::exception::Exception& e ) {
		throw e;
	} catch ( std::exception& e ) {
		throw e;
	}
}

std::unique_ptr<LARGE_INTEGER> maschinen::file::FileHandler::getSizeEx() {
	std::unique_ptr<LARGE_INTEGER> temp = std::make_unique<LARGE_INTEGER>();
	if ( isValid() ) GetFileSizeEx( handle, temp.get() );
	return temp;
}

std::unique_ptr<tstring> maschinen::file::FileHandler::getOwner() {
	PSECURITY_DESCRIPTOR psd = NULL;
	PSID psid = NULL;
	if ( GetSecurityInfo( handle, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, &psid, NULL, NULL, NULL, &psd ) != ERROR_SUCCESS ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
	LPTSTR acctname = NULL, domain_name = NULL;
	SID_NAME_USE sid_name_use = SidTypeUnknown;
	DWORD dw_acctname = 1, dw_domain_name = 1;
	if ( LookupAccountSid( NULL, psid, acctname, ( LPDWORD ) &dw_acctname, domain_name, ( LPDWORD ) &dw_domain_name, &sid_name_use ) != ERROR_SUCCESS ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
	acctname = ( LPTSTR ) GlobalAlloc( GMEM_FIXED, dw_acctname );
	if ( acctname == NULL ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
	domain_name = ( LPTSTR ) GlobalAlloc( GMEM_FIXED, dw_domain_name );
	if ( domain_name == NULL ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) ) {
			if ( acctname != NULL ) GlobalFree( acctname );
			throw maschinen::exception::Exception( error, error_code );
		}
		else {
			if ( acctname != NULL ) GlobalFree( acctname ); throw maschinen::exception::Exception();
		}
	}
	std::unique_ptr<tstring> temp = std::make_unique<tstring>();
	if ( LookupAccountSid( NULL, psid, acctname, ( LPDWORD ) &dw_acctname, domain_name, ( LPDWORD ) &dw_domain_name, &sid_name_use ) == TRUE )
		temp->assign( acctname );
	else {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
	if ( acctname != NULL ) GlobalFree( acctname );
	if ( domain_name != NULL ) GlobalFree( domain_name );
	return temp;
}

std::unique_ptr<FILETIME> maschinen::file::FileHandler::getCreatedTime() const{
	std::unique_ptr<FILETIME> time;
	GET_FILEEX_INFO_LEVELS level = GetFileExInfoStandard;
	WIN32_FILE_ATTRIBUTE_DATA data;
	TCHAR path[ MAX_LOADSTRING ];
	GetFinalPathNameByHandle( handle, path, MAX_LOADSTRING, NULL );
	if( GetFileAttributesEx(path, level, &data) ) {
		time = std::make_unique<FILETIME>();
		time->dwLowDateTime = data.ftCreationTime.dwLowDateTime;
		time->dwHighDateTime = data.ftCreationTime.dwHighDateTime;
	}
	else {
		TCHAR buffer[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
	return time;
}

std::unique_ptr<FILETIME> maschinen::file::FileHandler::getCreatedTime( LPCTSTR path ) const {
	std::unique_ptr<FILETIME> time;
	GET_FILEEX_INFO_LEVELS level = GetFileExInfoStandard;
	WIN32_FILE_ATTRIBUTE_DATA data;
	if( GetFileAttributesEx(path, level, &data) ) {
		time = std::make_unique<FILETIME>();
		time->dwLowDateTime = data.ftLastAccessTime.dwLowDateTime;
		time->dwHighDateTime = data.ftLastAccessTime.dwHighDateTime;
	}
	else {
		TCHAR buffer[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
	return time;
}

std::unique_ptr<FILETIME> maschinen::file::FileHandler::getLastAccessTime() const{
	std::unique_ptr<FILETIME> time;
	GET_FILEEX_INFO_LEVELS level = GetFileExInfoStandard;
	WIN32_FILE_ATTRIBUTE_DATA data;
	TCHAR path[ MAX_LOADSTRING ];
	GetFinalPathNameByHandle( handle, path, MAX_LOADSTRING, NULL );
	if ( GetFileAttributesEx( path, level, &data ) ) {
		time = std::make_unique<FILETIME>();
		time->dwLowDateTime = data.ftLastAccessTime.dwLowDateTime;
		time->dwHighDateTime = data.ftLastAccessTime.dwHighDateTime;
	}
	else {
		TCHAR buffer[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
	return time;
}
std::unique_ptr<FILETIME> maschinen::file::FileHandler::getLastAccessTime( LPCTSTR path ) const {
	std::unique_ptr<FILETIME> time;
	GET_FILEEX_INFO_LEVELS level = GetFileExInfoStandard;
	WIN32_FILE_ATTRIBUTE_DATA data;
	if ( GetFileAttributesEx( path, level, &data ) ) {
		time = std::make_unique<FILETIME>();
		time->dwLowDateTime = data.ftLastAccessTime.dwLowDateTime;
		time->dwHighDateTime = data.ftLastAccessTime.dwHighDateTime;
	}
	else {
		TCHAR buffer[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
	return time;
}
std::unique_ptr<FILETIME> maschinen::file::FileHandler::getLastWriteTime() const{
	std::unique_ptr<FILETIME> time;
	GET_FILEEX_INFO_LEVELS level = GetFileExInfoStandard;
	WIN32_FILE_ATTRIBUTE_DATA data;
	TCHAR path[ MAX_LOADSTRING ];
	GetFinalPathNameByHandle( handle, path, MAX_LOADSTRING, NULL );
	if ( GetFileAttributesEx( path, level, &data ) ) {
		time = std::make_unique<FILETIME>();
		time->dwLowDateTime = data.ftLastWriteTime.dwLowDateTime;
		time->dwHighDateTime = data.ftLastWriteTime.dwHighDateTime;
	}
	else {
		TCHAR buffer[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) ) 
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
	return time;
}

std::unique_ptr<FILETIME> maschinen::file::FileHandler::getLastWriteTime( LPCTSTR path ) const {
	std::unique_ptr<FILETIME> time;
	GET_FILEEX_INFO_LEVELS level = GetFileExInfoStandard;
	WIN32_FILE_ATTRIBUTE_DATA data;
	if ( GetFileAttributesEx( path, level, &data ) ) {
		time = std::make_unique<FILETIME>();
		time->dwLowDateTime = data.ftLastWriteTime.dwLowDateTime;
		time->dwHighDateTime = data.ftLastWriteTime.dwHighDateTime;
	}
	else {
		TCHAR buffer[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
	return time;
}

void maschinen::file::FileHandler::close() {
	if ( handle != INVALID_HANDLE_VALUE ) CloseHandle( handle );
	handle = INVALID_HANDLE_VALUE;
}

maschinen::file::FileHandler::~FileHandler() {
	close();
}
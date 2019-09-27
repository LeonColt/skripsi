#include "stdafx.h"
#include "FileHandleAsynchronous.h"


maschinen::file::FileHandleAsynchronous::FileHandleAsynchronous() : maschinen::file::FileHandleBase() {}

HANDLE maschinen::file::FileHandleAsynchronous::createAlways( LPCTSTR file_name ) {
	close();
	attrs_and_flags |= FILE_FLAG_OVERLAPPED;
	handle = CreateFile( file_name, access, share_mode, ( security_attributes ) ? ( security_attributes->getLP() ) : NULL, CREATE_ALWAYS, attrs_and_flags, NULL );
	if ( handle == INVALID_HANDLE_VALUE ) {
		TCHAR buffer [ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
	return handle;
}
HANDLE maschinen::file::FileHandleAsynchronous::createNew( LPCTSTR file_name ) {
	close();
	attrs_and_flags |= FILE_FLAG_OVERLAPPED;
	handle = CreateFile( file_name, access, share_mode, ( security_attributes ) ? ( security_attributes->getLP() ) : NULL, CREATE_NEW, attrs_and_flags, NULL );
	if ( handle == INVALID_HANDLE_VALUE ) {
		TCHAR buffer [ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
	return handle;
}
HANDLE maschinen::file::FileHandleAsynchronous::openAlways( LPCTSTR file_name ) {
	close();
	attrs_and_flags |= FILE_FLAG_OVERLAPPED;
	handle = CreateFile( file_name, access, share_mode, ( security_attributes ) ? ( security_attributes->getLP() ) : NULL, OPEN_ALWAYS, attrs_and_flags, NULL );
	if ( handle == INVALID_HANDLE_VALUE ) {
		TCHAR buffer [ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
	return handle;
}
HANDLE maschinen::file::FileHandleAsynchronous::openExisting( LPCTSTR file_name ) {
	close();
	attrs_and_flags |= FILE_FLAG_OVERLAPPED;
	handle = CreateFile( file_name, access, share_mode, ( security_attributes ) ? ( security_attributes->getLP() ) : NULL, OPEN_EXISTING, attrs_and_flags, NULL );
	if ( handle == INVALID_HANDLE_VALUE ) {
		TCHAR buffer [ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
	return handle;
}
HANDLE maschinen::file::FileHandleAsynchronous::truncateExisting( LPCTSTR file_name ) {
	close();
	attrs_and_flags |= FILE_FLAG_OVERLAPPED;
	handle = CreateFile( file_name, access, share_mode, ( security_attributes ) ? ( security_attributes->getLP() ) : NULL, TRUNCATE_EXISTING, attrs_and_flags, NULL );
	if ( handle == INVALID_HANDLE_VALUE ) {
		TCHAR buffer [ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
	return handle;
}

void maschinen::file::FileHandleAsynchronous::setHandle( HANDLE handle ) {
	close();
	this->handle = handle;
}

maschinen::iterator::ReaderIteratorAsynchronous& maschinen::file::FileHandleAsynchronous::beginRead( LPVOID buffer, DWORD buffer_size ) {
	riaf.reset();
	ria.reset();
	riaf = std::make_unique<maschinen::iterator::ReaderIteratorAsynchronous>();
	ria = std::make_unique<maschinen::iterator::ReaderIteratorAsynchronous>();
	HANDLE event = CreateEvent( NULL, TRUE, FALSE, NULL );
	if ( !event ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
	ria->handle = handle;
	ria->buffer = buffer;
	ria->buffer_size = buffer_size;
	ria->overlapped.hEvent = event;
	++( *ria.get() );
	return *ria.get();
}
const maschinen::iterator::ReaderIteratorAsynchronous& maschinen::file::FileHandleAsynchronous::endRead() const noexcept {return *riaf.get();}

maschinen::file::FileHandleAsynchronous::~FileHandleAsynchronous() {}

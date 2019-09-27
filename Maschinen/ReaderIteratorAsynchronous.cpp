#include "stdafx.h"
#include "ReaderIteratorAsynchronous.h"


maschinen::iterator::ReaderIteratorAsynchronous::ReaderIteratorAsynchronous() {
	ZeroMemory( &overlapped, sizeof( OVERLAPPED ) );
	handle = INVALID_HANDLE_VALUE;
	buffer = NULL;
	buffer_size = 0;
	pointer.QuadPart = 0;
	read_state = ERROR_SUCCESS;
	overlapped_state = ERROR_SUCCESS;
	number_of_byte_read = 0;
}

bool maschinen::iterator::ReaderIteratorAsynchronous::isIncomplete() const noexcept { return overlapped_state == ERROR_IO_INCOMPLETE; }
bool maschinen::iterator::ReaderIteratorAsynchronous::isComplete() const noexcept { return overlapped_state == ERROR_SUCCESS; }
bool maschinen::iterator::ReaderIteratorAsynchronous::isEndOfFile() const noexcept { return overlapped_state == ERROR_HANDLE_EOF; }
DWORD maschinen::iterator::ReaderIteratorAsynchronous::getResultElse() const noexcept { return overlapped_state; }
DWORD maschinen::iterator::ReaderIteratorAsynchronous::getNumberOfByteRead() const noexcept { return number_of_byte_read; }

maschinen::iterator::ReaderIteratorAsynchronous& maschinen::iterator::ReaderIteratorAsynchronous::operator++() {
	if ( read_state == ERROR_SUCCESS ) {
		SetLastError( ERROR_SUCCESS );
		read_state = ReadFile( handle, buffer, buffer_size, NULL, &overlapped );
		if ( read_state == FALSE ) {
			read_state = GetLastError();
			if ( read_state != ERROR_IO_PENDING && read_state != ERROR_BROKEN_PIPE ) {
				ResetEvent( overlapped.hEvent );
				CloseHandle( overlapped.hEvent );
				TCHAR error[ MAX_LOADSTRING ];
				DWORD error_code;
				if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
					throw maschinen::exception::Exception( error, error_code );
				else throw maschinen::exception::Exception();
			}
		}
		else {
			overlapped_state = read_state = ERROR_SUCCESS;
			pointer.QuadPart += ( LONGLONG ) number_of_byte_read;
			overlapped.Offset = pointer.LowPart;
			overlapped.OffsetHigh = pointer.HighPart;
			return *this;
		}
	}
	if ( read_state != ERROR_IO_PENDING ) {
		overlapped_state = read_state = ERROR_SUCCESS;
		OutputDebugStringW( L" tidak sama dengan io pending baca ulang\n" );
		return *this;
	}
	overlapped_state = GetOverlappedResult( handle, &overlapped, &number_of_byte_read, FALSE );
	if ( overlapped_state == FALSE ) overlapped_state = GetLastError(); else overlapped_state = ERROR_SUCCESS;

	if ( overlapped_state == ERROR_SUCCESS ) {
		overlapped_state = read_state = ERROR_SUCCESS;
		pointer.QuadPart += ( LONGLONG ) number_of_byte_read;
		overlapped.Offset = pointer.LowPart;
		overlapped.OffsetHigh = pointer.HighPart;
	}
	else if ( overlapped_state == ERROR_HANDLE_EOF ) {
		handle = INVALID_HANDLE_VALUE;
		ResetEvent( overlapped.hEvent );
		CloseHandle( overlapped.hEvent );
	}
	else if ( overlapped_state != ERROR_IO_INCOMPLETE ) {
		ResetEvent( overlapped.hEvent );
		CloseHandle( overlapped.hEvent );
		TCHAR error [ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
	return *this;
}
maschinen::iterator::ReaderIteratorAsynchronous maschinen::iterator::ReaderIteratorAsynchronous::operator++( int ) {
	ReaderIteratorAsynchronous tmp = *this;
	++*this;
	return tmp;
}

bool maschinen::iterator::ReaderIteratorAsynchronous::operator!=( const ReaderIteratorAsynchronous& that ) const noexcept {return handle != that.handle;}
bool maschinen::iterator::ReaderIteratorAsynchronous::operator==( const ReaderIteratorAsynchronous& that ) const noexcept {return handle == that.handle;}

maschinen::iterator::ReaderIteratorAsynchronous::~ReaderIteratorAsynchronous() {
}

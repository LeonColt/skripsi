#include "stdafx.h"
#include "AnonymoudPipe.h"


maschinen::pipe::AnonymoudPipe::AnonymoudPipe( LPCTSTR command_line ) {
	in_write = NULL;
	in_read = NULL;
	out_write = NULL;
	in_write = NULL;
	handle = NULL;

	SECURITY_ATTRIBUTES sa_attr;

	sa_attr.nLength = sizeof( SECURITY_ATTRIBUTES );
	sa_attr.bInheritHandle = TRUE;
	sa_attr.lpSecurityDescriptor = NULL;

	if ( !CreatePipe( &out_read, &out_write, &sa_attr, 0 ) ) {
		TCHAR buffer [ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}

	if ( !SetHandleInformation( out_read, HANDLE_FLAG_INHERIT, 0 ) ) {
		TCHAR buffer [ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
	if ( !CreatePipe( &in_read, &in_write, &sa_attr, 0 ) ) {
		TCHAR buffer [ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
	if ( !SetHandleInformation( in_write, HANDLE_FLAG_INHERIT, 0 ) ) {
		TCHAR buffer [ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
	createChildProcess( command_line );
}

void maschinen::pipe::AnonymoudPipe::createChildProcess( LPCTSTR command_line ) {
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory( &pi, sizeof( PROCESS_INFORMATION ) );
	ZeroMemory( &si, sizeof( STARTUPINFO ) );
	si.cb = sizeof( STARTUPINFO );
	si.hStdError = out_write;
	si.hStdOutput = out_write;
	si.hStdInput = in_read;
	si.dwFlags |= STARTF_USESTDHANDLES;
	si.dwFlags |= STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	const size_t size = tcslen( command_line ) + 1;
	std::unique_ptr<TCHAR []> scmd = std::make_unique<TCHAR []>( size );
	wcscpy_s( scmd.get(), size, command_line );

	if ( !CreateProcess( NULL,
		 scmd.get(),
		 NULL,
		 NULL,
		 TRUE,
		 0,
		 NULL,
		 NULL,
		 &si,
		 &pi
		 ) ) {
		TCHAR buffer [ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
	handle = pi.hProcess;
	CloseHandle( pi.hThread );
}

void maschinen::pipe::AnonymoudPipe::write( LPVOID buffer, DWORD size, DWORD& number_of_byte_written ) {
	if ( WriteFile( in_write, buffer, size, &number_of_byte_written, NULL ) == FALSE ) {
		TCHAR error [ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
}

maschinen::iterator::ReaderIterator& maschinen::pipe::AnonymoudPipe::beginRead( LPVOID buffer, DWORD buffer_size ){
	reader.handle = out_read;
	reader.buffer = buffer;
	reader.buffer_size = buffer_size;
	++reader;
	return reader;
}
const maschinen::iterator::ReaderIterator& maschinen::pipe::AnonymoudPipe::endRead() const noexcept{return freader;}

void maschinen::pipe::AnonymoudPipe::closeRead() {
	if ( in_read != NULL && in_read != INVALID_HANDLE_VALUE ) {
		CloseHandle( in_read );
		in_read = INVALID_HANDLE_VALUE;
	}
	if ( out_read != NULL && out_read != INVALID_HANDLE_VALUE ) {
		CloseHandle( out_read );
		out_read = INVALID_HANDLE_VALUE;
	}
}

void maschinen::pipe::AnonymoudPipe::closeWrite() {
	if ( in_write != NULL && in_write != INVALID_HANDLE_VALUE ) {
		CloseHandle( in_write );
		in_write = INVALID_HANDLE_VALUE;
	}
	if ( out_write != NULL && out_write != INVALID_HANDLE_VALUE ) {
		CloseHandle( out_write );
		out_write = INVALID_HANDLE_VALUE;
	}
}

void maschinen::pipe::AnonymoudPipe::terminate( UINT exit_code ) {
	closeWrite();
	closeRead();
	if ( handle != NULL && handle != INVALID_HANDLE_VALUE ) {
		TerminateProcess( handle, exit_code );
		handle = INVALID_HANDLE_VALUE;
	}
}

HANDLE maschinen::pipe::AnonymoudPipe::getRead() {
	return out_read;
}

HANDLE maschinen::pipe::AnonymoudPipe::getWrite() {
	return in_write;
}

HANDLE maschinen::pipe::AnonymoudPipe::get() {
	return handle;
}

maschinen::pipe::AnonymoudPipe::~AnonymoudPipe() {
	terminate( 1 );
}

#include "stdafx.h"
#include "RedirectIOChildProcess.h"


maschinen::pipe::RedirectIOChildProcess::RedirectIOChildProcess( LPCTSTR pipe_name ) {
	this->pipe_name.assign( TEXT( "\\\\.\\pipe\\" ) );
	this->pipe_name.append( pipe_name );

	out_read = INVALID_HANDLE_VALUE;
	out_write = INVALID_HANDLE_VALUE;
	in_read = INVALID_HANDLE_VALUE;
	in_write = INVALID_HANDLE_VALUE;
	error_write = INVALID_HANDLE_VALUE;
	ZeroMemory( &sa, sizeof( SECURITY_ATTRIBUTES ) );
	sa.nLength = sizeof( SECURITY_ATTRIBUTES );
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = nullptr;

	HANDLE out_read_tmp = CreateNamedPipe(
		this->pipe_name.c_str(),
		PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED,
		0,
		PIPE_UNLIMITED_INSTANCES,
		4096,
		4096,
		0,
		&sa
		);
	if ( out_read_tmp == INVALID_HANDLE_VALUE ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}

	out_write = CreateFile( this->pipe_name.c_str(), FILE_WRITE_DATA, 0, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL );
	if ( out_write == INVALID_HANDLE_VALUE ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
	DuplicateHandle( GetCurrentProcess(), out_write, GetCurrentProcess(), &error_write, 0, TRUE, DUPLICATE_SAME_ACCESS );
	if ( error_write == INVALID_HANDLE_VALUE ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
	DuplicateHandle( GetCurrentProcess(), out, GetCurrentProcess(), &out_read, 0, FALSE, DUPLICATE_SAME_ACCESS );
	if ( out_read == INVALID_HANDLE_VALUE ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}

	CloseHandle( out );

	HANDLE in_read_tmp = CreateNamedPipe(
		this->pipe_name.c_str(),
		PIPE_ACCESS_OUTBOUND,
		0,
		PIPE_UNLIMITED_INSTANCES,
		4096,
		4096,
		0,
		&sa
		);
	if ( in_read_tmp == INVALID_HANDLE_VALUE ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
	in_write = CreateFile( this->pipe_name.c_str(), FILE_READ_DATA, 0, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL );
	if ( in_write == INVALID_HANDLE_VALUE ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
	DuplicateHandle( GetCurrentProcess(), in, GetCurrentProcess(), &in_read, 0, FALSE, DUPLICATE_SAME_ACCESS );
	if ( in_read == INVALID_HANDLE_VALUE ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
	CloseHandle( in );
}

void maschinen::pipe::RedirectIOChildProcess::createProcess( LPTSTR command_line ) {
	PROCESS_INFORMATION pi;
	ZeroMemory( &pi, sizeof( SECURITY_INFORMATION ) );
	STARTUPINFO si;
	ZeroMemory( &si, sizeof( STARTUPINFO ) );
	si.cb = sizeof( STARTUPINFO );
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdOutput = out_write;
	si.hStdInput = in_read;
	si.hStdError = out_write;

	CreateProcess( NULL, command_line, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi );
	CloseHandle( pi.hThread );
	handle = pi.hProcess;
}

void maschinen::pipe::RedirectIOChildProcess::closeRead() {
	if ( in_read != NULL && in_read != INVALID_HANDLE_VALUE ) {
		CloseHandle( in_read );
		in_read = INVALID_HANDLE_VALUE;
	}
	if ( out_read != NULL && out_read != INVALID_HANDLE_VALUE ) {
		CloseHandle( out_read );
		out_read = INVALID_HANDLE_VALUE;
	}
}
void maschinen::pipe::RedirectIOChildProcess::closeWrite() {
	if ( in_write != NULL && in_write != INVALID_HANDLE_VALUE ) {
		CloseHandle( in_write );
		in_write = INVALID_HANDLE_VALUE;
	}
	if ( out_write != NULL && out_write != INVALID_HANDLE_VALUE ) {
		CloseHandle( out_write );
		out_write = INVALID_HANDLE_VALUE;
	}
	if ( error_write != NULL && error_write != INVALID_HANDLE_VALUE ) {
		CloseHandle( error_write );
		out_write = INVALID_HANDLE_VALUE;
	}
}

HANDLE maschinen::pipe::RedirectIOChildProcess::getOutRead() {
	return out_read;
}

HANDLE maschinen::pipe::RedirectIOChildProcess::getOutWrite() {
	return out_write;
}

void maschinen::pipe::RedirectIOChildProcess::terminate( UINT exit_code ) {
	closeWrite();
	closeRead();
	if ( handle != NULL && handle != INVALID_HANDLE_VALUE ) {
		TerminateProcess( handle, exit_code );
		handle = INVALID_HANDLE_VALUE;
	}
}

maschinen::pipe::RedirectIOChildProcess::~RedirectIOChildProcess() {
	//terminate(0);
	//if ( in != INVALID_HANDLE_VALUE ) CloseHandle( in );
	//if ( out != INVALID_HANDLE_VALUE ) CloseHandle( out );
}

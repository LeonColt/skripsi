#include "stdafx.h"
#include "NamedPipe.h"


maschinen::pipe::NamedPipe::NamedPipe() {
	handle = INVALID_HANDLE_VALUE;
}

maschinen::pipe::NamedPipe::NamedPipe( LPCTSTR name, DWORD open_mode, DWORD pipe_mode, DWORD max_instance, DWORD out_buffer_size, DWORD in_buffer_size, DWORD default_time_out ) {
	handle = CreateNamedPipe( name, open_mode, pipe_mode, max_instance, out_buffer_size, in_buffer_size, default_time_out, NULL );
}
maschinen::pipe::NamedPipe::NamedPipe( tstring& name ) {

}
maschinen::pipe::NamedPipe::NamedPipe( tstring* name ) {

}
void maschinen::pipe::NamedPipe::connect() {
	if ( isValid() )
		ConnectNamedPipe( handle, NULL );
}
void maschinen::pipe::NamedPipe::close() {
	if ( handle != INVALID_HANDLE_VALUE ) CloseHandle( handle );
}

bool maschinen::pipe::NamedPipe::isValid() const noexcept {
	return handle != INVALID_HANDLE_VALUE;
}

HANDLE maschinen::pipe::NamedPipe::get() const noexcept {
	return handle;
}

maschinen::pipe::NamedPipe::~NamedPipe() {
	close();
}

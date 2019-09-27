#include "stdafx.h"
#include "NamedPipeAsynchronous.h"


maschinen::pipe::NamedPipeAsynchronous::NamedPipeAsynchronous() {
}

HANDLE maschinen::pipe::NamedPipeAsynchronous::create() {
	open_mode |= FILE_FLAG_OVERLAPPED;
	HANDLE th = CreateNamedPipe( getName(), open_mode, mode, max_instance, out_buffer_size, in_buffer_size, default_time_out, ( getSecurityAttributes() ) ? getSecurityAttributes()->getLP() : NULL );
	set( th );
	return th;
}

maschinen::pipe::NamedPipeAsynchronous::~NamedPipeAsynchronous() {
}

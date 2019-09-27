#include "stdafx.h"
#include "NamedPipeBase.h"


maschinen::pipe::NamedPipeBase::NamedPipeBase() : INSTANCE_UNLIMITED(PIPE_UNLIMITED_INSTANCES) {
	handle = INVALID_HANDLE_VALUE;
	max_instance = 1;
	in_buffer_size = MAX_LOADSTRING;
	out_buffer_size = MAX_LOADSTRING;
	default_time_out = 0;
	open_mode = 0;
	mode = 0;
	security_attributes = nullptr;
}

void maschinen::pipe::NamedPipeBase::set( HANDLE handle ) noexcept {this->handle = handle;}
HANDLE maschinen::pipe::NamedPipeBase::get() const noexcept {return handle;}
void maschinen::pipe::NamedPipeBase::setName( LPCTSTR name ) {this->name = name;}
void maschinen::pipe::NamedPipeBase::setName( tstring& name ) {this->name = name;}
void maschinen::pipe::NamedPipeBase::setName( tstring* name ) {if(name) this->name.assign( name->c_str() );}
const TCHAR* maschinen::pipe::NamedPipeBase::getName() const noexcept { return name.c_str(); }
void maschinen::pipe::NamedPipeBase::setSecurityAttributes( maschinen::security::SecurityAttributes* security_attributes ) {this->security_attributes = security_attributes;}
maschinen::security::SecurityAttributes* maschinen::pipe::NamedPipeBase::getSecurityAttributes() const noexcept { return security_attributes; }
void maschinen::pipe::NamedPipeBase::close() {if ( handle != INVALID_HANDLE_VALUE ) CloseHandle( handle );}
maschinen::pipe::NamedPipeBase::~NamedPipeBase() {close();}

void maschinen::pipe::NamedPipeBase::setPipeModeMessage( bool mode_message ) {
	if( mode_message ) mode |= PIPE_TYPE_MESSAGE; 
	else {
		if ( mode & PIPE_TYPE_MESSAGE ) mode ^= PIPE_TYPE_MESSAGE;
	}
}
void maschinen::pipe::NamedPipeBase::setPipeReadModeMessage( bool mode_read_message ) {
	if ( mode_read_message ) {
		setPipeModeMessage();
		mode |= PIPE_READMODE_MESSAGE;
	}
	else {
		if ( mode & PIPE_READMODE_MESSAGE ) mode ^= PIPE_READMODE_MESSAGE;
	}
}
void maschinen::pipe::NamedPipeBase::setPipeModeNoWait( bool mode_no_wait ) {
	if ( mode_no_wait ) mode |= PIPE_NOWAIT;
	else {
		if ( mode & PIPE_NOWAIT ) mode ^= PIPE_NOWAIT;
	}
}

void maschinen::pipe::NamedPipeBase::setMaxInstance( DWORD max_instance ) {
	if ( max_instance > INSTANCE_UNLIMITED ) this->max_instance = INSTANCE_UNLIMITED;
	else this->max_instance = max_instance;
}

void maschinen::pipe::NamedPipeBase::setInOutBufferSize( DWORD in_buffer_size, DWORD out_buffer_size ) {
	this->in_buffer_size = in_buffer_size;
	this->out_buffer_size = out_buffer_size;
}

void maschinen::pipe::NamedPipeBase::setDefaultTimeOut( DWORD default_time_out ) {
	this->default_time_out = default_time_out;
}

void maschinen::pipe::NamedPipeBase::setAccessDuplex() {
	open_mode |= PIPE_ACCESS_DUPLEX;
	if ( open_mode & PIPE_ACCESS_INBOUND ) open_mode ^= PIPE_ACCESS_INBOUND;
	if ( open_mode & PIPE_ACCESS_OUTBOUND ) open_mode ^= PIPE_ACCESS_OUTBOUND;
}
void maschinen::pipe::NamedPipeBase::setAccessInBound() {
	if ( open_mode & PIPE_ACCESS_DUPLEX ) open_mode ^= PIPE_ACCESS_DUPLEX;
	open_mode |= PIPE_ACCESS_INBOUND;
	if ( open_mode & PIPE_ACCESS_OUTBOUND ) open_mode ^= PIPE_ACCESS_OUTBOUND;
}
void maschinen::pipe::NamedPipeBase::setAcessOutBound() {
	if ( open_mode & PIPE_ACCESS_DUPLEX ) open_mode ^= PIPE_ACCESS_DUPLEX;
	if ( open_mode & PIPE_ACCESS_INBOUND ) open_mode ^= PIPE_ACCESS_INBOUND;
	open_mode |= PIPE_ACCESS_OUTBOUND;
}
void maschinen::pipe::NamedPipeBase::setFlagFirstInstance( bool flag_first_instance ) {
	if ( flag_first_instance ) open_mode |= FILE_FLAG_FIRST_PIPE_INSTANCE;
	else {
		if( open_mode & FILE_FLAG_FIRST_PIPE_INSTANCE ) open_mode ^= FILE_FLAG_FIRST_PIPE_INSTANCE;
	}
}
void maschinen::pipe::NamedPipeBase::setFlagWriteThrough( bool flag_write_through ) {
	if ( flag_write_through ) open_mode |= FILE_FLAG_WRITE_THROUGH;
	else {
		if ( open_mode & FILE_FLAG_WRITE_THROUGH ) open_mode ^= FILE_FLAG_WRITE_THROUGH;
	}
}

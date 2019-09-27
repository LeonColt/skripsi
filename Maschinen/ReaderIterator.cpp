#include "stdafx.h"
#include "ReaderIterator.h"


maschinen::iterator::ReaderIterator::ReaderIterator() {
	handle = INVALID_HANDLE_VALUE;
	buffer = nullptr;
	buffer_size = 0;
	number_of_byte_read = 0;
}

maschinen::iterator::ReaderIterator& maschinen::iterator::ReaderIterator::operator++() {
	if ( handle == NULL || handle == INVALID_HANDLE_VALUE ) throw maschinen::exception::ReaderIteratorNullHandleException();
	if ( buffer == nullptr ) throw maschinen::exception::ReaderIteratorNullBufferException();
	if ( ReadFile( handle, buffer, buffer_size, &number_of_byte_read, NULL ) == TRUE ) {
		if ( number_of_byte_read == 0 ) handle = INVALID_HANDLE_VALUE;
	} else {
		if( GetLastError() == ERROR_BROKEN_PIPE ) {
			handle = INVALID_HANDLE_VALUE;
			return *this;
		}
		TCHAR buffer [ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
	return *this;
}
maschinen::iterator::ReaderIterator maschinen::iterator::ReaderIterator::operator++( int ) {
	ReaderIterator tmp = *this;
	++*this;
	return ( tmp );
}
maschinen::iterator::ReaderIterator& maschinen::iterator::ReaderIterator::preNext() {
	++*this;
	return *this;
}
maschinen::iterator::ReaderIterator maschinen::iterator::ReaderIterator::postNext() {
	ReaderIterator tmp = *this;
	++*this;
	return ( tmp );
}
bool maschinen::iterator::ReaderIterator::operator!=( const HANDLE finish_reader ) const noexcept {
	return handle != finish_reader;
}
bool maschinen::iterator::ReaderIterator::operator==( const HANDLE finish_reader ) const noexcept {
	return handle == finish_reader;
}

bool maschinen::iterator::ReaderIterator::operator!=( const ReaderIterator& finish_reader ) const noexcept {
	return handle != finish_reader.handle;
}
bool maschinen::iterator::ReaderIterator::operator==( const ReaderIterator& finish_reader ) const noexcept {
	return handle == finish_reader.handle;
}

bool maschinen::iterator::ReaderIterator::operator!=( const ReaderIterator* finish_reader ) const noexcept {
	return ( finish_reader == nullptr ) ? true : handle != finish_reader->handle;
}
bool maschinen::iterator::ReaderIterator::operator==( const ReaderIterator* finish_reader ) const noexcept {
	return ( finish_reader == nullptr ) ? false : handle == finish_reader->handle;
}

void maschinen::iterator::ReaderIterator::operator=( const ReaderIterator& that ) noexcept {
	this->handle = that.handle;
	this->buffer = that.buffer;
	this->buffer_size = that.buffer_size;
	this->number_of_byte_read = that.number_of_byte_read;
}
void maschinen::iterator::ReaderIterator::operator=( const ReaderIterator* that ) noexcept {
	if ( that == nullptr ) return;
	this->handle = that->handle;
	this->buffer = that->buffer;
	this->buffer_size = that->buffer_size;
	this->number_of_byte_read = that->number_of_byte_read;
}

maschinen::iterator::ReaderIterator::~ReaderIterator() {}

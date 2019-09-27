#include "stdafx.h"
#include "DirectoryHandler.h"


maschinen::file::DirectoryHandler::DirectoryHandler() {
}

maschinen::file::DirectoryHandler::DirectoryHandler( LPCTSTR path ) {
	this->path.assign( path );
}

maschinen::file::DirectoryHandler::DirectoryHandler( tstring& path ) {
	this->path.assign( path.c_str() );
}

void maschinen::file::DirectoryHandler::setPath( LPCTSTR path ) {
	this->path.assign( path );
}

void maschinen::file::DirectoryHandler::setPath( tstring& path ) {
	this->path.assign( path.c_str() );
}

maschinen::file::DirectoryReadingIterator& maschinen::file::DirectoryHandler::begin() {
	reader.reset();
	finish_reader.reset();
	WIN32_FIND_DATA data;
	reader = std::make_unique<DirectoryReadingIterator>();
	finish_reader = std::make_unique<DirectoryReadingIterator>();
	finish_reader->dh = nullptr;
	handler = FindFirstFile( path.c_str(), &data );
	if ( handler == INVALID_HANDLE_VALUE ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString(error, MAX_LOADSTRING, error_code) )
			 throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
	reader->dh = this;
	wcscpy_s( reader->alternate_file_name, data.cAlternateFileName );
	reader->file_name.assign( data.cFileName );
	reader->creation_time = data.ftCreationTime;
	reader->file_attributes = data.dwFileAttributes;
	reader->last_access_time = data.ftLastAccessTime;
	reader->last_write_time = data.ftLastWriteTime;
	reader->nfile_size_high = data.nFileSizeHigh;
	reader->nfile_size_low = data.nFileSizeLow;
	return *reader.get();
}

maschinen::file::DirectoryReadingIterator& maschinen::file::DirectoryHandler::end() const {
	return *finish_reader.get();
}

HANDLE& maschinen::file::DirectoryHandler::get() {
	return handler;
}

maschinen::file::DirectoryHandler::~DirectoryHandler() {
	if ( handler != INVALID_HANDLE_VALUE ) FindClose( handler );
}

size_t maschinen::file::DirectoryHandler::getNumberOfChildren() {
	size_t number_of_children = 0;
	for ( auto i = begin(); i != end(); i++ )
		number_of_children++;
	return number_of_children;
}

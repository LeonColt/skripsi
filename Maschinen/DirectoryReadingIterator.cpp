#include "stdafx.h"
#include "DirectoryReadingIterator.h"


maschinen::file::DirectoryReadingIterator::DirectoryReadingIterator() {
	dh = nullptr;
}

void maschinen::file::DirectoryReadingIterator::operator++() {
	if ( dh != nullptr ) {
		WIN32_FIND_DATA data;
		if ( FindNextFile( dh->get(), &data ) > 0 ) {
			wcscpy_s( alternate_file_name, data.cAlternateFileName );
			file_name.assign( data.cFileName );
			creation_time = data.ftCreationTime;
			file_attributes = data.dwFileAttributes;
			last_access_time = data.ftLastAccessTime;
			last_write_time = data.ftLastWriteTime;
			nfile_size_high = data.nFileSizeHigh;
			nfile_size_low = data.nFileSizeLow;
		}
		else dh = nullptr;
	}
}

void maschinen::file::DirectoryReadingIterator::operator++( int ) {
	if ( dh != nullptr ) {
		WIN32_FIND_DATA data;
		if ( FindNextFile( dh->get(), &data ) > 0 ) {
			wcscpy_s( alternate_file_name, data.cAlternateFileName );
			file_name.assign( data.cFileName );
			creation_time = data.ftCreationTime;
			file_attributes = data.dwFileAttributes;
			last_access_time = data.ftLastAccessTime;
			last_write_time = data.ftLastWriteTime;
			nfile_size_high = data.nFileSizeHigh;
			nfile_size_low = data.nFileSizeLow;
		}
		else dh = nullptr;
	}
}

bool maschinen::file::DirectoryReadingIterator::operator!=( const DirectoryReadingIterator& finish_reader ) const {
	return (( DirectoryHandler *)dh) != (( DirectoryHandler * ) finish_reader.dh);
}


maschinen::file::DirectoryReadingIterator::~DirectoryReadingIterator() {
}

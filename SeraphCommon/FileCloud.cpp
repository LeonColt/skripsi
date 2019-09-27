#include "stdafx.h"
#include "FileCloud.h"


FileCloud::FileCloud() {
	id = 0;
}

std::unique_ptr<tstring> FileCloud::getType() const noexcept {
	return std::make_unique<tstring>( TEXT( "Unknown" ) );
}
/*
//begin comparing file
bool FileCloud::operator==( FileCloud& file ) {
	if ( file.getTypeId() && format_id == file.getFormatId() ) {
		if ( FileCloud* fc = dynamic_cast< FileCloud* >( &file ) ) {
			if ( fc->getId() == id ) return true;
		}
	}
	return false;
}

bool FileCloud::operator==( FileCloud* file ) {
	if ( file->getTypeId() && format_id == file->getFormatId() ) {
		if ( FileCloud* fc = dynamic_cast< FileCloud* >( file ) ) {
			if ( fc->getId() == id ) return true;
		}
	}
	return false;
}

bool FileCloud::equal( FileCloud& file ) {
	if ( file.getTypeId() && format_id == file.getFormatId() ) {
		if ( FileCloud* fc = dynamic_cast< FileCloud* >( &file ) ) {
			if ( fc->getId() == id ) return true;
		}
	}
	return false;
}

bool FileCloud::equal( FileCloud* file ) {
	if ( file->getTypeId() && format_id == file->getFormatId() ) {
		if ( FileCloud* fc = dynamic_cast< FileCloud* >( file ) ) {
			if ( fc->getId() == id ) return true;
		}
	}
	return false;
}
//end comparing file
*/

FileCloud::~FileCloud() {
}

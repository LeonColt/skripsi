#include "stdafx.h"
#include "FileNotFoundException.h"


maschinen::exception::FileNotFoundException::FileNotFoundException() : maschinen::exception::Exception( TEXT( "The system cannot find the file specified." ), ERROR_FILE_NOT_FOUND) {
}


maschinen::exception::FileNotFoundException::~FileNotFoundException() {
}

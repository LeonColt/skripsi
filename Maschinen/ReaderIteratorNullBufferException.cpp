#include "stdafx.h"
#include "ReaderIteratorNullBufferException.h"


maschinen::exception::ReaderIteratorNullBufferException::ReaderIteratorNullBufferException() : maschinen::exception::Exception( TEXT( "Iterator Buffer must be provided" ), ERROR_INVALID_PARAMETER ) {}


maschinen::exception::ReaderIteratorNullBufferException::~ReaderIteratorNullBufferException() {}

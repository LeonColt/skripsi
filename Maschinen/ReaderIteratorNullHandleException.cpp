#include "stdafx.h"
#include "ReaderIteratorNullHandleException.h"


maschinen::exception::ReaderIteratorNullHandleException::ReaderIteratorNullHandleException() : maschinen::exception::Exception( TEXT("Handle cannot be NULL while Reading"), ERROR_INVALID_PARAMETER) {}


maschinen::exception::ReaderIteratorNullHandleException::~ReaderIteratorNullHandleException() {}

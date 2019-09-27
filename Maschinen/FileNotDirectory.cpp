#include "stdafx.h"
#include "FileNotDirectory.h"


maschinen::exception::FileNotDirectory::FileNotDirectory() : Exception(L"File is not a Directory", 0) {
}


maschinen::exception::FileNotDirectory::~FileNotDirectory() {
}

#pragma once
#include "DirectoryHandler.h"
#include "DirectoryReadingIterator.h"
#include "DriverIterator.h"
#include "DriveFixed.h"
#include "DriveRemovable.h"
#include "DriveCDRom.h"
#include "DriveUnknown.h"
class SERAPHCOMMON_API DriveLoader {
public:
	typedef DriverIterator iterator;
	DriveLoader();
	DriverIterator begin();
	DriverIterator end();
	~DriveLoader();
};


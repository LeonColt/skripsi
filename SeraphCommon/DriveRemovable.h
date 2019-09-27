#pragma once
#include "Drive.h"
#include "DirectoryHandler.h"
#include "DirectoryLocal.h"
#include "FileLocal.h"
class SERAPHCOMMON_API DriveRemovable : public Drive {
public:
	DriveRemovable();
	~DriveRemovable();
};


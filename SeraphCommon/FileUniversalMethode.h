#pragma once
#include <memory>
#include "SeraphCommon.h"
#include "Maschinen.h"
#include "Local.h"
#include "DriveFixed.h"
#include "DriveRemovable.h"
#include "DriveCDRom.h"

SERAPHCOMMON_API std::unique_ptr<Local> loadLocalByPath( LPCTSTR path ) noexcept;
SERAPHCOMMON_API std::unique_ptr<Local> loadLocalByPath( tstring& path ) noexcept;
SERAPHCOMMON_API std::unique_ptr<Local> loadLocalByPath( tstring* path ) noexcept;

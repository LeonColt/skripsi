#pragma once
#include "Setting.h"
#include <process.h>
#include "FileHandler.h"
#include "SeraphException.h"
class SERAPHCOMMON_API Log {
private:
	tstring name;
	SYSTEMTIME local_time, system_time;
	Setting* setting;
	bool external_local_time, external_system_time;
	bool exists();
	void create();
public:
	Log();
	void setFileName( LPCTSTR name );
	void write( LPCTSTR text );
	void setLocalTime( LPSYSTEMTIME local_time );
	void setSystemTime( LPSYSTEMTIME system_time );
	void setSetting( Setting* setting );

	std::unique_ptr<tstring> getPath();

	~Log();
};
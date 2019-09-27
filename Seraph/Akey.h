// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the AKEY_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// AKEY_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once
#include <vector>
#include <WinInet.h>
#include "Setting.h"
#include "ContainedController.h"
#include "Log.h"
#include "SeraphException.h"
#include "JSON.h"
#include "UniversalMethode.h"
// This class is exported from the Akey.dll
class Akey {
private:
	tstring app_key;
	Setting* setting;
	//HKEY hkey;
	LONG getAppKeyfromRegistry();
	void setAppKeytoRegistry();
	void getAppKeyfromInternet();
	void confirmAppkey();
public:
	Akey(void);
	// TODO: add your methods here.
	void execute();
	void setSetting(Setting* p_setting);
	std::unique_ptr<tstring> getAppKey();
	void logging(maschinen::wininet::HttpRequest* hr, tstring* result);
};

#pragma once
#include <exception>
#include <Poco\JSON\JSONException.h>
#include "AsynchronousTask.h"
#include "Akey.h"
#include "UserCredential.h"
#include "UniversalMethode.h"
class CustomAsyncTask : public maschinen::thread::task::AsynchronousTask
{
protected:
	Akey* akey = nullptr;
	UserCredential* uc = nullptr;
	Setting* setting = nullptr;
	TCHAR error_marker[ MAX_LOADSTRING ];
public:
	CustomAsyncTask();
	void setAkey(Akey* akey);
	void setUserCredential(UserCredential* uc);
	void setSetting(Setting* setting);
	void setErrorMarker(LPCTSTR error_marker);
	~CustomAsyncTask();
};


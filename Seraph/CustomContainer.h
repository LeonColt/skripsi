#pragma once
#include "Container.h"
#include "Setting.h"
#include "Akey.h"
#include "UserCredential.h"
class CustomContainer : public maschinen::Container
{
protected:
	Setting* setting;
	Akey* akey;
	UserCredential* uc;
	//ThreadParameter tp;
	TCHAR error_marker[MAX_LOADSTRING];
public:
	CustomContainer();
	void setSetting(Setting* setting);
	void setAkey(Akey* akey);
	void setUserCredential(UserCredential* uc);
	~CustomContainer();
};


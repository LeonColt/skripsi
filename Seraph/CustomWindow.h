#pragma once
#include "Window.h"
#include "Akey.h"
#include "Setting.h"
#include "UserCredential.h"
class Akey;
class Setting;
class CustomWindow : public maschinen::Window {
protected:
	Akey* akey = nullptr;
	Setting* setting = nullptr;
	UserCredential* user_credential = nullptr;
public:
	CustomWindow();
	void setAkey(Akey* akey) noexcept;
	void setSetting( Setting* setting ) noexcept;
	void setUserCredential( UserCredential* user_credential ) noexcept;
	Akey* getAkey() const noexcept;
	Setting* getSetting() const noexcept;
	UserCredential* getUserCredential() const noexcept;
	~CustomWindow();
};


#include "stdafx.h"
#include "CustomContainer.h"


CustomContainer::CustomContainer()
{
	wcscpy_s(error_marker, L"");
}

void CustomContainer::setSetting(Setting* setting)
{
	this->setting = setting;
}

void CustomContainer::setAkey(Akey* akey)
{
	this->akey = akey;
}

void CustomContainer::setUserCredential(UserCredential* uc)
{
	this->uc = uc;
}

CustomContainer::~CustomContainer()
{
}

#include "stdafx.h"
#include "CustomAsyncTask.h"
#include "Window.h"


CustomAsyncTask::CustomAsyncTask()
{
	wcscpy_s(error_marker, TEXT("Error"));
}

void CustomAsyncTask::setSetting(Setting* setting){this->setting = setting;}
void CustomAsyncTask::setUserCredential(UserCredential* uc){this->uc = uc;}
void CustomAsyncTask::setAkey(Akey* akey){this->akey = akey;}
void CustomAsyncTask::setErrorMarker(LPCTSTR error_marker){wcscpy_s(this->error_marker, error_marker);}


CustomAsyncTask::~CustomAsyncTask()
{
}

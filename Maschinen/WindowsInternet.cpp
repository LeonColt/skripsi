#include "stdafx.h"
#include "WindowsInternet.h"


maschinen::wininet::Wininet::Wininet()
{
	flags = 0;
	type_access = INTERNET_OPEN_TYPE_PRECONFIG;
	user_agent.assign(TEXT(""));
}

void maschinen::wininet::Wininet::initialize()
{
	handler = InternetOpen(
		user_agent.c_str(), 
		type_access, 
		(proxy_name.empty()) ? NULL : proxy_name.c_str(), 
		(proxy_bypass.empty()) ? NULL : proxy_bypass.c_str(), flags);
	if (handler == NULL)
	{
		DWORD error_code;
#ifdef _UNICODE
		if (getLastErrorAsString(error, MAX_LOADSTRING, error_code))
#else
		if (getLastErrorAsString(error_mb, MAX_LOADSTRING, error_code))
#endif // _UNICODE
		{
#ifdef _UNICODE
			size_t size = wcslen(error);
			wcstombs_s(&size, error_mb, error, MAX_LOADSTRING);
#endif // _UNICODE
			throw std::exception(error_mb, (int)error_code);
		}
		else throw std::exception("an unknown error occured");
	}
}

void maschinen::wininet::Wininet::setAccessTypeToDirect()
{
	type_access = INTERNET_OPEN_TYPE_DIRECT;
}

void maschinen::wininet::Wininet::setAccessTypeToPreConfig()
{
	type_access = INTERNET_OPEN_TYPE_PRECONFIG;
}

void maschinen::wininet::Wininet::setAccessTypeToPreConfigWithNoAutoProxy()
{
	type_access = INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY;
}

void maschinen::wininet::Wininet::setAccessTypeToProxy( LPCTSTR proxy_name, LPCTSTR proxy_bypass )
{
	type_access = INTERNET_OPEN_TYPE_PROXY;
	this->proxy_name.assign( proxy_name );
	this->proxy_bypass.assign( proxy_bypass );
}

void maschinen::wininet::Wininet::flagAsynchronous(bool mode)
{
	if (mode) flags |= INTERNET_FLAG_ASYNC;
	else
	{
		flags |= INTERNET_FLAG_ASYNC;
		flags ^= INTERNET_FLAG_ASYNC;
	}
}

void maschinen::wininet::Wininet::flagFromCache(bool mode)
{
	if (mode) flags |= INTERNET_FLAG_FROM_CACHE;
	else
	{
		flags |= INTERNET_FLAG_FROM_CACHE;
		flags ^= INTERNET_FLAG_FROM_CACHE;
	}
}

void maschinen::wininet::Wininet::internetFlagOffline(bool mode)
{
	if (mode) flags |= INTERNET_FLAG_OFFLINE;
	else
	{
		flags |= INTERNET_FLAG_OFFLINE;
		flags ^= INTERNET_FLAG_OFFLINE;
	}
}

void maschinen::wininet::Wininet::close()
{
#ifdef _DEBUG
	OutputDebugString(TEXT("WindowsInternet close\n"));
#endif // _DEBUG
	if (handler != NULL) InternetCloseHandle(handler);
}

HINTERNET maschinen::wininet::Wininet::getHandler()
{
	return handler;
}

maschinen::wininet::Wininet::~Wininet()
{
	close();
}

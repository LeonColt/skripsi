#include "stdafx.h"
#include "WininetConnect.h"


maschinen::wininet::WininetConnect::WininetConnect()
{
	port = INTERNET_DEFAULT_HTTP_PORT;
	service = INTERNET_SERVICE_HTTP;
}

void maschinen::wininet::WininetConnect::initialize()
{
	handler = InternetConnect(hwininet->handler, 
		server_name.c_str(), 
		port, 
		(username.empty()) ? NULL : username.c_str(), 
		(password.empty()) ? NULL : password.c_str(), 
		service, 
		flags, 
		context);
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

void maschinen::wininet::WininetConnect::setWininet(maschinen::wininet::Wininet* hwininet)
{
	this->hwininet = hwininet;
}

void maschinen::wininet::WininetConnect::setServerName(LPCTSTR server_name)
{
	this->server_name.assign(server_name);
}

void maschinen::wininet::WininetConnect::setPortToFTP()
{
	port = INTERNET_DEFAULT_FTP_PORT;
}

void maschinen::wininet::WininetConnect::setPortToGopher()
{
	port = INTERNET_DEFAULT_GOPHER_PORT;
}

void maschinen::wininet::WininetConnect::setPortToHttp()
{
	port = INTERNET_DEFAULT_HTTP_PORT;
}

void maschinen::wininet::WininetConnect::setPortToHttps()
{
	port = INTERNET_DEFAULT_HTTPS_PORT;
}

void maschinen::wininet::WininetConnect::setPortToSocks()
{
	port = INTERNET_DEFAULT_SOCKS_PORT;
}

void maschinen::wininet::WininetConnect::setPortToDefault()
{
	port = INTERNET_INVALID_PORT_NUMBER;
}

void maschinen::wininet::WininetConnect::setUsername(LPCTSTR username)
{
	this->username.assign(username);
}

void maschinen::wininet::WininetConnect::setPassword(LPCTSTR password)
{
	this->password.assign(password);
}

void maschinen::wininet::WininetConnect::setServiceToFTP()
{
	service = INTERNET_SERVICE_FTP;
}

void maschinen::wininet::WininetConnect::setServiceToGopher()
{
	service = INTERNET_SERVICE_GOPHER;
}

void maschinen::wininet::WininetConnect::setServiceToHttp()
{
	service = INTERNET_SERVICE_HTTP;
}

void maschinen::wininet::WininetConnect::setContext(DWORD_PTR context)
{
	this->context = context;
}

void maschinen::wininet::WininetConnect::close()
{
#ifdef _DEBUG
	OutputDebugString(TEXT("WininetConnect close\n"));
#endif // _DEBUG
	if(handler != NULL) InternetCloseHandle(handler);
}

maschinen::wininet::WininetConnect::~WininetConnect()
{
	close();
}

#pragma once
#include <WinInet.h>
#include <string>
#include "WindowsInternet.h"
namespace maschinen
{
	namespace wininet
	{
		class Wininet;
		class MASCHINEN_API WininetConnect
		{
		private:
			maschinen::wininet::Wininet* hwininet = nullptr;
			TCHAR error[MAX_LOADSTRING];
			char error_mb[MAX_LOADSTRING];
			HINTERNET handler;
			tstring server_name;
			tstring username;
			tstring password;
			INTERNET_PORT port;
			DWORD service, flags;
			DWORD_PTR context = NULL;
		public:
			WininetConnect();
			void setWininet(maschinen::wininet::Wininet* hwininet);
			void initialize();
			void setServerName(LPCTSTR server_name);
			void setPortToFTP();
			void setPortToGopher();
			void setPortToHttp();
			void setPortToHttps();
			void setPortToSocks();
			void setPortToDefault();
			void setUsername(LPCTSTR username);
			void setPassword(LPCTSTR password);
			void setServiceToFTP();
			void setServiceToGopher();
			void setServiceToHttp();
			void setContext(DWORD_PTR context);
			void close();
			~WininetConnect();
		};
	}
}


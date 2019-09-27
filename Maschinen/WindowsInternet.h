#pragma once
#include <WinInet.h>
#include <string>
#include "Maschinen.h"
#include "WininetConnect.h"
namespace maschinen
{
	namespace wininet
	{
		class MASCHINEN_API Wininet
		{
			friend class WininetConnect;
		private:
			HINTERNET handler;
			tstring user_agent;
			tstring proxy_name;
			tstring proxy_bypass;
#ifdef _UNICODE
			TCHAR error[MAX_LOADSTRING];
#endif // _UNICODE
			char error_mb[MAX_LOADSTRING];
			DWORD type_access, flags;
		public:
			Wininet();
			void initialize();
			//Resolves all host names locally.
			void setAccessTypeToDirect();
			//Retrieves the proxy or direct configuration from the registry.
			void setAccessTypeToPreConfig();
			//Retrieves the proxy or direct configuration from the registry and prevents the use of a startup Microsoft JScript or Internet Setup (INS) file.
			void setAccessTypeToPreConfigWithNoAutoProxy();
			//Passes requests to the proxy unless a proxy bypass list is supplied and the name to be resolved bypasses the proxy. In this case, the function uses INTERNET_OPEN_TYPE_DIRECT.
			void setAccessTypeToProxy(LPCTSTR proxy_name, LPCTSTR proxy_bypass);
			//Makes only asynchronous requests on handles descended from the handle returned from this function.
			void flagAsynchronous(bool mode = true);
			//Does not make network requests. All entities are returned from the cache. If the requested item is not in the cache, a suitable error, such as ERROR_FILE_NOT_FOUND, is returned.
			void flagFromCache(bool mode = true);
			//Identical to INTERNET_FLAG_FROM_CACHE. Does not make network requests. All entities are returned from the cache. If the requested item is not in the cache, a suitable error, such as ERROR_FILE_NOT_FOUND, is returned.
			void internetFlagOffline(bool mode = true);
			//close wininet handler
			void close();
			HINTERNET getHandler();
			~Wininet();
		};
	}
}


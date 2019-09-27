#pragma once
#include "HttpRequest.h"
namespace maschinen
{
	namespace wininet
	{
		class HttpRequest;
		class MASCHINEN_API HttpRequestReadingIterator
		{
		public:
			maschinen::wininet::HttpRequest* hr = nullptr;
			LPVOID buffer;
			DWORD buffer_size;
			LPDWORD number_of_byte_read;

			HttpRequestReadingIterator();
			void operator++(int);
			bool operator!=(const HttpRequestReadingIterator& finish);
			~HttpRequestReadingIterator();
		};
	}
}


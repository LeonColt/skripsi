#include "stdafx.h"
#include "HttpRequestReadingIterator.h"


maschinen::wininet::HttpRequestReadingIterator::HttpRequestReadingIterator()
{
	hr = nullptr;
}

void maschinen::wininet::HttpRequestReadingIterator::operator++(int)
{
	if (hr != nullptr)
	{
		if (InternetReadFile(hr->hrequest, buffer, buffer_size, number_of_byte_read))
		{
			if (*number_of_byte_read == 0) hr = nullptr;
		}
		else hr = nullptr;
	}
}

bool maschinen::wininet::HttpRequestReadingIterator::operator!=(const HttpRequestReadingIterator& finish)
{
	return hr != finish.hr;
}

maschinen::wininet::HttpRequestReadingIterator::~HttpRequestReadingIterator()
{
}

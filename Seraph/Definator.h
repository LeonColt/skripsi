#pragma once
#include <vector>
#include "HttpParam.h"
#include "HttpParams.h"
#include "WininetConnect.h"
#include "HttpRequest.h"
#include "Setting.h"
class Setting;
struct ThreadParameter{
	int event_code;
	maschinen::wininet::HttpParams* params = nullptr;
	maschinen::wininet::WininetConnect* wc = nullptr;
	HWND main_handler = nullptr;
	Setting* setting = nullptr;
	INT status = HTTP_STATUS_BAD_GATEWAY;
	TCHAR result[MAX_LOADSTRING];
	TCHAR error[MAX_LOADSTRING];
	size_t result_size = MAX_LOADSTRING;
	size_t error_size = MAX_LOADSTRING;
};
struct ATResult {
	const DWORD STATUS_PAUSE = 1;
	const DWORD STATUS_FINISH = 2;
	const DWORD STATUS_ERROR = 3;
	DWORD status = 0;
	tstring result;
};
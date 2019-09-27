#pragma once
#include <vector>
#include <ShlObj.h>
#include "Maschinen.h"
#include "SeraphCommon.h"
#include "HttpRequest.h"
#include "HttpHeaders.h"
#include "HttpParams.h"
#include "SeraphException.h"

SERAPHCOMMON_API INT sendRequest( LPCTSTR server_name, LPCTSTR url, maschinen::wininet::HttpRequest* comm, LPCTSTR request, LPCTSTR akey, maschinen::wininet::HttpParams* params );
SERAPHCOMMON_API INT sendRequest( LPCTSTR server_name, LPCTSTR url, maschinen::wininet::HttpRequest* comm, const tstring& request, LPCTSTR akey, maschinen::wininet::HttpParams* params );

SERAPHCOMMON_API void getDefaultHttpParam( maschinen::wininet::HttpParams* params, const tstring* php_session_id );
SERAPHCOMMON_API void getDefaultHttpParam( maschinen::wininet::HttpParams& params, const tstring* php_session_id );

SERAPHCOMMON_API std::unique_ptr<tstring> getDefaultAppDataPath( bool add_back_slash_in_the_end = false );
SERAPHCOMMON_API std::unique_ptr<tstring> fileSizeToStr( ULONGLONG file_size );
SERAPHCOMMON_API ULONGLONG fileSizeToProperSize( ULONGLONG file_size );
SERAPHCOMMON_API void systemTimetoWString( LPTSTR text, size_t size, LPCTSTR seperator, LPSYSTEMTIME st );
SERAPHCOMMON_API std::unique_ptr<tstring> systemTimeTotstr( LPCTSTR seperator, LPSYSTEMTIME st );
SERAPHCOMMON_API std::unique_ptr<tstring> readHttpRequestToStr( maschinen::wininet::HttpRequest* request );
SERAPHCOMMON_API std::unique_ptr<tstring> floorSeconds( LONGLONG seconds );
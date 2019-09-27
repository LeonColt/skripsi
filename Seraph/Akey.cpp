// Akey.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Akey.h"


// This is the constructor of a class that has been exported.
// see Akey.h for the class definition
Akey::Akey(){}
void Akey::execute()
{
	/*
	1. dapatkan key dari registry
	2. jika tidak ada maka dapatkan key dari internet
	3. jika tidak ada maka tampilkan error, lalu exit, jika ada maka simpan ke registry
	4. ulangi langkah dari 1, jika ada copy ke string app_key, jika tidak ada tampilkan error
	*/
	try {
		if ( getAppKeyfromRegistry() == ERROR_SUCCESS ) return;
		getAppKeyfromInternet();
		setAppKeytoRegistry();
		confirmAppkey();
		if ( getAppKeyfromRegistry() == ERROR_SUCCESS ) return;
	} catch ( SeraphException& e ) {
		throw e;
	} catch ( maschinen::exception::Exception& e ) {
		throw e;
	}
}

void Akey::confirmAppkey()
{
	maschinen::wininet::HttpParams params;
	getDefaultHttpParam(&params, NULL);
	maschinen::wininet::HttpRequest comm;
	INT status = sendRequest( setting->getServerName()->c_str(), setting->getUrl()->c_str(), &comm, internet::value::request::CONFIRM_APP_KEY, app_key.c_str(), &params );
	tstring result( readHttpRequestToStr( &comm )->c_str() );
	logging( &comm, &result );
	if ( status != HTTP_STATUS_OK ) throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), result.c_str(), 0 );
	Poco::JSON::Object::Ptr json = deserialization( result.c_str() );
	if ( !json->has( VAR_JSON_STATUS ) ) throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), L"Invalid JSON Result", 0 );
	if ( json->get( VAR_JSON_STATUS ).extract<int>() == 0 ) {
		if(json->has(VAR_JSON_MESSAGE) ) throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), json->get(VAR_JSON_MESSAGE).extract<std::string>().c_str(), 0 );
		else throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), L"Unknow Error Occured", 0 );
	}
}
void Akey::getAppKeyfromInternet()
{
	maschinen::wininet::HttpParams params;
	getDefaultHttpParam( &params, nullptr );
	maschinen::wininet::HttpRequest comm;
	INT status = sendRequest( setting->getServerName()->c_str(), setting->getUrl()->c_str(), &comm, internet::value::request::APP_KEY, nullptr, &params );
	tstring result( readHttpRequestToStr( &comm )->c_str() );
	logging( &comm, &result );
	if ( status != HTTP_STATUS_OK ) {
		throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), result.c_str(), status );
	}
	Poco::JSON::Object::Ptr json = deserialization( result.c_str() );
	if(!json->has(VAR_JSON_STATUS) ) throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), L"Invalid JSON Result", 0 );
	if ( json->get( VAR_JSON_STATUS ).extract<int>() == 0 || !json->has(VAR_JSON_RESULT) ) {
		if ( json->has( VAR_JSON_MESSAGE ) ) 
			throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), json->get(VAR_JSON_MESSAGE).extract<std::string>().c_str(), 0 );
		else throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), L"Unknow Error Occured", 0 );
	}
	app_key.assign( strToWstr( json->get( VAR_JSON_RESULT ).extract<std::string>().c_str() )->c_str() );
}

void Akey::setAppKeytoRegistry()
{
	HKEY hkey;
	LONG result = RegCreateKeyEx( HKEY_LOCAL_MACHINE, L"SOFTWARE\\Seraph", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hkey, NULL );
	if ( result != ERROR_SUCCESS ) throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), translateErrorCode( result )->c_str(), result );
	result = RegSetValueEx( hkey, L"erhlanten-daten-9493", 0, REG_SZ, ( const BYTE* ) app_key.c_str(), ( DWORD ) ( wcslen( app_key.c_str() ) * 2 + 1 ) );
	if ( result != ERROR_SUCCESS ) { RegCloseKey( hkey ); throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), translateErrorCode( result )->c_str(), result );}
	RegCloseKey( hkey );
}

LONG Akey::getAppKeyfromRegistry()
{
	HKEY hkey;
	LONG result = RegOpenKeyEx( HKEY_LOCAL_MACHINE, L"SOFTWARE\\Seraph", 0, KEY_READ, &hkey );
	if ( result == ERROR_FILE_NOT_FOUND || result == ERROR_PATH_NOT_FOUND ) return result;
	else if ( result != ERROR_SUCCESS ) throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), translateErrorCode( result )->c_str(), result );
	DWORD size = 512;
	std::unique_ptr<BYTE[]> buffer;
	do {
		buffer = std::make_unique<BYTE[]>( size );
		result = RegQueryValueEx( hkey, L"erhlanten-daten-9493", NULL, NULL, buffer.get(), &size );
		size++;
	} while ( result == ERROR_MORE_DATA );
	if(result != ERROR_SUCCESS && result != ERROR_FILE_NOT_FOUND && result != ERROR_PATH_NOT_FOUND) {
		RegCloseKey( hkey );
		throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), to_tstring(result).c_str(), result );
		//throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), translateErrorCode( result )->c_str(), result );
	}
	else if (result == ERROR_SUCCESS) app_key.assign( ( TCHAR* ) buffer.get() );
	RegCloseKey( hkey );
	return result;
}

std::unique_ptr<tstring> Akey::getAppKey() {return std::make_unique<tstring>( app_key.c_str() );}
void Akey::setSetting(Setting* p_setting){this->setting = p_setting;}

void Akey::logging(maschinen::wininet::HttpRequest* hr, tstring* result)
{
	Log log;
	log.setSetting(setting);
	if (setting->getLogUrl()) log.write(hr->getUrl()->c_str());
	if ( setting->getLogPost() ) log.write( hr->getSendingData()->c_str() );
	if (setting->getLogResult()) log.write(result->c_str());
}
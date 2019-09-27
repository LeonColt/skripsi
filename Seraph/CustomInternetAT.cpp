#include "stdafx.h"
#include "CustomInternetAT.h"


CustomInternetAT::CustomInternetAT() {
}

LPVOID CustomInternetAT::doInBackground( LPVOID input ) {
	std::unique_ptr<ATResult> result = std::make_unique<ATResult>();

	maschinen::wininet::HttpRequest comm;

	try {
		INT status = sendRequest( setting->getServerName()->c_str(), setting->getUrl()->c_str(), &comm, TEXT( "" ), akey->getAppKey()->c_str(), ( maschinen::wininet::HttpParams* )input );
		{
			char buffer_mb[ MAX_LOADSTRING ];
			TCHAR buffer[ MAX_LOADSTRING ];
			DWORD number_of_byte_read = 0;
			size_t size;
			for ( auto i = comm.beginRead( buffer_mb, MAX_LOADSTRING, &number_of_byte_read ); i != comm.endRead(); i++ ) {
				buffer_mb[ number_of_byte_read ] = 0;
				size = strlen( buffer_mb );
				mbstowcs_s( &size, buffer, buffer_mb, MAX_LOADSTRING );
				result->result.append( buffer );
			}
		}
		if ( status == HTTP_STATUS_OK ) result->status = result->STATUS_FINISH; else result->status = result->STATUS_ERROR;
	} catch ( maschinen::exception::Exception& e ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		e.getMessage( buffer, MAX_LOADSTRING );
		if ( parent->compare( WINDOW ) )
			MessageBox( parent->getHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
		else if ( parent->compare( CONTAINER ) )
			MessageBox( ( ( maschinen::Container* )parent )->getWindowHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
		else
			MessageBox( NULL, buffer, error_marker, MB_ICONERROR | MB_OK );
		SeraphException se;
		se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
	} catch ( std::exception& e ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		size_t size = strlen( e.what() );
		mbstowcs_s( &size, buffer, e.what(), MAX_LOADSTRING );
		if ( parent->compare( WINDOW ) )
			MessageBox( parent->getHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
		else if ( parent->compare( CONTAINER ) )
			MessageBox( ( ( maschinen::Container* )parent )->getWindowHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
		else
			MessageBox( NULL, buffer, error_marker, MB_ICONERROR | MB_OK );
		SeraphException se;
		se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
	}
	logging( &comm, &result->result );
	return ( LPVOID ) result.release();
}

void CustomInternetAT::onPostExecute( LPVOID result ) {
	std::unique_ptr<ATResult> res( ( ATResult* ) result );
	//TCHAR buffer[ MAX_LOADSTRING ];
	onPrePostExecute();
	if ( res->status == res->STATUS_FINISH ) {

	}
	/*
	if ( status == HTTP_STATUS_OK ) {
		try {
			Poco::JSON::Object::Ptr json;
			if ( parseJson( ( ( tstring* ) result )->c_str(), json ) ) {
				if ( jsonCheckStatus( json ) ) {
					if ( json->has( internet::variable::VAR_PHPSESSID.c_str() ) ) {
						size_t size = strlen( json->get( internet::variable::VAR_PHPSESSID.c_str() ).extract<std::string>().c_str() );
						mbstowcs_s( &size, buffer, json->get( internet::variable::VAR_PHPSESSID.c_str() ).extract<std::string>().c_str(), MAX_LOADSTRING );
						uc->setPhpSessionId( buffer );
#ifdef _DEBUG
						OutputDebugStringW( L"\n PHPSESSID: " );
						OutputDebugStringW( buffer );
						OutputDebugStringW( L"\n" );
#endif // _DEBUG
					}
					onMainPostExecute( json );
				}
				else {
					if ( json->has( VAR_JSON_MESSAGE ) ) {
						TCHAR buffer[ MAX_LOADSTRING ];
						size_t size = strlen( json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str() );
						mbstowcs_s( &size, buffer, json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str(), MAX_LOADSTRING );
						if ( parent->compare( WINDOW ) )
							MessageBox( parent->getHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
						else if ( parent->compare( CONTAINER ) )
							MessageBox( ( ( maschinen::Container* )parent )->getWindowHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
						else
							MessageBox( NULL, buffer, error_marker, MB_ICONERROR | MB_OK );
					}
					else {
						if ( parent->compare( WINDOW ) )
							MessageBox( parent->getHandler(), L"invalid server status", error_marker, MB_ICONERROR | MB_OK );
						else if ( parent->compare( CONTAINER ) )
							MessageBox( ( ( maschinen::Container* )parent )->getWindowHandler(), L"invalid server status", error_marker, MB_ICONERROR | MB_OK );
						else
							MessageBox( NULL, L"invalid server status", error_marker, MB_ICONERROR | MB_OK );
					}
				}
			}
		} catch ( Poco::JSON::JSONException& e ) {
			TCHAR buffer[ MAX_LOADSTRING ];
			size_t size = strlen( e.what() );
			mbstowcs_s( &size, buffer, e.what(), MAX_LOADSTRING );
			if ( parent->compare( WINDOW ) )
				MessageBox( parent->getHandler(), buffer, L"Error", MB_ICONERROR | MB_OK );
			else if ( parent->compare( CONTAINER ) )
				MessageBox( ( ( maschinen::Container* )parent )->getWindowHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
			else
				MessageBox( NULL, buffer, error_marker, MB_ICONERROR | MB_OK );
			SeraphException se;
			se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
		} catch ( std::exception& e ) {
			TCHAR buffer[ MAX_LOADSTRING ];
			size_t size = strlen( e.what() );
			mbstowcs_s( &size, buffer, e.what(), MAX_LOADSTRING );
			if ( parent->compare( WINDOW ) )
				MessageBox( parent->getHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
			else if ( parent->compare( CONTAINER ) )
				MessageBox( ( ( maschinen::Container* )parent )->getWindowHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
			else
				MessageBox( NULL, buffer, error_marker, MB_ICONERROR | MB_OK );
			SeraphException se;
			se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
		}
	}
	else {
		TCHAR code[ MAX_LOADSTRING ];
		wcscpy_s( buffer, TEXT( "error, status code : " ) );
		_itow_s( status, code, 10 );
		wcscat_s( buffer, code );

		if ( parent->compare( WINDOW ) )
			MessageBox( parent->getHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
		else if ( parent->compare( CONTAINER ) )
			MessageBox( ( ( maschinen::Container* )parent )->getWindowHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
		else
			MessageBox( NULL, buffer, error_marker, MB_ICONERROR | MB_OK );
		SeraphException se;
		se.write( string::file::name::ERROR_LOG_NAME.c_str(), code );
	}
	onPostPostExecute();
	delete result;*/
}

void CustomInternetAT::logging( maschinen::wininet::HttpRequest* hr, tstring* result ) {
	try {
		if ( setting == nullptr ) {
			OutputDebugStringW( L"SETTING IS NULL" );
			return;
		}
		Log log;
		log.setFileName( string::file::name::REQUEST_LOG_NAME.c_str() );
		log.setSetting( setting );
		if ( setting->getLogUrl() && hr != nullptr ) log.write( hr->getUrl()->c_str() );
		if ( setting->getLogPost() && hr != nullptr ) log.write( hr->getSendingData()->c_str() );
		if ( setting->getLogResult() && hr != nullptr ) log.write( result->c_str() );
	} catch ( SeraphException& e ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		e.getMessage( buffer, MAX_LOADSTRING );
		if ( parent->compare( WINDOW ) )
			MessageBox( parent->getHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
		else if ( parent->compare( CONTAINER ) )
			MessageBox( ( ( maschinen::Container* )parent )->getWindowHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
		else
			MessageBox( NULL, buffer, error_marker, MB_ICONERROR | MB_OK );
	}
}

void CustomInternetAT::onPrePostExecute() {}
void CustomInternetAT::onMainPostExecute( Poco::JSON::Object::Ptr& json ) {}
void CustomInternetAT::onPostPostExecute() {}

CustomInternetAT::~CustomInternetAT() {
}

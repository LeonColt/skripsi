#include "stdafx.h"
#include "CustomInternetJSONAsyncTask.h"


CustomInternetJSONAsyncTask::CustomInternetJSONAsyncTask() {
}

LPVOID CustomInternetJSONAsyncTask::doInBackground( LPVOID input ) {
	ATRequestAndParamCustomInternetJson* incomers = ( ATRequestAndParamCustomInternetJson* ) input;
	tstring* result = new tstring();
	maschinen::wininet::HttpRequest comm;
	try {
		status = sendRequest( setting->getServerName()->c_str(), setting->getUrl()->c_str(), &comm, *incomers->request, akey->getAppKey()->c_str(), incomers->params );
		char buffer_mb[ MAX_LOADSTRING ];
		TCHAR buffer[ MAX_LOADSTRING ];
		DWORD number_of_byte_read = 0;
		size_t size;
		for ( auto i = comm.beginRead( buffer_mb, MAX_LOADSTRING - 1, &number_of_byte_read ); i != comm.endRead(); i++ ) {
			buffer_mb[ number_of_byte_read ] = 0;
			size = strlen( buffer_mb );
			mbstowcs_s( &size, buffer, buffer_mb, MAX_LOADSTRING );
			result->append( buffer );
		}
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
	logging( &comm, result );
	return ( LPVOID ) result;
}

void CustomInternetJSONAsyncTask::onPostExecute( LPVOID result ) {
	std::unique_ptr<tstring> res( ( tstring* ) result );
	onPrePostExecute();
	if ( status != HTTP_STATUS_OK ) {
		tstring message( res->c_str() );
		message.append( TEXT( " : " ) );
		message.append( to_tstring( status ) );

		if ( parent->compare( WINDOW ) )
			MessageBox( parent->getHandler(), message.c_str(), error_marker, MB_ICONERROR | MB_OK );
		else if ( parent->compare( CONTAINER ) )
			MessageBox( ( ( maschinen::Container* )parent )->getWindowHandler(), message.c_str(), error_marker, MB_ICONERROR | MB_OK );
		else
			MessageBox( NULL, message.c_str(), error_marker, MB_ICONERROR | MB_OK );
		SeraphException se;
		se.write( string::file::name::ERROR_LOG_NAME.c_str(), message.c_str() );
		return;
	}
	try {
		OutputDebugStringW( res->c_str() );
		OutputDebugStringW( TEXT( "\n" ) );
		Poco::JSON::Object::Ptr json = deserialization( res->c_str() );
		if ( !json->has( VAR_JSON_STATUS ) ) {
			if ( json->has( VAR_JSON_MESSAGE ) ) {
				if ( parent->compare( WINDOW ) )
					MessageBox( parent->getHandler(), strToWstr( json->get( VAR_JSON_MESSAGE ).extract<std::string>() )->c_str(), error_marker, MB_ICONERROR | MB_OK );
				else if ( parent->compare( CONTAINER ) )
					MessageBox( ( ( maschinen::Container* )parent )->getWindowHandler(), strToWstr( json->get( VAR_JSON_MESSAGE ).extract<std::string>() )->c_str(), error_marker, MB_ICONERROR | MB_OK );
				else
					MessageBox( NULL, strToWstr( json->get( VAR_JSON_MESSAGE ).extract<std::string>() )->c_str(), error_marker, MB_ICONERROR | MB_OK );
			}
			else{
				if ( parent->compare( WINDOW ) )
					MessageBox( parent->getHandler(), TEXT( "Invalid JSON Format" ), error_marker, MB_ICONERROR | MB_OK );
				else if ( parent->compare( CONTAINER ) )
					MessageBox( ( ( maschinen::Container* )parent )->getWindowHandler(), TEXT( "Invalid JSON Format" ), error_marker, MB_ICONERROR | MB_OK );
				else
					MessageBox( NULL, TEXT( "Invalid JSON Format" ), error_marker, MB_ICONERROR | MB_OK );
			}
			return;
		}
		if ( json->get( VAR_JSON_STATUS ).extract<int>() == 0 ) {
			onJsonStatusFail( json );
			return;
		}
		if ( json->has( internet::variable::VAR_PHPSESSID.c_str() ) ) uc->setPhpSessionId( strToWstr( json->get( internet::variable::VAR_PHPSESSID.c_str() ).extract<std::string>().c_str() )->c_str() );
		onMainPostExecute( json );
	} catch ( Poco::JSON::JSONException& e ) {
		if ( parent->compare( WINDOW ) )
			MessageBox( parent->getHandler(), strToWstr( e.what() )->c_str(), L"Error", MB_ICONERROR | MB_OK );
		else if ( parent->compare( CONTAINER ) )
			MessageBox( ( ( maschinen::Container* )parent )->getWindowHandler(), strToWstr( e.what() )->c_str(), error_marker, MB_ICONERROR | MB_OK );
		else
			MessageBox( NULL, strToWstr( e.what() )->c_str(), error_marker, MB_ICONERROR | MB_OK );
		SeraphException se;
		se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
	} catch ( std::exception& e ) {
		if ( parent->compare( WINDOW ) )
			MessageBox( parent->getHandler(), strToWstr(e.what())->c_str(), error_marker, MB_ICONERROR | MB_OK );
		else if ( parent->compare( CONTAINER ) )
			MessageBox( ( ( maschinen::Container* )parent )->getWindowHandler(), strToWstr( e.what() )->c_str(), error_marker, MB_ICONERROR | MB_OK );
		else
			MessageBox( NULL, strToWstr( e.what() )->c_str(), error_marker, MB_ICONERROR | MB_OK );
		SeraphException se;
		se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
	}
	onPostPostExecute();
}

void CustomInternetJSONAsyncTask::onJsonStatusFail( Poco::JSON::Object::Ptr& json ) {
	if ( json->has( VAR_JSON_MESSAGE ) ) {
		if ( parent->compare( WINDOW ) )
			MessageBox( parent->getHandler(), strToWstr( json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str() )->c_str(), error_marker, MB_ICONERROR | MB_OK );
		else if ( parent->compare( CONTAINER ) )
			MessageBox( ( ( maschinen::Container* )parent )->getWindowHandler(), strToWstr( json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str() )->c_str(), error_marker, MB_ICONERROR | MB_OK );
		else
			MessageBox( NULL, strToWstr( json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str() )->c_str(), error_marker, MB_ICONERROR | MB_OK );
		return;
	}
	else {
		if ( parent->compare( WINDOW ) )
			MessageBox( parent->getHandler(), L"Unknow Error Occured", error_marker, MB_ICONERROR | MB_OK );
		else if ( parent->compare( CONTAINER ) )
			MessageBox( ( ( maschinen::Container* )parent )->getWindowHandler(), L"Unknow Error Occured", error_marker, MB_ICONERROR | MB_OK );
		else
			MessageBox( NULL, L"Unknow Error Occured", error_marker, MB_ICONERROR | MB_OK );
		return;
	}
}

void CustomInternetJSONAsyncTask::logging( maschinen::wininet::HttpRequest* hr, tstring* result ) {
	try {
		if ( setting == nullptr ) {
			OutputDebugStringW( L"SETTING IS NULL" );
			return;
		}
		Log log;
		log.setFileName( string::file::name::REQUEST_LOG_NAME.c_str() );
		log.setSetting( setting );
		if ( setting->getLogUrl() && hr != nullptr ) log.write( hr->getUrl()->c_str() );
		if ( setting->getLogPost() && hr != nullptr ) log.write(hr->getSendingData()->c_str());
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

void CustomInternetJSONAsyncTask::onPrePostExecute() {}
void CustomInternetJSONAsyncTask::onMainPostExecute( Poco::JSON::Object::Ptr& json ) {}
void CustomInternetJSONAsyncTask::onPostPostExecute() {}


CustomInternetJSONAsyncTask::~CustomInternetJSONAsyncTask() {
}

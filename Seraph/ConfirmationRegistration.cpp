#include "stdafx.h"
#include "ConfirmationRegistration.h"


ConfirmationRegistration::ConfirmationRegistration() {
}

LPVOID ConfirmationRegistration::doInBackground( LPVOID input ) {
	maschinen::wininet::HttpParams params;
	getDefaultHttpParam( params, &uc->getPhpSessionId() );
	params.add( SECURITY_TOKEN, token.c_str(), params.METHOD_POST );
	params.add( SECRET_KEY, secret_key.c_str(), params.METHOD_POST );
	ATRequestAndParamCustomInternetJson atp;
	atp.request = &internet::value::request::CONFIRM_REGISTRATION;
	atp.params = &params;
	return CustomInternetJSONAsyncTask::doInBackground( &atp );
}
void ConfirmationRegistration::onMainPostExecute( Poco::JSON::Object::Ptr& json ){
	if ( json->has( VAR_JSON_MESSAGE ) )
		MessageBox( parent->getWindowHandler(), 
					strToWstr( json->get( VAR_JSON_MESSAGE ).extract<std::string>() )->c_str(), 
					error_marker, MB_ICONINFORMATION | MB_OK );
}

void ConfirmationRegistration::setToken( LPCTSTR token ) {
	this->token.assign( token );
}
void ConfirmationRegistration::setSecretKey( LPCTSTR secret_key ){
	this->secret_key.assign( secret_key );
}

ConfirmationRegistration::~ConfirmationRegistration() {
}

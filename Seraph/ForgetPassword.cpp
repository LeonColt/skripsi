#include "stdafx.h"
#include "ForgetPassword.h"

ForgetPassword::ForgetPassword() : ID_USERNAME_EMAIL(TEXT("cf-fp-32948239048023948239482394")){
	id_username_email.assign( TEXT( "" ) );
}

LPVOID ForgetPassword::doInBackground( LPVOID input ) {
	maschinen::wininet::HttpParams params;
	getDefaultHttpParam( params, &uc->getPhpSessionId() );
	params.add( ID_USERNAME_EMAIL, id_username_email.c_str(), params.METHOD_POST );
	ATRequestAndParamCustomInternetJson atp;
	atp.request = &internet::value::request::FORGET_PASSWORD;
	atp.params = &params;
	return CustomInternetJSONAsyncTask::doInBackground( &atp );
}
void ForgetPassword::onMainPostExecute( Poco::JSON::Object::Ptr& json ){
	if( json->has(VAR_JSON_MESSAGE) )
		MessageBox( parent->getWindowHandler(), strToWstr( json->get( VAR_JSON_MESSAGE ).extract<std::string>() )->c_str(), error_marker, MB_ICONINFORMATION | MB_OK );
}

void ForgetPassword::setIdUsernameEmail( LPCTSTR id_username_email ) {
	this->id_username_email.assign( id_username_email );
}

ForgetPassword::~ForgetPassword() {
}

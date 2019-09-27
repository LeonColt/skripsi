#include "stdafx.h"
#include "RegistryRegistry.h"


RegistryRegistry::RegistryRegistry()
{
}

void RegistryRegistry::onPreExecute()
{
	CustomAsyncTask::onPreExecute();
	pb->show();
	pb->setMarqueeSpeed(true, 1);
	btt_reg->disable();
	btt_login->disable();
}

LPVOID RegistryRegistry::doInBackground( LPVOID input ) {
	maschinen::wininet::HttpParams params;
	getDefaultHttpParam( params, &uc->getPhpSessionId() );
	params.add( ID, user_id, params.METHOD_POST );
	params.add( USERNAME, username, params.METHOD_POST );
	params.add( EMAIL, email, params.METHOD_POST );
	params.add( PASSWORD, password, params.METHOD_POST );
	ATRequestAndParamCustomInternetJson temp;
	temp.request = &internet::value::request::REGISTRATION;
	temp.params = &params;
	return CustomInternetJSONAsyncTask::doInBackground( &temp );
}

void RegistryRegistry::onPrePostExecute()
{
	pb->setMarqueeSpeed(false, 0);
	pb->hide();
	btt_reg->enable();
	btt_login->enable();
}

void RegistryRegistry::onMainPostExecute(Poco::JSON::Object::Ptr& json)
{
	if(json->has(VAR_JSON_MESSAGE) ) 
		MessageBox( ( ( maschinen::Window* )parent )->getHandler(), strToWstr(json->get(VAR_JSON_MESSAGE).extract<std::string>())->c_str(), L"Registration", MB_ICONINFORMATION | MB_OK );
	if ( json->has( VAR_JSON_RESULT ) )
		( ( Registry* ) parent )->setToken( strToWstr( json->get( VAR_JSON_RESULT ).extract<std::string>() )->c_str() );
}

void RegistryRegistry::setProgressBarAndBttRegAndBttLogin(maschinen::ProgressBar* pb, maschinen::Button* btt_reg, maschinen::Button* btt_login)
{
	this->pb = pb;
	this->btt_reg = btt_reg;
	this->btt_login = btt_login;
}

void RegistryRegistry::setUserDatas( LPCTSTR id, LPCTSTR username, LPCTSTR email, LPCTSTR password ) {
	this->user_id.assign( id );
	this->username.assign( username );
	this->email.assign( email );
	this->password.assign( password );
}

RegistryRegistry::~RegistryRegistry()
{
}

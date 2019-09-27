#include "stdafx.h"
#include "LoginAsynctask.h"


LoginAsynctask::LoginAsynctask()
{
}

void LoginAsynctask::onPreExecute()
{
	CustomAsyncTask::onPreExecute();
	((Login*)parent)->memberDisable();
}

void LoginAsynctask::onPrePostExecute()
{
	((Login*)parent)->memberEnable();
}

LPVOID LoginAsynctask::doInBackground( LPVOID input ) {
	maschinen::wininet::HttpParams params;
	getDefaultHttpParam( params, &uc->getPhpSessionId() );
	params.add( var_id_username_email, id_username_email, params.METHOD_POST );
	params.add( var_password, password, params.METHOD_POST );
	ATRequestAndParamCustomInternetJson temp;
	temp.request = &internet::value::request::LOGIN;
	temp.params = &params;
	return CustomInternetJSONAsyncTask::doInBackground( ( LPVOID ) &temp );
}

void LoginAsynctask::onMainPostExecute(Poco::JSON::Object::Ptr& json)
{
	if (json->has(VAR_JSON_RESULT)) {
		Poco::JSON::Object::Ptr res = json->get(VAR_JSON_RESULT).extract<Poco::JSON::Object::Ptr>();
		if (!res->has(RETURN_LOGIN_ID)) throw SeraphException(string::file::name::ERROR_LOG_NAME.c_str(), TEXT("Login proses is not compleate"), 0);
		uc->setLoginId(strToWstr(res->get(RETURN_LOGIN_ID).extract<std::string>())->c_str());
		uc->setBenutzerID( _strtoui64( res->get( RETURN_BENUTZER_ID ).extract<std::string>().c_str(), nullptr, 10 ) );
		uc->setUserName( strToWstr( res->get( RETURN_BENUTZERNAME ).extract<std::string>() )->c_str() );
		uc->setEmail( strToWstr( res->get( RETURN_EMAIL ).extract<std::string>() )->c_str() );
		uc->setGroupPrivilege( res->get( RETURN_GROUP_PRIVILEGE ).extract<int>() );
		((MainWindow*)parent->getParent())->showFileControl();
		((MainWindow*)parent->getParent())->loginSuccess();
	}
	else {
		if (json->has(VAR_JSON_MESSAGE)) MessageBox(parent->getHandler(), strToWstr(json->get(VAR_JSON_MESSAGE).extract<std::string>())->c_str(), error_marker, MB_OK | MB_ICONERROR);
		else MessageBox(parent->getHandler(), TEXT("An Unknown Error Occured"), error_marker, MB_OK | MB_ICONERROR);
	}
}

void LoginAsynctask::onPostPostExecute()
{
}

void LoginAsynctask::setDatas( LPCTSTR id_username_email, LPCTSTR password ) {
	this->id_username_email.assign( id_username_email );
	this->password.assign( password );
}


LoginAsynctask::~LoginAsynctask()
{
}

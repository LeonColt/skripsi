#include "stdafx.h"
#include "Window.h"
#include "RegistryGetBenutzerID.h"


RegistryGetBenutzerID::RegistryGetBenutzerID()
{
}

LPVOID RegistryGetBenutzerID::doInBackground(LPVOID input) {
	maschinen::wininet::HttpParams params;
	getDefaultHttpParam(&params, &uc->getPhpSessionId());
	ATRequestAndParamCustomInternetJson temp;
	temp.request = &internet::value::request::REGISTRATION_GET_ID;
	temp.params = &params;
	return CustomInternetJSONAsyncTask::doInBackground(&temp);
}

void RegistryGetBenutzerID::onMainPostExecute(Poco::JSON::Object::Ptr& json)
{
	if (!json->has(VAR_JSON_RESULT)) throw SeraphException(TEXT(" Request id cannot be empty "));
	((Registry*)parent)->setUserId(strToWstr(json->get(VAR_JSON_RESULT).extract<std::string>())->c_str());
}

void RegistryGetBenutzerID::onPostPostExecute()
{

}

RegistryGetBenutzerID::~RegistryGetBenutzerID()
{
}

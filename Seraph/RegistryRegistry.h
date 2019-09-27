#pragma once
#include "CustomInternetJSONAsyncTask.h"
#include "ProgressBar.h"
#include "Button.h"
#include "Window.h"
#include "Registry.h"
class RegistryRegistry : public CustomInternetJSONAsyncTask
{
	const tstring ID = TEXT( "cf-benutzer-884753545" );
	const tstring USERNAME = TEXT( "cf-benutzer-03493849" );
	const tstring EMAIL = TEXT( "cf-benutzer-24335454" );
	const tstring PASSWORD = TEXT( "cf-benutzer-2394394392232" );
private:
	maschinen::ProgressBar* pb = nullptr;
	maschinen::Button* btt_reg; maschinen::Button* btt_login;
	tstring user_id, username, email, password;
protected:
	void onPreExecute();
	LPVOID doInBackground( LPVOID input );
	void onPrePostExecute();
	void onMainPostExecute(Poco::JSON::Object::Ptr& json);
public:
	RegistryRegistry();
	void setProgressBarAndBttRegAndBttLogin(maschinen::ProgressBar* pb, maschinen::Button* btt_reg, maschinen::Button* btt_login);
	void setUserDatas( LPCTSTR id, LPCTSTR username, LPCTSTR email, LPCTSTR password );
	~RegistryRegistry();
};
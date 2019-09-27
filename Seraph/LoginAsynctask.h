#pragma once
#include "CustomInternetJSONAsyncTask.h"
#include "ProgressBar.h"
#include "Button.h"
#include "Edit.h"
#include "MainWindow.h"
#include "Login.h"

class LoginAsynctask : public CustomInternetJSONAsyncTask
{
private:
	const std::string RETURN_LOGIN_ID = "nmlf-l-i-483598472348927492742";
	const std::string RETURN_BENUTZER_ID = "nmlf-l-i-0485-0495-03854583058";
	const std::string RETURN_BENUTZERNAME = "nmlf-l-i-9340938492849028490284902485738924";
	const std::string RETURN_EMAIL = "nmlf-l-i-939489204820348203480234";
	const std::string RETURN_REQUEST = "nmlf-l-i-483908402742830478234728";
	const std::string RETURN_GROUP_PRIVILEGE = "nmlf-l-i-3847928749284792472398";
	tstring id_username_email, password;
protected:
	LPVOID doInBackground( LPVOID input );
public:
	LoginAsynctask();
	void onPreExecute();
	void onPrePostExecute();
	void onMainPostExecute(Poco::JSON::Object::Ptr& json);
	void onPostPostExecute();
	void setDatas( LPCTSTR id_username_email, LPCTSTR password );
	~LoginAsynctask();
};


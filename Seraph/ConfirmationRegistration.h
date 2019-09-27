#pragma once
#include "CustomInternetJSONAsyncTask.h"
class ConfirmationRegistration : public CustomInternetJSONAsyncTask {
private:
	const tstring SECURITY_TOKEN = TEXT( "cf-c-st-3482934829342942789274" );
	const tstring SECRET_KEY = TEXT( "cf-c-sk-394829347283947294789472894" );
	tstring token;
	tstring secret_key;
protected:
	LPVOID doInBackground( LPVOID input );
	void onMainPostExecute( Poco::JSON::Object::Ptr& json );
public:
	ConfirmationRegistration();
	void setToken( LPCTSTR token );
	void setSecretKey( LPCTSTR secret_key );
	~ConfirmationRegistration();
};


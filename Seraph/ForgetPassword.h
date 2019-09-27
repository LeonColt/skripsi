#pragma once
#include "CustomInternetJSONAsyncTask.h"
class ForgetPassword : public CustomInternetJSONAsyncTask {
private:
	const tstring ID_USERNAME_EMAIL;
	tstring id_username_email;
	LPVOID doInBackground( LPVOID input );
	void onMainPostExecute( Poco::JSON::Object::Ptr& json );
public:
	ForgetPassword();
	void setIdUsernameEmail( LPCTSTR id_username_email );
	~ForgetPassword();
};


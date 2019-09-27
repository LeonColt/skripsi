#pragma once
#include "CustomInternetJSONAsyncTask.h"
#include "Cloud.h"
#include "SetPrivilegeUserOnFile.h"
class SetPrivilegeUserOnFile : public CustomInternetJSONAsyncTask {
private:
	const tstring FILE_ID = TEXT( "cf-aulofp-39283901809831932" );
	const tstring USER_ID_USERNAME_EMAIL = TEXT( "cf-aulofp-3287489347983247928472" );
	const tstring TYPE = TEXT( "cf-aulofp-38972384972894728947294824" );
	const tstring TYPE_SEEN = TEXT( "cf-aulofp-348293472984782947289472894" );
	const tstring TYPE_DOWNLOAD = TEXT( "cf-aulofp-4827482374274289748247824" );
	const tstring TYPE_UPLOAD_ON_DIRECTORY = TEXT( "cf-aulofp-3472893472894789247284" );
	const tstring LOGIN_ID = TEXT( "cf-aulofp-284237427429847247298474" );

	Cloud* cloud;
	tstring user_id_username_email;
	tstring type;
protected:
	LPVOID doInBackground( LPVOID input );
	void onMainPostExecute(Poco::JSON::Object::Ptr& json);
public:
	SetPrivilegeUserOnFile();
	void setCloud( Cloud* cloud );
	void setUserIdUsernameEmail( LPCTSTR user_id_username_email );
	void setTypeSeen();
	void setTypeDownload();
	void setTypeUploadOnDirectory();
	~SetPrivilegeUserOnFile();
};


#pragma once
#include "CustomInternetJSONAsyncTask.h"
#include "CloudPropertyAbout.h"
class MarkFileAsPrivilegeByAll : public CustomInternetJSONAsyncTask {
private:
	const tstring ID = TEXT( "cf-mfapba-48948290384902849204" );
	const tstring FLAG = TEXT( "cf-mfapba-34948290480248024924" );
	const tstring TYPE = TEXT( "cf-mfapba-93489274923472947294724" );
	const tstring TYPE_SEEN = TEXT( "cf-mfapba-28389173137913791371" );
	const tstring TYPE_DOWNLOAD = TEXT( "cf-mfapba-823891371731937193" );
	const tstring TYPE_MODIFY = TEXT( "cf-mfapba-988301830183183913" );
	const tstring LOGIN_ID = TEXT( "cf-mfapba-99834934829048924824" );

	const INT SEEN, DOWNLOAD, MODIFY;
	Cloud* cloud;
	INT type;
protected:
	LPVOID doInBackground( LPVOID input );
	void onMainPostExecute( Poco::JSON::Object::Ptr& json );
	void onJsonStatusFail( Poco::JSON::Object::Ptr& json );
public:
	MarkFileAsPrivilegeByAll();
	void setCloud( Cloud* cloud );
	void setTypeSeen();
	void setTypeDownload();
	void setTypeModify();
	~MarkFileAsPrivilegeByAll();
};


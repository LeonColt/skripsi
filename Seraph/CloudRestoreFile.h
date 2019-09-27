#pragma once
#include "CustomInternetJSONAsyncTask.h"
#include "FileControl.h"
class CloudRestoreFile : public CustomInternetJSONAsyncTask {
private:
	const tstring PATHS = TEXT( "cf-rf-48390842903847589375935734" );
	const tstring LOGIN_ID = TEXT( "nmlf-rf-49394328478974375834534" );
	std::vector<tstring> sources;
protected:
	LPVOID doInBackground( LPVOID input );
	void onMainPostExecute( Poco::JSON::Object::Ptr& json );
public:
	CloudRestoreFile();
	void setSources( std::vector<tstring>& sources );
	~CloudRestoreFile();
};


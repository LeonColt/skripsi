#pragma once
#include "CustomInternetJSONAsyncTask.h"
#include "FileControl.h"
class CloudDeleteFile : public CustomInternetJSONAsyncTask {
private:
	const tstring PATHS = TEXT("cf-df-78941841841896408496419841631065841086946985048");
	const tstring LOGIN_ID = TEXT( "nmlf-fd-4948374832472947247294278429" );
	std::vector<tstring> sources;
protected:
	LPVOID doInBackground( LPVOID input );
	void onMainPostExecute( Poco::JSON::Object::Ptr& json );
public:
	CloudDeleteFile();
	void setSources( std::vector<tstring>& sources );
	~CloudDeleteFile();
};
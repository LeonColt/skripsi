#pragma once
#include "CustomInternetJSONAsyncTask.h"
#include "FileControl.h"
class DeleteCloudRecycleBinObject : public CustomInternetJSONAsyncTask {
private:
	const tstring PATHS = TEXT( "cf-ddf-34389478294728974824782349" );
	const tstring LOGIN_ID = TEXT( "cf-ddf-348294082903482094829482934" );
	std::vector<tstring> sources;
protected:
	LPVOID doInBackground( LPVOID input );
	void onMainPostExecute( Poco::JSON::Object::Ptr& json );
public:
	DeleteCloudRecycleBinObject();
	void setSources(std::vector<tstring>& sources );
	~DeleteCloudRecycleBinObject();
};


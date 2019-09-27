#pragma once
#ifndef POCO_NO_UNWINDOWS
#define POCO_NO_UNWINDOWS
#endif // !POCO_NO_UNWINDOWS
#include "CustomAsyncTask.h"
#include "UniversalMethode.h"
#include "Container.h"
struct ATRequestAndParamCustomInternetJson {
	const tstring* request = nullptr;
	maschinen::wininet::HttpParams* params = nullptr;
};
class CustomInternetJSONAsyncTask : public CustomAsyncTask {
protected:
	INT status;
	void logging( maschinen::wininet::HttpRequest* hr, tstring* result );
	LPVOID doInBackground( LPVOID input );
	void onPostExecute( LPVOID result );
	virtual void onPrePostExecute();
	virtual void onMainPostExecute( Poco::JSON::Object::Ptr& json );
	virtual void onPostPostExecute();
	virtual void onJsonStatusFail( Poco::JSON::Object::Ptr& json );
public:
	CustomInternetJSONAsyncTask();
	~CustomInternetJSONAsyncTask();
};


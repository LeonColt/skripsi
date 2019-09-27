#pragma once
#include "CustomAsyncTask.h"
#include "Container.h"
#include "Definator.h"
class CustomInternetAT : public CustomAsyncTask {
protected:
	LPVOID doInBackground( LPVOID input );
	void onPostExecute( LPVOID result );
	virtual void onPrePostExecute();
	virtual void onMainPostExecute( Poco::JSON::Object::Ptr& json );
	virtual void onPostPostExecute();
	void logging( maschinen::wininet::HttpRequest* hr, tstring* result );
public:
	CustomInternetAT();
	~CustomInternetAT();
};


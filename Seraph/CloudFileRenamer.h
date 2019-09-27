#pragma once
#include "CustomInternetJSONAsyncTask.h"
#include "FileControl.h"
class CloudFileRenamer : public CustomInternetJSONAsyncTask {
private:
	const tstring ID = TEXT( "nmlf-fr-9349380248932089024890238424" );
	const tstring NEW_NAME = TEXT( "nmlf-fr-9439089085947890389038459034" );
	QWORD id;
	tstring new_name;
protected:
	LPVOID doInBackground( LPVOID input );
	void onMainPostExecute( Poco::JSON::Object::Ptr& json );
public:
	CloudFileRenamer( QWORD id, LPCTSTR new_name );
	~CloudFileRenamer();
};


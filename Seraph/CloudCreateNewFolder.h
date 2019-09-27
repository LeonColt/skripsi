#pragma once
#include "CustomInternetJSONAsyncTask.h"
#include "FileControl.h"
class CloudCreateNewFolder : public CustomInternetJSONAsyncTask {
private:
	const tstring PARENT = TEXT( "nmlf-cnf-02439085824902849375824" );
	//const tstring NAME = "nmlf-cnf-304830957898403504909729849545";
	const tstring LOGIN_ID = TEXT( "nmlf-cnf-439078974984728947284792" );
	QWORD parent_id;
protected:
	LPVOID doInBackground( LPVOID input );
	void onMainPostExecute( Poco::JSON::Object::Ptr& json );
public:
	CloudCreateNewFolder();
	void setParentId( QWORD parent_id );
	~CloudCreateNewFolder();
};


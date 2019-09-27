#pragma once
#include "CustomInternetJSONAsyncTask.h"
#include "FileControl.h"
class CloudMoveFile : public CustomInternetJSONAsyncTask {
	const tstring FROMS = TEXT( "nmlf-fms-4387829749832749824" );
	const tstring TO = TEXT( "nmlf-fms-949357028974982734982749" );
	const tstring BENUTZER = TEXT( "nmlf-fms-5804985093284274247" );
private:
	std::vector<QWORD> froms;
	QWORD to;
protected:
	LPVOID doInBackground( LPVOID input );
	void onMainPostExecute( Poco::JSON::Object::Ptr& json );
public:
	CloudMoveFile();
	void setFroms( std::vector<QWORD> froms ) noexcept;
	void setTo( QWORD to ) noexcept;
	~CloudMoveFile();
};


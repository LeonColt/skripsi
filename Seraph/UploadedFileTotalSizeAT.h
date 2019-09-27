#pragma once
#include "CustomInternetJSONAsyncTask.h"
#include "FileUploadProcessWindow.h"
class FileUploadProcessWindow;
class UploadedFileTotalSizeAT : public CustomInternetJSONAsyncTask {
private:
	const tstring ID = TEXT( "nmlf-futus-348938594087357398753985" );
	const tstring LOGIN_ID = TEXT( "nmlf-futus-87489375934583530957982764821365135176214" );
	tstring id;
protected:
	void onPreExecute();
	void onMainPostExecute( Poco::JSON::Object::Ptr& json );
	LPVOID doInBackground( LPVOID input );
	void onJsonStatusFail( Poco::JSON::Object::Ptr& json );
public:
	UploadedFileTotalSizeAT( );
	void setUploadId( LPCTSTR id) noexcept;
	~UploadedFileTotalSizeAT();
};


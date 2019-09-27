#pragma once
#include "Cloud.h"
#include "FileControl.h"
#include "CustomInternetJSONAsyncTask.h"
class CutFileCloudRequest : public CustomInternetJSONAsyncTask {
private:
	const tstring FROMS = TEXT( "nmlf-fms-4387829749832749824" );
	const tstring TO = TEXT( "nmlf-fms-949357028974982734982749" );
	const tstring LOGIN_ID = TEXT( "nmlf-fms-5804985093284274247" );

	tstring target;
	std::list<tstring> sources;
protected:
	LPVOID doInBackground( LPVOID input );
	void onMainPostExecute( Poco::JSON::Object::Ptr& json );
public:
	CutFileCloudRequest();
	void setTarget( LPCTSTR target ) noexcept;
	void setTarget( tstring& target ) noexcept;
	void setTarget( tstring* target ) noexcept;
	void addSources( LPCTSTR cloud ) noexcept;
	void addSources( tstring& cloud ) noexcept;
	void addSources( tstring* cloud ) noexcept;
	~CutFileCloudRequest();
};


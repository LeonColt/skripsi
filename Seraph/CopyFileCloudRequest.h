#pragma once
#include "CustomInternetJSONAsyncTask.h"
#include "Cloud.h"
#include "FileControl.h"
class CopyFileCloudRequest : public CustomInternetJSONAsyncTask {
private:
	const tstring SOURCES = TEXT( "nmlf-fcpy-934092849274720948920842904" );
	const tstring TARGET = TEXT( "nmlf-fcpy-4983053984309482094728472834" );
	const tstring LOGIN_ID = TEXT( "nmlf-fcpy-348032489324890842908402948924" );

	tstring target;
	std::list<tstring> sources;
protected:
	LPVOID doInBackground( LPVOID input );
	void onMainPostExecute( Poco::JSON::Object::Ptr& json );
public:
	CopyFileCloudRequest();
	void setTarget( LPCTSTR target ) noexcept;
	void setTarget( tstring& target ) noexcept;
	void setTarget( tstring* target ) noexcept;
	void addSources( LPCTSTR cloud ) noexcept;
	void addSources( tstring& cloud ) noexcept;
	void addSources( tstring* cloud ) noexcept;
	~CopyFileCloudRequest();
};


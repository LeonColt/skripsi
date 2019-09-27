#pragma once
#include <ShlObj.h>
#include "FileControl.h"
#include "resources.h"
#include "HttpParams.h"
#include "MainWindow.h"
#include "FileCloud.h"
#include "CustomInternetJSONAsyncTask.h"
#include "FileHandler.h"
#include "Local.h"
class DownloadRequest : public CustomInternetJSONAsyncTask
{
private:
	const tstring LOGIN_ID = TEXT( "cf-dfb-8987585585" );
	const tstring PATHS = TEXT( "cf-dfb-23433434" );
	const tstring AS_ZIP = TEXT( "cf-dfb-93937589473598" );

	const std::string KEMBALIAN_ID = "cf-fdbk-3989375894765894";
	const std::string KEMBALIAN_FILES = "cf-fdbk-39589407948758937534985";
	const std::string KEMBALIAN_FILES_ID = "cf-fdbk-34948957489679837598423";
	const std::string KEMBALIAN_FILES_NAME = "cf-fdbk-name";
	const std::string KEMBALIAN_FILES_SIZE = "cf-fdbk-size";
	const std::string KEMBALIAN_FILES_DIRECTORY = "cf-fdbk-directory"; //bool
	const std::string KEMBALIAN_FILES_PARENT = "cf-fdbk-parent";
	const std::string KEMBALIAN_FILES_ORIGINAL = "cf-fdbk-original";
	tstring target;
	std::list<tstring> sources;
protected:
	void onPreExecute();
	LPVOID doInBackground(LPVOID input);
	void onPrePostExecute();
	void onMainPostExecute(Poco::JSON::Object::Ptr& json);
	void onPostPostExecute();
public:
	DownloadRequest();
	void setTarget(LPCTSTR target) noexcept;
	void setTarget( tstring& target ) noexcept;
	void setTarget( tstring* target ) noexcept;
	void addSources( LPCTSTR cloud ) noexcept;
	void addSources( tstring& cloud ) noexcept;
	void addSources( tstring* cloud ) noexcept;
	~DownloadRequest();
};


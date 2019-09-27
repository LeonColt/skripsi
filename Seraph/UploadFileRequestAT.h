/*
1. request to server for upload request
2. caching file
3. write meta data
4. open FileUploadList
*/

#pragma once
#include "CustomInternetJSONAsyncTask.h"
#include "DirectoryCloud.h"
#include "Local.h"
#include "FileLocal.h"
#include "DirectoryLocal.h"
#include "MetaDataUpload.h"
#include "MainWindow.h"
#include "MetaDataUploadDetail.h"
#include "FileUniversalMethode.h"
struct SourcesPathReminder {
	Local* local = nullptr;
	UINT cid = 0;
};
class UploadFileRequestAT : public CustomInternetJSONAsyncTask {
private:
	const tstring FILES = TEXT( "nmlf-fub-29893840237498354387593485374" );
	const tstring LOGIN_ID = TEXT( "nmlf-fub-4945098294345820420492358047503573985" );
	const tstring PARENT = TEXT( "nmlf-fub-398350948603480385093485034753957935" );

	const std::string INPUT_FILES = "nmlf-fub-i-9380395809347503495734";
	const std::string INPUT_FILES_NAME = "nmlf-fub-i-5803495808504937503947U5";
	const std::string INPUT_FILES_IS_DIRECTORY = "nmlf-fub-i-34829035809574398573495";
	const std::string INPUT_FILES_SIZE = "nmlf-fub-i-043905893475893579385";
	const std::string INPUT_FILES_CID = "nmlf-fub-i-495893045739874293742984723984";
	const std::string INPUT_FILES_CHILDREN = "nmlf-fub-i-34398579384759384759843759375983475938";

	const std::string RETURN_ID = "id";
	const std::string RETURN_FILES = "Files";
	const std::string RETURN_FILES_ID = "File Id";
	const std::string RETURN_FILES_NAME = "File Name";
	const std::string RETURN_FILES_PARENT = "File Parent";
	const std::string RETURN_FILES_CID = "File cid";

	tstring target;
	std::list<tstring> sources;
	std::vector<SourcesPathReminder> result_sources;
	void transformFilesToJson( Poco::JSON::Array& json, LPCTSTR path, UINT& cid ) noexcept;
protected:
	LPVOID doInBackground( LPVOID input );
	void onPrePostExecute();
	void onMainPostExecute( Poco::JSON::Object::Ptr& json );
	void onPostPostExecute();
public:
	UploadFileRequestAT();
	void setTarget( LPCTSTR target ) noexcept;
	void setTarget( tstring& target ) noexcept;
	void setTarget( tstring* target ) noexcept;
	void addSources( LPCTSTR cloud ) noexcept;
	void addSources( tstring& cloud ) noexcept;
	void addSources( tstring* cloud ) noexcept;
	~UploadFileRequestAT();
};


#pragma once
#include <memory>
#include <thread>
#include <chrono>
#include "CustomInternetJSONAsyncTask.h"
#include "FileControl.h"
#include "Cloud.h"
#include "DirectoryCloud.h"
#include "FileCloud.h"
#include "OnListViewSelectEvent.h"
#include "CloudCreateNewFolder.h"
#include "FileHandleAsynchronous.h"
#include "AnonymoudPipe.h"
#include "CloudFileRenamer.h"
class CloudFileOperator : public CustomInternetJSONAsyncTask {
private:
	const std::string GET_FILE_ID = "cf-gafsaf-8789955585897959"; //string then cast to UINT64
	const std::string GET_FILE_NAME = "cf-gafsaf-7864051513135464313"; //string
	const std::string GET_FILE_SIZE = "cf-gafsaf-8485964928658996349252"; //string then cast to UINT64
	const std::string GET_FILE_OWNER = "cf-gafsaf-8589649621635825420032"; //string
	const std::string GET_FILE_PARENT = "cf-gafsaf-02850205349037403"; //string then cast to UINT64
	const std::string GET_FILE_SEEN_BY_ALL = "cf-gafsaf-3874982742936427423";
	const std::string GET_FILE_DOWNLOAD_BY_ALL = "cf-gafsaf-995-0350937853953953905834";
	const std::string GET_FILE_MODIFY_BY_ALL = "cf-gafsaf-328585874892374289472";
	const std::string GET_FILE_DOWNLOAD_ALLOWED_CURRENT_USER = "cf-gafsaf-439048028490238402489204";//bool
	const std::string GET_FILE_UPLOAD_ON_DIRECTORY_ALLOWED_CURRENT_USER = "cf-gafsaf-349823904823849203482904";//bool
	const std::string GET_FILE_DELETE_ALLOWED_CURRENT_USER = "cf-gafsaf-39420948238420482084294";//bool
	const std::string GET_FILE_IS_DIRECTORY = "cf-gafsaf-4983209482394723894723742"; //bool

	const std::string GET_RECYCLE_BIN_ID = "cf-grb-4343254543534424234"; //string then cast to UINT64
	const std::string GET_RECYCLE_BIN_NAME = "cf-grb-3435465643534343224"; //string
	const std::string GET_RECYCLE_BIN_DIRECTORY = "cf-grb-9483209482904824"; //bool
	const std::string GET_RECYCLE_BIN_SIZE = "cf-grb-3843420948490380"; //string then cast to UINT64
	const std::string GET_RECYCLE_BIN_OWNER = "cf-grb-83904809238427487429824"; //string

	tstring result;
	bool first_load = true;
	bool flag_stop = false;
	HANDLE waiter = NULL;
	std::unique_ptr<maschinen::pipe::AnonymoudPipe> pipe;
	bool flag_cancelling;
	tstring cloud_search_name;

	void setCloudProperty(Cloud*& cloud, Poco::JSON::Object::Ptr& json, DirectoryCloud* parent);
protected:
	void onPreExecute();
	LPVOID doInBackground( LPVOID input );
	void onProgressUpdate( LPVOID progress, bool on_task );
	void onPostExecute( LPVOID result );
public:
	CloudFileOperator();
	void createNewFolder( DirectoryCloud* dc ) const;
	void renameFile(QWORD id, LPCTSTR new_name) const noexcept;
	void restart();
	void stop();
	bool isStopped() const noexcept;
	void setSearchName( LPCTSTR cloud_search_name );
	~CloudFileOperator();
};


#pragma once
#include <thread>
#include <chrono>
#include "CustomInternetJSONAsyncTask.h"
#include "AnonymoudPipe.h"
#include "Cloud.h"
#include "CloudPropertyPrivilegeList.h"
#include "User.h"

class CloudPropertyPrivilegeList;
class GetPrivilegeList : public CustomInternetJSONAsyncTask {
	const tstring TYPE_SEEN, TYPE_DOWNLOAD, TYPE_UPLOAD_ON_DIRECTORY;
	Cloud* cloud;
	const std::string ID = "id";
	const std::string USERNAME = "username";

	HANDLE waiter = NULL;
	bool flag_stop, first_load;
	std::unique_ptr<maschinen::pipe::AnonymoudPipe> pipe;

	std::vector<std::unique_ptr<User>> users;

	tstring type;
protected:
	LPVOID doInBackground( LPVOID input );
	void onProgressUpdate( LPVOID progress, bool on_task );
	void onPostExecute( LPVOID result );
public:
	GetPrivilegeList();
	void setCloud( Cloud* cloud );
	void stop();
	std::vector<std::unique_ptr<User>>::const_iterator begin();
	std::vector<std::unique_ptr<User>>::const_iterator end();
	void setTypeSeen();
	void setTypeDownload();
	void setTypeUploadOnDirectory();
	~GetPrivilegeList();
};


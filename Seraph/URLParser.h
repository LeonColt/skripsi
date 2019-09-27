#pragma once
#define POCO_NO_UNWINDOWS
#include <list>
#include "CustomAsyncTask.h"
#include "BaseFile.h"
#include "DriveFixed.h"
#include "DriveRemovable.h"
#include "FileLocal.h"
#include "FileControl.h"
class URLParser : public CustomAsyncTask  {
private:
	const std::string GET_FILE_ID = "cf-gafsaf-8789955585897959"; //string then cast to UINT64
	const std::string GET_FILE_NAME = "cf-gafsaf-7864051513135464313"; //string
	const std::string GET_FILE_SIZE = "cf-gafsaf-8485964928658996349252"; //string then cast to UINT64
	const std::string GET_FILE_OWNER = "cf-gafsaf-8589649621635825420032"; //string
	const std::string GET_FILE_EXISTS = "cf-gafsaf-02850205954057947"; //bool
	const std::string GET_FILE_PARENT = "cf-gafsaf-02850205349037403"; //string then cast to UINT64
	const std::string GET_FILE_SEEN_BY_ALL = "cf-gafsaf-3874982742936427423"; //bool
	const std::string GET_FILE_DOWNLOAD_BY_ALL = "cf-gafsaf-995-0350937853953953905834";//bool
	const std::string GET_FILE_MODIFY_BY_ALL = "cf-gafsaf-328585874892374289472";//bool
	const std::string GET_FILE_DOWNLOAD_ALLOWED_CURRENT_USER = "cf-gafsaf-439048028490238402489204";//bool
	const std::string GET_FILE_UPLOAD_ON_DIRECTORY_ALLOWED_CURRENT_USER = "cf-gafsaf-349823904823849203482904";//bool
	const std::string GET_FILE_DELETE_ALLOWED_CURRENT_USER = "cf-gafsaf-39420948238420482084294";//bool
	const std::string GET_FILE_IS_DIRECTORY = "cf-gafsaf-4983209482394723894723742"; //bool
	const std::string GET_FILE_DATE_CREATED = "cf-gafsaf-3942948248242949204"; //string

	BaseFile* pointer;
	const INT LOCAL, CLOUD;
	INT type;
	INT decideType( std::vector<tstring>& paths );
	bool flag_cancelling;
	std::unique_ptr<maschinen::pipe::AnonymoudPipe> pipe;
	INT status;
protected:
	LPVOID doInBackground( LPVOID input );
	void onPostExecute( LPVOID input );
public:
	URLParser();
	~URLParser();
};


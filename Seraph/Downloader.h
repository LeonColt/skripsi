#pragma once
#include "MetaDataDownload.h"
#include "CustomAsyncTask.h"
#include "FileProcessWindow.h"
struct DownloaderResult {
	const DWORD STATUS_PAUSE = 1;
	const DWORD STATUS_FINISH = 2;
	const DWORD STATUS_ERROR = 3;
	DWORD status = 0;
	tstring message;
	SeraphException* ex = nullptr;
};
class MetaDataDownload;
class Downloader : public CustomAsyncTask {
private:
	const tstring DOWNLOAD_ID = TEXT( "cf-df-0432434675567" );
	const tstring FILE_ID = TEXT( "cf-df-39859759837509274892589325" );
	const DWORD BUFFER_SIZE;
	MetaDataDownload* meta_data = nullptr;
	bool flag_pause = false;
protected:
	void onPreExecute();
	LPVOID doInBackground( LPVOID input );
	void onPostExecute( LPVOID result );
public:
	Downloader(const DWORD buffer_size);
	void setMetaData(MetaDataDownload* meta_data);
	void pause();
	~Downloader();
};
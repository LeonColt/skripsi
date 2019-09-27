#pragma once
#include "CustomInternetJSONAsyncTask.h"
#include "MetaDataUpload.h"
#include "Crypt.h"
#include "MetaDataUploadDetail.h"
#include "HttpHeaders.h"
struct UploaderResult {
	const DWORD STATUS_PAUSE = 1;
	const DWORD STATUS_FINISH = 2;
	const DWORD STATUS_ERROR = 3;
	DWORD status = 0;
	tstring message;
};
class MetaDataUpload;
class Uploader : public CustomInternetJSONAsyncTask {
	const std::string ID ="cf-uf-3445693958694505465789487888796546484";
	const std::string FILE_ID = "cf-uf-983094928403297585794035809348039485092292";
	const std::string FILE_SIZE = "cf-uf-8798465447846416512136048048484848";
	const std::string THE_FILE = "cf-uf-858898584995455968375897598793857924131";
	const std::string LOGIN_ID = "cf-uf-879343757729479247950820482797498";

	struct FileSizeVariable { //File Size
		const tstring ID = TEXT( "nmlf-fugufs-489375893479843759375935" );
		const tstring FILE_ID = TEXT( "nmlf-fugufs-48608503480749274923792" );
		const tstring LOGIN_ID = TEXT( "nmlf-fugufs-938508509485092840923842094" );
	} FileSize;
private:
	const DWORD BUFFER_MULTIPLER_TO_CHUNK = 10;
	const DWORD BUFFER_SIZE;
	MetaDataUpload* meta_data = nullptr;
	bool flag_pause = false;
protected:
	void onPreExecute();
	LPVOID doInBackground( LPVOID input );
	void onPostExecute( LPVOID result );
	std::unique_ptr<LARGE_INTEGER> getFileSizeUploaded( const MetaDataUploadDetail& mdud );
	bool reportFileUploadFinish();
	void appendSendingData( std::string& data, std::string& boundary, QWORD size, const MetaDataUploadDetail& mdud );
public:
	Uploader(const DWORD buffer_size);
	void setMetaData( MetaDataUpload* meta_data );
	void pause();
	~Uploader();
};


#pragma once
#include "CustomAsyncTask.h"
#include "HttpDownloadFile.h"
#include "UniversalMethode.h"
#include "UserCredential.h"
#include "FileDownloadMetaData.h"
class FileDownloadWindowDownloader : public CustomAsyncTask {
	LONGLONG byte_start, byte_end;
	tstring path, download_id, file_meta_name;
	TCHAR error[MAX_LOADSTRING];
	LONGLONG file_size;
	QWORD file_id;
	maschinen::communicator::HttpDownloadFile hdf;
	void onPreExecute();
	LPVOID doInBackground(LPVOID input);
	void onPostExecute(LPVOID result);
public:
	BOOL status_finish_download = FALSE;

	FileDownloadWindowDownloader();
	void setByteStart( LONGLONG byte_start ); 
	void setByteEnd(LONGLONG byte_end);
	void setPath(LPCTSTR path);
	void setDownloadId(LPCTSTR download_id);
	void setFileSize(LONGLONG file_size);
	void setFileMetaName( LPCTSTR file_meta_name );
	~FileDownloadWindowDownloader();
};
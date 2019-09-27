#pragma once
#ifndef POCO_NO_UNWINDOWS
#define POCO_NO_UNWINDOWS
#endif // !POCO_NO_UNWINDOWS
#include <Shlwapi.h>
#include "CustomAsyncTask.h"
#include "CloudFileOperator.h"
#include "DownloadRequest.h"
#include "UploadFileRequestAT.h"
#include "ComponentObjectModel.h"
#include "FileOperation.h"
#include "FileControl.h"
#include "CloudDeleteFile.h"
#include "CopyFileCloudRequest.h"
#include "CutFileCloudRequest.h"
#include "CloudRestoreFile.h"
#include "DeleteCloudRecycleBinObject.h"
class FileControl;
class UploadFileRequestAT;
class FileControlDownloadRequest;
class CloudFileOperator;
class FileExecutor {
private:
	Akey* akey;
	Setting* setting;
	UserCredential* uc;
	FileControl* fc;
	CloudFileOperator* cloud;
	tstring target;
	std::list<tstring> sources;
	const tstring ERROR_MARKER;
	DWORD operation = 0;
	const DWORD OPERATION_NULL, OPERATION_CUT, OPERATION_COPY, OPERATION_DELETE, OPERATION_RESTORE;
	const INT CLOUD, LOCAL, CLOUD_BIN;
	void copy() const;
	void cut() const;
	void deleteFile() const;
	void restoreFile() const;

	INT determineType( LPCTSTR path ) const noexcept;
public:
	FileExecutor( FileControl* const fc, LPCTSTR error_marker, Akey* akey, Setting* setting, UserCredential* uc);
	void execute();
	void setTarget( LPCTSTR target ) noexcept;
	void addSource( LPCTSTR source ) noexcept;
	void cleanSource() noexcept;
	void operationCopy();
	void operationCut();
	void operationDelete();
	void operationRestore();
	~FileExecutor();
};


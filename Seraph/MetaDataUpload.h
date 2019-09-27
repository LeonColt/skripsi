#pragma once
#include "MetaData.h"
#include "Uploader.h"
#include "DirectoryHandler.h"
#include "MetaDataDoesNotMatchException.h"
#include "FileUploadProcessWindow.h"
#include "MetaDataUploadDetail.h"
class Uploader;
class FileUploadProcessWindow;
class MetaDataUpload : public MetaData {
private:
	const std::string DISPOSITION_VALUE = "-";
	const std::string FILES = "FILES";
	const std::string FILES_ID = "ID";
	const std::string FILES_NAME = "NAME";
	const std::string FILES_PATH = "PATH";
	const std::string FILES_AS_DIRECTORY = "AS_DIRECTORY";
	const std::string FINISHING_PROCESS = "FINISH_PROCESS";

	std::unique_ptr<Uploader> uploader;
	std::vector<MetaDataUploadDetail> datas;
	FileUploadProcessWindow* ui = nullptr;
	tstring meta_path;
public:
	MetaDataUpload();
	void startUpload( const DWORD buffer_size );
	void pause();
	void write( std::vector<MetaDataUploadDetail>& files_real_path );
	void read(LPCTSTR meta_name);
	virtual std::unique_ptr<tstring> getTargetName();
	std::unique_ptr<tstring> getCacheFileUploadDirectoryName() const noexcept;
	void setUI( FileUploadProcessWindow* ui ) noexcept;
	void setFiles( std::vector<MetaDataUploadDetail>& datas );
	std::vector<MetaDataUploadDetail>::const_iterator begin() const noexcept;
	std::vector<MetaDataUploadDetail>::const_iterator end() const noexcept;
	QWORD getSize() const;
	void deleteCache();
	void deleteMetaDataFile();
	~MetaDataUpload();
};


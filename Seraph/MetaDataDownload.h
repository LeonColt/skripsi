#pragma once
#include "MetaData.h"
#include "Downloader.h"
#include "MetaDataDoesNotMatchException.h"
#include "MetaDataDownloadBaseFile.h"
#include "MetaDataDownloadDirectory.h"
#include "MetaDataDownloadFile.h"
class Downloader;
class FileProcessWindow;
class MetaDataDownload : public MetaData {
	const std::string DISPOSITION_VALUE = "+";
	const std::string VAR_TARGET_PATH = "TARGET_PATH";
	const std::string FILES = "FILES";
	const std::string FILES_ID = "ID";
	const std::string FILES_NAME = "NAME";
	const std::string FILES_SIZE = "SIZE";
	const std::string FILES_PARENT = "PARENT";
	const std::string FILES_ORIGINAL = "ORIGINAL";
	const std::string FILES_DIRECTORY = "DIRECTORY";
private:
	std::unique_ptr<Downloader> downloader;
	tstring target_path;
	std::unique_ptr<MetaDataDownloadDirectory> root;
	FileProcessWindow* ui = nullptr;
	void MetaDataToJsonArray( Poco::JSON::Array* arr, MetaDataDownloadDirectory* dir);
public:
	MetaDataDownload();
	void startDownload( const DWORD buffer_size );
	void pause();
	void cancel();
	void write();
	void read(LPCTSTR meta_name);
	void addFile(QWORD id, LPCTSTR name, ULONGLONG size, bool is_directory, QWORD parent_id, bool original);
	void setUI( FileProcessWindow* ui );
	void remove() const;
	std::unique_ptr<tstring> getFilePathByFileId(QWORD file_id);
	ULONGLONG getSize() const noexcept;
	ULONGLONG getByteWritten() const noexcept;

	//begin set
	void setTargetPath( LPCTSTR target_path ) noexcept;
	void setTargetPath( tstring& target_path ) noexcept;
	void setTargetPath( tstring* target_path ) noexcept;
	//end set

	//begin get
	void filesToVector( std::vector<MetaDataDownloadBaseFile*>& files, MetaDataDownloadBaseFile* start_position = nullptr ) const noexcept;
	MetaDataDownloadDirectory* getFiles() const noexcept;
	std::unique_ptr<tstring> getTargetPath() const noexcept;
	MetaDataDownloadBaseFile* getFileById( QWORD id, MetaDataDownloadBaseFile* start_position = nullptr ) const noexcept;
	ULONGLONG getByteWritten( MetaDataDownloadBaseFile* meta_data );
	ULONGLONG getSize( MetaDataDownloadBaseFile* meta_data );
	ULONGLONG getSize( QWORD id ) const noexcept;
	ULONGLONG getByteWritten( QWORD id ) const noexcept;

	bool isFinished() const noexcept;
	void deleteFiles() const noexcept;
	//end get
	~MetaDataDownload();
};


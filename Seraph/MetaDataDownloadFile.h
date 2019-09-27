#pragma once
#include "MetaDataDownloadBaseFile.h"
class MetaDataDownloadFile : public MetaDataDownloadBaseFile {
public:
	MetaDataDownloadFile();
	std::unique_ptr<tstring> getPath() noexcept;
	~MetaDataDownloadFile();
};


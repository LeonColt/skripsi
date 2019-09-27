#pragma once
#include "MetaDataDownloadBaseFile.h"
class MetaDataDownloadDirectory : public MetaDataDownloadBaseFile {
	std::vector<MetaDataDownloadBaseFile*> children;
public:
	MetaDataDownloadDirectory();
	void addChildren( MetaDataDownloadBaseFile* meta_data );
	std::vector<MetaDataDownloadBaseFile*>::const_iterator begin() const noexcept;
	std::vector<MetaDataDownloadBaseFile*>::const_iterator end() const noexcept;
	std::unique_ptr<tstring> getPath() noexcept;
	bool empty() const noexcept;
	void clear() noexcept;
	~MetaDataDownloadDirectory();
};


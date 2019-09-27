#pragma once
#include "DirectoryLocal.h"
#include "ComponentObjectModel.h"
#include "FileOperation.h"
class SERAPHCOMMON_API Drive : public Local {
private:
	maschinen::file::DirectoryHandler dh;
	maschinen::file::DirectoryReadingIterator dri;
public:
	typedef LocalIterator iterator;
	Drive();
	std::unique_ptr<tstring> getVolumeName();
	std::unique_ptr<tstring> getFileSystemName();
	DWORD getVolumeSerialNumber();
	std::unique_ptr<tstring> getOwner();
	ULONGLONG getSize() const noexcept;
	bool isDeletable() const noexcept;
	bool isOpenable() const noexcept;
	bool isMovable() const noexcept { return false; }
	bool isDuplicatable() const noexcept { return false; }
	void createNewDirectory() const;

	bool hasChildren();
	LocalIterator begin();
	LocalIterator end();

	~Drive();
};


#pragma once
#include <vector>
#include "Local.h"
#include "FileHandler.h"
#include "FileTypeMapping.h"

class SERAPHCOMMON_API FileLocal : public Local {
private:
	maschinen::file::FileHandler handler;
protected:
	LONGLONG size;
public:
	const DWORD SHARE_DEFAULT, SHARE_DELETE, SHARE_READ, SHARE_WRITE;
	const DWORD DISPOSITION_ALWAYS, DISPOSITION_NEW, DISPOSITION_OPEN_ALWAYS, DISPOSITION_OPEN_EXISTING, DISPOSITION_TRUNCATE_EXISTING;

	FileLocal();
	ULONGLONG getSize() const;
	std::unique_ptr<tstring> getOwner();
	std::unique_ptr<tstring> getType();
	std::unique_ptr<FILETIME> getCreatedTime() const;
	std::unique_ptr<FILETIME> getLastAccessTime() const;
	std::unique_ptr<FILETIME> getLastWriteTime() const;
	bool isDeletable() const noexcept { return true; }
	bool isOpenable() const noexcept { return true; }
	bool isMovable() const noexcept { return true; }
	bool isDuplicatable() const noexcept { return true; }

	//begin comparing file
	virtual bool operator==( FileLocal& file );
	virtual bool operator==( FileLocal* file );
	virtual bool equal( FileLocal& file );
	virtual bool equal( FileLocal* file );
	//end comparing file

	bool exists();
	~FileLocal();
};


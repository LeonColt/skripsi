#pragma once
#include "BaseFile.h"
#include "SeraphException.h"
class SERAPHCOMMON_API CloudRecycleBinObject : public BaseFile {
	QWORD id;
	tstring owner;
	bool flag_directory;
	tstring date_deleted;
public:
	CloudRecycleBinObject();
	void setId( QWORD id ) noexcept;
	QWORD getId() const noexcept;
	std::unique_ptr<tstring> getPath() const noexcept;
	void setOwner( LPCTSTR owner );
	void setOwner( tstring& owner );
	void setOwner( tstring* owner );
	std::unique_ptr<tstring> getOwner() const noexcept;
	void setAsDirectory(bool flag_directory);
	bool isDirectory() const noexcept;
	std::unique_ptr<tstring> getType() const noexcept;
	void setDateDeleted( tstring date_deleted ) noexcept;
	std::unique_ptr<tstring> getDateDeleted() const noexcept;
	~CloudRecycleBinObject();
};
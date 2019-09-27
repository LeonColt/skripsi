#pragma once
#include "BaseFile.h"
#include "SeraphException.h"
#include "DirectoryHandler.h"
class SERAPHCOMMON_API Local : public BaseFile {
private:
	maschinen::file::DirectoryHandler* dh;
public:
	Local();
	virtual bool isDuplicatable() const noexcept = 0;
	virtual std::unique_ptr<tstring> getOwner() = 0;
	virtual bool isDeletable() const noexcept = 0;
	virtual bool isOpenable() const noexcept = 0;
	virtual bool isMovable() const noexcept = 0;
	virtual ULONGLONG getSize() const = 0;
	void rename( LPCTSTR new_name ) const;
	bool compare(Local* local);
	bool compare( Local& local );
	bool operator==( Local* local );
	bool operator==( Local& local );
	~Local();
};


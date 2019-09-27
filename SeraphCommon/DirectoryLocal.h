#pragma once
#include <memory>
#include <Shlwapi.h>
#include "Local.h"
#include "DirectoryHandler.h"
#include "ComponentObjectModel.h"
#include "FileOperation.h"
#include "LocalIterator.h"
class LocalIterator;
class SERAPHCOMMON_API DirectoryLocal : public Local {
private:
	maschinen::file::DirectoryHandler dh;
	maschinen::file::DirectoryReadingIterator dri;
public:
	typedef LocalIterator iterator;
	DirectoryLocal();
	ULONGLONG getSize() const;
	virtual std::unique_ptr<tstring> getOwner();
	bool hasChildren();
	LocalIterator begin();
	LocalIterator end();
	virtual bool isDeletable() const noexcept { return true; }
	virtual bool isOpenable() const noexcept { return true; }
	virtual bool isMovable() const noexcept { return true; }
	virtual bool isDuplicatable() const noexcept { return true; }
	void createNewDirectory() const;
	void deleteMember( Local* file ) const;
	void deleteMember( std::vector<Local*>& files ) const;
	void copy( Local* source ) const;
	void copy( std::vector<Local*>& sources ) const;
	void move( Local* source ) const;
	void move( std::vector<Local*>& sources ) const;

	~DirectoryLocal();
};


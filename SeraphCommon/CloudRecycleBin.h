#pragma once
#include "BaseFile.h"
#include "CloudRecycleBinObject.h"
class SERAPHCOMMON_API CloudRecycleBin : public BaseFile {
private:
	std::list<BaseFile*> children;
public:
	CloudRecycleBin();
	std::list<BaseFile*>::const_iterator begin() const noexcept;
	std::list<BaseFile*>::const_iterator end() const noexcept;
	bool hasAnyChildren() const noexcept;
	void addChildren( BaseFile* child );
	~CloudRecycleBin();
};


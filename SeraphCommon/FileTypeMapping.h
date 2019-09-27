#pragma once
#include "SeraphCommon.h"
#include "GlobalType.h"
#include "UniversalMethode.h"

class SERAPHCOMMON_API FileTypeMapping {
	struct FILE_TYPES {
		tstring format;
		tstring name;
	};
private:
	std::vector<FILE_TYPES> types;
public:
	FileTypeMapping();
	bool exists();
	void read();
	std::unique_ptr<tstring> getType( LPCTSTR file_name );
	std::unique_ptr<tstring> getPath();
	~FileTypeMapping();
};


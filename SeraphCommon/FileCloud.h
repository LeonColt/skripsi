#pragma once
#include "Cloud.h"
class SERAPHCOMMON_API FileCloud : public Cloud {
public:
	FileCloud();

	std::unique_ptr<tstring> getType() const noexcept;

	/*
	//begin comparing file
	virtual bool operator==( FileCloud& file );
	virtual bool operator==( FileCloud* file );
	virtual bool equal( FileCloud& file );
	virtual bool equal( FileCloud* file );
	//end comparing file
	*/

	~FileCloud();
};


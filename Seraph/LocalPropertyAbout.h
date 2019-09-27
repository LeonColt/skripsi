#pragma once
#include "CustomContainer.h"
#include "resources.h"
#include "Local.h"
#include "FileLocal.h"
class LocalPropertyAbout : public CustomContainer {
private:
	Local* local;

	maschinen::Label label_name;
	maschinen::Edit edit_name;

	maschinen::Label label_owner_var;
	maschinen::Label label_owner_val;

	maschinen::Label label_path_var;
	maschinen::Label label_path_val;

	maschinen::Label date_created_var;
	maschinen::Label date_created_val;

	maschinen::Label last_access_time_var;
	maschinen::Label last_access_time_val;

	maschinen::Label last_write_time_var;
	maschinen::Label last_write_time_val;

	std::unique_ptr<tstring> extractTimes(LPSYSTEMTIME time);
protected:
	void onCreate();
public:
	LocalPropertyAbout();
	void setLocal( Local* local );
	~LocalPropertyAbout();
};


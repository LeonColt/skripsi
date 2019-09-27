#pragma once
#include "CustomContainer.h"
#include "CloudRecycleBinObject.h"
class CloudRecycleBinPropertyAbout : public CustomContainer {
private:
	CloudRecycleBinObject* crbo;

	maschinen::Label label_name;
	maschinen::Edit edit_name;

	maschinen::Label label_owner_var;
	maschinen::Label label_owner_val;

	maschinen::Label label_parent_var;
	maschinen::Label label_parent_val;

	maschinen::Label date_created_var;
	maschinen::Label date_created_val;
protected:
	void onCreate();
public:
	CloudRecycleBinPropertyAbout();
	void setCloudRecycleBinObject( CloudRecycleBinObject* crbo );
	~CloudRecycleBinPropertyAbout();
};


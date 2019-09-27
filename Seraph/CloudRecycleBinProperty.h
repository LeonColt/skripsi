#pragma once
#include "Property.h"
#include "CloudRecycleBinObject.h"
#include "CloudRecycleBinPropertyAbout.h"
#include "resources.h"
#include "OnClickListener.h"
class CloudRecycleBinProperty : public Property, public maschinen::event::OnClickListener {
private:
	CloudRecycleBinObject* crbo;

	maschinen::Button btt_about;
	std::unique_ptr<CloudRecycleBinPropertyAbout> about;
protected:
	void onCreate();
public:
	CloudRecycleBinProperty();
	void onClick( maschinen::event::OnClickEvent& e );
	void setCloudRecycleBinObject( CloudRecycleBinObject* crbo );
	~CloudRecycleBinProperty();
};


#pragma once
#include "Property.h"
#include "FileControl.h"
#include "Local.h"
#include "LocalPropertyAbout.h"
#include "OnClickListener.h"
class LocalProperty : public Property, public maschinen::event::OnClickListener {
private:
	FileControl* fc;
	Local* local;

	maschinen::Button btt_about;
	std::unique_ptr<LocalPropertyAbout> about;
protected:
	void onCreate();
public:
	LocalProperty();
	void onClick( maschinen::event::OnClickEvent& e );
	void setFileControl( FileControl* fc );
	void setLocal( Local* local );
	~LocalProperty();
};


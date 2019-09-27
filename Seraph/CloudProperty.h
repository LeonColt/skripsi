#pragma once
#include "Property.h"
#include "FileControl.h"
#include "LayoutMatrix.h"
#include "Button.h"
#include "OnClickListener.h"
#include "CloudPropertyAbout.h"
#include "CloudPropertyPrivilegeList.h"
class CloudProperty : public Property, public maschinen::event::OnClickListener {
private:
	FileControl* fc;
	Cloud* cloud;
	maschinen::Button btt_about;
	maschinen::Button btt_seen_list;
	maschinen::Button btt_downloader_list;
	maschinen::Button btt_uploader_list;
	std::unique_ptr<CloudPropertyAbout> about;
	std::unique_ptr<CloudPropertyPrivilegeList> seen_list;
	std::unique_ptr<CloudPropertyPrivilegeList> downloader_list;
	std::unique_ptr < CloudPropertyPrivilegeList> uploader_list;
protected:
	void onClick( maschinen::event::OnClickEvent& e );
	void onCreate();
	void onShow();
	void onClose();
public:
	CloudProperty();
	void setFileControl( FileControl* fc );
	void setCloud( Cloud* cloud );
	~CloudProperty();
};


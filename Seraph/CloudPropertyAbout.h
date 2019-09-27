#pragma once
#include "resources.h"
#include "CustomContainer.h"
#include "Label.h"
#include "Edit.h"
#include "CheckBox.h"
#include "DirectoryCloud.h"
#include "OnCheckedChangeListener.h"
#include "MarkFileAsPrivilegeByAll.h"
class CloudPropertyAbout : public CustomContainer, 
	public maschinen::event::listener::OnCheckedChangeListener {
private:
	Cloud* cloud;

	maschinen::Label label_id_var;
	maschinen::Label label_id_val;

	maschinen::Label label_name;
	maschinen::Edit edit_name;

	maschinen::Label label_seen_by_all;
	maschinen::CheckBox check_seen_by_all;

	maschinen::Label label_download_by_all;
	maschinen::CheckBox check_download_by_all;

	maschinen::Label label_modify_by_all;
	maschinen::CheckBox check_modify_by_all;

	maschinen::Label label_owner_var;
	maschinen::Label label_owner_val;

	maschinen::Label label_parent_var;
	maschinen::Label label_parent_val;

	maschinen::Label date_created_var;
	maschinen::Label date_created_val;
protected:
	void onCreate();
	void onCheckedChanged( maschinen::CheckBox* check, bool in_check );
public:
	CloudPropertyAbout();
	void setCloud( Cloud* cloud );
	void setSeenByAllEnableDisable( bool flag );
	void setDownloadByAllEnableDisable( bool flag );
	void setModifyByAllEnableDisable( bool flag );
	void setSeenByAllCheckUncheck(bool checked);
	void setDownloadByAllCheckUncheck( bool checked );
	void setModifyByAllCheckUncheck( bool checked );
	~CloudPropertyAbout();
};


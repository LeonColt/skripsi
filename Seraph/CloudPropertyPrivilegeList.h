#pragma once
#include "Cloud.h"
#include "resources.h"
#include "CustomContainer.h"
#include "Table.h"
#include "Label.h"
#include "Edit.h"
#include "Button.h"
#include "GetPrivilegeList.h"
#include "User.h"
#include "OnClickListener.h"
#include "SetPrivilegeUserOnFile.h"
class GetPrivilegeList;
class CloudPropertyPrivilegeList : public CustomContainer, maschinen::event::OnClickListener{
private:
	const INT TYPE_SEEN, TYPE_DOWNLOAD, TYPE_UPLOAD_ON_DIRECTORY;
	Cloud* cloud;
	maschinen::Label label_id_username_email_added;
	maschinen::Edit edit_id_username_email_added;
	maschinen::Button btt_add_user_privilege;
	std::unique_ptr<maschinen::list_view::ListViewBasic> lv;
	std::unique_ptr<GetPrivilegeList> lister;
	INT type;
protected:
	void onCreate();
	void onDestroy();
	void onClick( maschinen::event::OnClickEvent& e );
public:
	CloudPropertyPrivilegeList();
	void setCloud( Cloud* cloud );
	void setUsers( std::vector<std::unique_ptr<User>>::const_iterator& begin, std::vector<std::unique_ptr<User>>::const_iterator& end );
	void setTypeSeen();
	void setTypeDownload();
	void setTypeUploadOnDirectory();
	~CloudPropertyPrivilegeList();
};


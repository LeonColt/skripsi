#pragma once
#include <memory>
#include <regex>
#include <shellapi.h>
#include "CustomContainer.h"
#include "Edit.h"
#include "Button.h"
#include "CheckBox.h"
#include "ProgressBar.h"
#include "Definator.h"
#include "resources.h"
#include "MainWindow.h"
#include "RegistryGetBenutzerID.h"
#include "RegistryRegistry.h"
#include "WininetConnect.h"
#include "OnClickListener.h"
#include "ConfirmationRegistration.h"
class Registry : public CustomContainer, maschinen::event::OnClickListener, maschinen::event::listener::OnCheckedChangeListener {
private:
	maschinen::Edit id, username, email, password, confirm_password;
	maschinen::Button btt, btt_login;
	maschinen::CheckBox cb;
	maschinen::ProgressBar pb;
	bool isRegistrationValid();
	bool flag_confirmed;
	tstring token;
	maschinen::Edit secret_code;
	maschinen::Button btt_confirm;
protected:
	void onCreate();
	void onShow();
public:
	Registry();
	void onClick( maschinen::event::OnClickEvent& e );
	void onCheckedChanged( maschinen::CheckBox* check_box, bool is_checked );
	void setUserId(LPCTSTR user_id);
	void setToken( LPCTSTR token );
	~Registry();
};


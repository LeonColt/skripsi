#pragma once
#include <memory>
#include <WinInet.h>
#include "CustomContainer.h"
#include "Akey.h"
#include "Edit.h"
#include "Button.h"
#include "ProgressBar.h"
#include "resources.h"
#include "UniversalMethode.h"
#include "UserCredential.h"
#include "MainWindow.h"
#include "LoginAsynctask.h"
#include "WininetConnect.h"
#include "OnClickListener.h"
#include "ForgetPassword.h"

class LoginAsynctask;
class Login : public CustomContainer, maschinen::event::OnClickListener
{
private:
	maschinen::Edit id_username_email, password;
	maschinen::Button btt_login, btt_forget_password, btt_register;
	maschinen::ProgressBar pb;
	bool validate();
protected:
	void onCreate();
	void onShow();
public:
	Login();
	void setAkey(Akey* akey);
	void setSetting(Setting* setting);
	void setUserCredential(UserCredential* uc);
	void memberDisable();
	void memberEnable();
	void onClick( maschinen::event::OnClickEvent& e );
	void emptyDatas();
	~Login();
};


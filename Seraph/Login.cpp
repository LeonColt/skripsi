#include "stdafx.h"
#include "Login.h"

Login::Login(){wcscpy_s(error_marker, L"Error Login");}

void Login::onCreate()
{
	id_username_email.setParentAndId(this, edit_login_id_username_email);
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &id_username_email, 0, 0, 0, 15, 0, 0 );
	id_username_email.applyTabStop();
	id_username_email.create();
	id_username_email.setPlaceHolder(true, L"id or username or email");

	password.setParentAndId(this, edit_login_password);
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &password, 17, 0, 0, 15, 0, 0 );
	password.applyPassword();
	password.applyTabStop();
	password.create();
	password.setPlaceHolder(true, L"password");

	btt_login.setParentAndId(this, button_login_login);
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &btt_login, 41, 0, 0, 8, 0, 0 );
	btt_login.create();
	btt_login.applyTabStop();
	btt_login.setText(L"Login");
	btt_login.setOnClickListener( this, false );

	//HBITMAP bitmap = (HBITMAP)LoadImage( GetModuleHandle( NULL ), MAKEINTRESOURCE(IDI_SERAPH), IMAGE_BITMAP, NULL, NULL, LR_DEFAULTCOLOR );

	//SendMessage( btt_login.getHandler(), BM_SETIMAGE, ( WPARAM ) IMAGE_BITMAP, ( LPARAM ) bitmap );

	btt_forget_password.setParentAndId(this, button_login_forget_password);
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &btt_forget_password, 51, 0, 0, 8, 0, 0 );
	btt_forget_password.create();
	btt_forget_password.applyTabStop();
	btt_forget_password.setText(L"Forget Password");
	btt_forget_password.setOnClickListener( this, false );

	btt_register.setParentAndId(this, button_login_registration);
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &btt_register, 61, 0, 0, 8, 0, 0 );
	btt_register.create();
	btt_register.applyTabStop();
	btt_register.setText(L"Registry");
	btt_register.setOnClickListener( this, false );

	pb.setParentAndId(this, progress_bar_login_pb);
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &pb, 0, 0, 0, 69, 0, 0 );
	pb.styleMarquee();
	pb.create();
}

void Login::onShow() {
	CustomContainer::onShow();
	pb.hide();
}

void Login::memberDisable()
{
	pb.show();
	pb.setMarqueeSpeed(true, 1);
	id_username_email.disable();
	password.disable();
	btt_login.disable();
	btt_forget_password.disable();
	btt_register.disable();
}

void Login::memberEnable()
{
	pb.hide();
	id_username_email.enable();
	password.enable();
	btt_login.enable();
	btt_forget_password.enable();
	btt_register.enable();
}

bool Login::validate()
{
	bool validated = true;
	TCHAR message[MAX_LOADSTRING] = L"";
	if (id_username_email.getText()->empty()) { validated = false; wcscat_s(message, L"ID or Username or Email cannot be empty.\n"); }
	if (password.getText()->empty()) { validated = false; wcscat_s(message, L"Password cannot be empty.\n"); }
	if (!validated) MessageBox(((maschinen::Window*)parent)->getHandler(), message, error_marker, MB_ICONERROR | MB_OK);
	return validated;
}

void Login::setAkey(Akey* akey)
{
	this->akey = akey;
}

void Login::setSetting(Setting* setting)
{
	this->setting = setting;
}

void Login::setUserCredential(UserCredential* uc)
{
	this->uc = uc;
}

void Login::onClick( maschinen::event::OnClickEvent& e ) {
	switch ( e.getSourceId() ) {
		case button_login_login:
		{
			if ( !uc->isCredential() ) {
				if ( validate() ) {
					LoginAsynctask* lat = new LoginAsynctask();
					lat->setAutoDelete();
					lat->setParent( this );
					lat->setAkey( akey );
					lat->setSetting( setting );
					lat->setUserCredential( uc );
					lat->setDatas( id_username_email.getText()->c_str(), password.getText()->c_str() );
					lat->execute();
					password.clearText();
				}
			}
		} break;

		case button_login_forget_password: {
			if ( id_username_email.isTextEmpty() ) {
				MessageBox( getWindowHandler(), TEXT( "Please Input Your Id or UserName or Email on login textbox" ), error_marker, MB_ICONERROR | MB_OK );
				break;
			}
			if( MessageBox( getWindowHandler(), TEXT( "Are you sure you wanna reset your password?" ), error_marker, MB_ICONQUESTION | MB_YESNO ) == IDYES ) {
				ForgetPassword* fg = new ForgetPassword();
				fg->setParent( this );
				fg->setAutoDelete();
				fg->setAkey( akey );
				fg->setSetting( setting );
				fg->setUserCredential( uc );
				fg->setIdUsernameEmail( id_username_email.getText()->c_str() );
				fg->execute();
			}
		} break;
		case button_login_registration: static_cast<MainWindow*>( parent )->showRegistry(); break;
	}
}

void Login::emptyDatas() {
	id_username_email.clearText();
	password.clearText();
}

Login::~Login()
{
}

#include "stdafx.h"
#include "Registry.h"


Registry::Registry(){}

void Registry::onShow()
{
	CustomContainer::onShow();
	pb.hide();
	secret_code.hide();
	btt_confirm.hide();

	RegistryGetBenutzerID* rgbi = new RegistryGetBenutzerID();
	rgbi->setAutoDelete();
	rgbi->setParent(this);
	rgbi->setAkey(akey);
	rgbi->setSetting(setting);
	rgbi->setUserCredential(uc);
	rgbi->execute();
}

void Registry::onCreate()
{
	id.setParentAndId(this, edit_registration_id);
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &id, 7, 1, 0, 8, 0, 0 );
	id.applyReadOnly();
	id.create();

	username.setParentAndId(this, edit_registration_username);
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &username, 7, 3, 0, 8, 0, 0 );
	username.create();
	username.setPlaceHolder(true, L"Username");

	email.setParentAndId(this, edit_registration_email);
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &email, 7, 5, 0, 8, 0, 0 );
	email.create();
	email.setPlaceHolder(true, L"Email");

	password.setParentAndId(this, edit_registration_password);
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &password, 7, 7, 0, 8, 0, 0 );
	password.applyPassword();
	password.create();
	password.setPlaceHolder(true, L"Password");

	confirm_password.setParentAndId(this, edit_registration_confirm_password);
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &confirm_password, 7, 9, 0, 8, 0, 0 );
	confirm_password.applyPassword();
	confirm_password.create();
	confirm_password.setPlaceHolder(true, L"Confirm Password");

	cb.setParentAndId(this, check_box_registration_aggree);
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &cb, 8, 11, 0, 3, 1, 0 );
	cb.create();
	cb.setText(L"Agree");
	cb.setOnCheckedChangeListener( this, false );

	btt.setParentAndId(this, btt_registration_regisration);
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &btt, 8, 15, 0, 3, 1, 0 );
	btt.create();
	btt.setText(L"Submit");
	btt.setOnClickListener( this, false );

	btt_login.setParentAndId(this, btt_registration_login);
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &btt_login, 8, 19, 0, 3, 1, 0 );
	btt_login.create();
	btt_login.setText(L"Login");
	btt_login.setOnClickListener( this, false );

	pb.setParentAndId(this, 0);
	pb.styleMarquee();
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &pb, 10, 10, 0, 10, 5, 0 );
	pb.create();

	secret_code.setParentAndId( this, EDIT_CONFIRM_REGISTRATION_SECRET_CODE );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &secret_code, 0, 0, 0, 3, 1, 0 );
	secret_code.create();

	btt_confirm.setParentAndId( this, BTT_CONFIRM_REGISTRATION );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &btt_confirm, 0, 3, 0, 3, 1, 0 );
	btt_confirm.create();
	btt_confirm.setText( TEXT( "Confirm" ) );
	btt_confirm.setOnClickListener( this );
}

bool Registry::isRegistrationValid()
{
	tstring message = TEXT( "" );
	bool valid = true;
	if ( id.getText()->length() < 1 ) {valid = false; message.append( TEXT( "Id cannot be empty.\n" ) );}
	if ( username.getText()->length() < 1 ) { valid = false; message.append( TEXT( "Username cannot be empty.\n" ) ); }
	if ( username.getText()->length() < 1 ) { valid = false; message.append( TEXT( "Email cannot be empty.\n" ) ); }
	if ( password.getText()->length() < 1 ) { valid = false; message.append( TEXT( "Password cannot be empty.\n" ) ); }
#ifdef _UNICODE
	std::wregex regex( TEXT( "(.{8,})" ) );
#else
	std::regex regex( TEXT( "(.{8,})" ) );
#endif // _UNICODE
	if ( !std::regex_search( password.getText()->c_str(), regex ) ) { valid = false; message.append( TEXT( "Password must contain at least 8 characters" ) ); }
	regex.assign( TEXT( "((?=.*\\d).{8,})" ) );
	if ( !std::regex_search( password.getText()->c_str(), regex ) ) { valid = false; message.append( TEXT( "Password must contain at least one numeric" ) ); }
	regex.assign( TEXT( "((?=.*[a-z]).{8,})" ) );
	if ( !std::regex_search( password.getText()->c_str(), regex ) ) { valid = false; message.append( TEXT( "Password must contain at least 1 lower characters" ) ); }
	regex.assign( TEXT( "((?=.*[A-Z]).{8,})" ) );
	if ( !std::regex_search( password.getText()->c_str(), regex ) ) { valid = false; message.append( TEXT( "Password must contain at least 1 upper characters" ) ); }
	regex.assign( TEXT( "((?=.*[@#$%!^&*()_\\-=+\\:;\'\"<, .> \? / `~]).{8,})" ) );
	//regex.assign( TEXT( "((?=.*[@#$%!^&*()_\\-=+\\:;\'\"<, .> \? / `~] ).{8, } )" ) );
	if ( !std::regex_search( password.getText()->c_str(), regex ) ) { valid = false; message.append( TEXT( "Password must contain at least 1 special characters" ) ); }
	//regex.assign( TEXT( "((?=.*\\d)(?=.*[a-z])(?=.*[A-Z])(?=.*[@#$%]).{8,})" ) );
	//if (!std::regex_search(password.getText()->c_str(), regex)) valid = false;

	if (confirm_password.getText()->compare(password.getText()->c_str()) != 0)
	{
		valid = false;
		message.append( TEXT( "Password and Confirm Password must be same.\n" ) );
	}
	if ( !cb.isChecked() ) {
		valid = false;
		message.append( TEXT( "Agreement must be checked.\n" ) );
	}
	if (!valid) MessageBox(((maschinen::Window*)parent)->getHandler(), message.c_str(), L"Error", MB_ICONERROR | MB_OK);
	return valid;
}

void Registry::onClick( maschinen::event::OnClickEvent& e ) {
	switch ( e.getSourceId() ) {
		case btt_registration_regisration:
		{
			if ( isRegistrationValid() ) {
				RegistryRegistry* r2 = new RegistryRegistry();
				r2->setAutoDelete();
				r2->setParent( this );
				r2->setAkey( akey );
				r2->setUserCredential( uc );
				r2->setSetting( setting );
				r2->setProgressBarAndBttRegAndBttLogin( &pb, &btt, &btt_login );
				r2->setUserDatas( id.getText()->c_str(), username.getText()->c_str(), email.getText()->c_str(), password.getText()->c_str() );
				r2->execute();
			}
		} break;
		case btt_registration_login: { static_cast<MainWindow*>( parent )->showLogin(); static_cast<MainWindow*>( parent )->showFileControl(); } break;

		case BTT_CONFIRM_REGISTRATION: {
			if( !secret_code.getText()->empty()) {
				ConfirmationRegistration* cr = new ConfirmationRegistration();
				cr->setParent( this );
				cr->setAutoDelete();
				cr->setErrorMarker( error_marker );
				cr->setAkey( akey );
				cr->setSetting( setting );
				cr->setUserCredential( uc );
				cr->setToken( token.c_str() );
				cr->setSecretKey( secret_code.getText()->c_str() );
				cr->execute();
			}
			else MessageBox( getWindowHandler(), TEXT( "Please Input the secret key that we sent to you" ), error_marker, MB_ICONERROR | MB_OK );
		} break;
	}
}

void Registry::onCheckedChanged( maschinen::CheckBox* check_box, bool is_checked ) {
	if ( check_box->getId() == cb.getId() ) {
		//if(is_checked ) ShellExecute( ( ( maschinen::Window* )parent )->getHandler(), L"open", L"http://www.google.com", NULL, NULL, SW_SHOW );
	}
}

void Registry::setUserId(LPCTSTR user_id) {
	id.setText(user_id);
}

void Registry::setToken( LPCTSTR token ) {
	this->token.assign( token );
	id.hide();
	username.hide();
	email.hide(); 
	password.hide();
	confirm_password.hide();
	btt.hide();
	btt_login.hide();
	cb.hide();
	secret_code.show();
	btt_confirm.show();
}

Registry::~Registry()
{
}

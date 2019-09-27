#include "stdafx.h"
#include "UserCredential.h"


UserCredential::UserCredential() : NONE(0), SUPERADMIN(1), ADMIN(2), MEMBER(3)
{
	benutzer_id = 0;
}

void UserCredential::setPhpSessionId( LPCTSTR php_session_id ) {
	this->php_session_id.assign( php_session_id );
}

tstring& UserCredential::getPhpSessionId() {
	return php_session_id;
}

QWORD UserCredential::getBenutzerID() const {
	return benutzer_id;
}

void UserCredential::setBenutzerID( LONGLONG benutzer_id ) {
	this->benutzer_id = benutzer_id;
}

void UserCredential::setLoginId(LPCTSTR login_id)
{
	this->login_id.assign( login_id );
}

void UserCredential::setUserName(LPCTSTR username)
{
	this->username.assign( username );
}

std::unique_ptr<tstring> UserCredential::getUserName() const noexcept {return std::make_unique<tstring>( username.c_str() );}
std::unique_ptr<tstring> UserCredential::getLoginId() const noexcept { return std::make_unique<tstring>( login_id.c_str() ); }

void UserCredential::setEmail(LPCTSTR email)
{
	this->email.assign( email );
}

void UserCredential::setRequestGetFile(LPCTSTR request_get_file)
{
	wcscpy_s(this->request_get_file, request_get_file);
}

void UserCredential::setRequestDownloadFiles(LPCTSTR request_download_file)
{
	wcscpy_s(this->request_download_file, request_download_file);
}

bool UserCredential::read()
{
	return true;
}

bool UserCredential::write()
{
	return true;
}

bool UserCredential::isCredential()
{
	return benutzer_id != 0 && !login_id.empty() && !username.empty();
}

void UserCredential::setGroupPrivilege( int privilege_group ) {
	this->privilege_group = privilege_group;
}
int UserCredential::getGroupPrivilege(){
	return privilege_group;
}
bool UserCredential::isSuperAdmin(){
	return privilege_group == SUPERADMIN;
}
bool UserCredential::isAdmin(){
	return privilege_group == ADMIN;
}
bool UserCredential::isMember(){
	return privilege_group == MEMBER;
}

void UserCredential::uncredential() noexcept {
	php_session_id.clear();
	login_id.clear();
	username.clear();
	email.clear();
	benutzer_id = 0;
	privilege_group = 0;
}

UserCredential::~UserCredential()
{
}

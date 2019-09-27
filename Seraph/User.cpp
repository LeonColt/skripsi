#include "stdafx.h"
#include "User.h"


User::User() {
}

void User::setId( QWORD id ){
	this->id = id;
}
void User::setUsername( tstring& username ){
	this->username.assign( username.c_str() );
}
void User::setUsername( LPCTSTR username ) {
	this->username.assign( username );
}
void User::setEmail( tstring& email ){
	this->email.assign( email.c_str() );
}
QWORD User::getId(){
	return id;
}
std::unique_ptr<tstring> User::getUsername(){
	return std::make_unique<tstring>( username.c_str() );
}
std::unique_ptr<tstring> User::getEmail() {
	return std::make_unique<tstring>( email.c_str() );
}

bool User::operator==( User& user ){
	return id == user.id;
}
bool User::operator==( User* user ) {
	if ( user == nullptr ) return false;
	return user->id == id;
}

bool User::compare( User& user ){
	return id == user.id;
}
bool User::compare( User* user ) {
	if ( user == nullptr ) return false;
	return user->id == id;
}

User::~User() {
}

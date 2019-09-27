#pragma once
#include "Maschinen.h"
class User {
private:
	QWORD id;
	tstring username;
	tstring email;
public:
	User();
	void setId( QWORD id );
	void setUsername( LPCTSTR username );
	void setUsername( tstring& username );
	void setEmail( tstring& email );
	QWORD getId();
	std::unique_ptr<tstring> getUsername();
	std::unique_ptr<tstring> getEmail();
	bool operator==(User& user);
	bool operator==( User* user );
	bool compare( User& user );
	bool compare( User* user );
	~User();
};


#pragma once
#include "UniversalMethode.h"
#include "UserCredential.h"
#include "Crypt.h"
#include "Akey.h"
class MetaData {
protected:
	const std::string DISPOSITION = "DISPOSITION";
	const std::string ID = "ID";
	Akey* akey = nullptr;
	Setting* setting = nullptr;
	UserCredential* user_credential = nullptr;
	tstring id;
	bool flag_read = false;
public:
	MetaData();
	void create();
	bool exists();
	virtual void read(LPCTSTR meta_name) = 0;
	void read( tstring& meta_name );
	void read( tstring* meta_name );
	void read();

	void setId( LPCTSTR id ) noexcept; 
	std::unique_ptr<tstring> getId() const noexcept;

	std::unique_ptr<tstring> getParentPath() const;
	std::unique_ptr<tstring> getPath() const;
	std::unique_ptr<tstring> getMetaName() const;

	void setAkey( Akey* akey );void setSetting( Setting* setting );void setUserCredential( UserCredential* user_credential );

	bool operator==( MetaData& meta_data );
	bool operator==( MetaData* meta_data );

	~MetaData();
};
#include "stdafx.h"
#include "CloudRecycleBinObject.h"


CloudRecycleBinObject::CloudRecycleBinObject() {
	id = 0;
}

void CloudRecycleBinObject::setId( QWORD id ) noexcept{this->id = id;}
QWORD CloudRecycleBinObject::getId() const noexcept {return id;}
std::unique_ptr<tstring> CloudRecycleBinObject::getPath() const noexcept {
	std::unique_ptr<tstring> res = std::make_unique<tstring>();
	res->append( L"Cloud Recycle Bin\\");
	res->append( getName()->c_str() );
	return res;
}

void CloudRecycleBinObject::setOwner( LPCTSTR owner ){
	this->owner.assign( owner );
}
void CloudRecycleBinObject::setOwner( tstring& owner ){
	this->owner.assign( owner.c_str() );
}
void CloudRecycleBinObject::setOwner( tstring* owner ) {
	this->owner.assign( owner->c_str() );
}

std::unique_ptr<tstring> CloudRecycleBinObject::getOwner() const noexcept {
	return std::make_unique<tstring>( owner.c_str() );
}

void CloudRecycleBinObject::setAsDirectory( bool flag_directory ) {
	this->flag_directory = flag_directory;
}
bool CloudRecycleBinObject::isDirectory() const noexcept { return flag_directory; }

std::unique_ptr<tstring> CloudRecycleBinObject::getType() const noexcept {
	if ( flag_directory ) return std::make_unique<tstring>( TEXT( "Directory" ) );
	else return std::make_unique<tstring>( TEXT( "Unknown" ) );
}

void CloudRecycleBinObject::setDateDeleted( tstring date_deleted ) noexcept{
	this->date_deleted.assign( date_deleted.c_str() );
}
std::unique_ptr<tstring> CloudRecycleBinObject::getDateDeleted() const noexcept{
	return std::make_unique<tstring>( date_deleted.c_str() );
}

CloudRecycleBinObject::~CloudRecycleBinObject() {
}

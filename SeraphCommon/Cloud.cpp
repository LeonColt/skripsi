#include "stdafx.h"
#include "Cloud.h"



Cloud::Cloud() {
	downloadable = deletable = seen_by_all = download_by_all = modify_by_all = false;
}
void Cloud::setId( QWORD id ) noexcept { this->id = id; }
QWORD Cloud::getId() const noexcept { return id; }
void Cloud::setParentId( QWORD parent_id ) { this->parent_id = parent_id; }
QWORD Cloud::getParentId() { return parent_id; }
void Cloud::setOwner( LPCTSTR owner ) { this->owner.assign( owner ); }
std::unique_ptr<tstring> Cloud::getOwner() const noexcept { return std::make_unique<tstring>( owner.c_str() ); }
QWORD Cloud::getOwnerId() const noexcept { return owner_id; }
void Cloud::setDeletable( bool deletable ) noexcept { this->deletable = deletable; }
bool Cloud::isDeletable() const noexcept { return deletable; }
void Cloud::setDownloadable( bool downloadable ) noexcept { this->downloadable = downloadable; }
bool Cloud::isDownloadable() const noexcept { return downloadable; }

void Cloud::setSize( LONGLONG size ) { this->size = size; }
LONGLONG Cloud::getSize() { return size; }

void Cloud::setSeenByAll( bool seen_by_all ) {this->seen_by_all = seen_by_all;}
void Cloud::setDownloadByAll( bool download_by_all ) {this->download_by_all = download_by_all;}
void Cloud::setModifyByAll( bool modify_by_all ) {this->modify_by_all = modify_by_all;}

bool Cloud::isSeenByAll() { return seen_by_all; }
bool Cloud::isDownloadByAll() { return download_by_all; }
bool Cloud::isModifyByAll() { return modify_by_all; }

void Cloud::operator=( Cloud& cloud ) noexcept{
	BaseFile::operator=( cloud );
	parent_id = cloud.parent_id;
	id = cloud.id;
	size = cloud.size;
	owner = cloud.owner;
	owner_id = cloud.owner_id;
	downloadable = cloud.downloadable;
	deletable = cloud.deletable;
	seen_by_all = cloud.seen_by_all;
	download_by_all = cloud.download_by_all;
	modify_by_all = cloud.modify_by_all;
}
void Cloud::operator=( Cloud* cloud ) noexcept{
	BaseFile::operator=( cloud );
	parent_id = cloud->parent_id;
	id = cloud->id;
	size = cloud->size;
	owner = cloud->owner;
	owner_id = cloud->owner_id;
	downloadable = cloud->downloadable;
	deletable = cloud->deletable;
	seen_by_all = cloud->seen_by_all;
	download_by_all = cloud->download_by_all;
	modify_by_all = cloud->modify_by_all;
}
void Cloud::assign( Cloud& cloud ) noexcept{
	BaseFile::assign( cloud );
	parent_id = cloud.parent_id;
	id = cloud.id;
	size = cloud.size;
	owner = cloud.owner;
	owner_id = cloud.owner_id;
	downloadable = cloud.downloadable;
	deletable = cloud.deletable;
	seen_by_all = cloud.seen_by_all;
	download_by_all = cloud.download_by_all;
	modify_by_all = cloud.modify_by_all;
}
void Cloud::assign( Cloud* cloud ) noexcept{
	BaseFile::assign( cloud );
	parent_id = cloud->parent_id;
	id = cloud->id;
	size = cloud->size;
	owner = cloud->owner;
	owner_id = cloud->owner_id;
	downloadable = cloud->downloadable;
	deletable = cloud->deletable;
	seen_by_all = cloud->seen_by_all;
	download_by_all = cloud->download_by_all;
	modify_by_all = cloud->modify_by_all;
}

Cloud::~Cloud() {
}

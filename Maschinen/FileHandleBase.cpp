#include "stdafx.h"
#include "FileHandleBase.h"


maschinen::file::FileHandleBase::FileHandleBase() {
	security_attributes = nullptr;
	handle = INVALID_HANDLE_VALUE;
	share_mode = 0;
	access = 0;
	attrs_and_flags = 0;
	file_name.assign( TEXT( "" ) );
}

HANDLE maschinen::file::FileHandleBase::get() const noexcept {return handle;}
bool maschinen::file::FileHandleBase::isValid() const noexcept { return handle != INVALID_HANDLE_VALUE; }
void maschinen::file::FileHandleBase::set( HANDLE handle ) noexcept {
	close();
	this->handle = handle;
}
void maschinen::file::FileHandleBase::close() {
	if ( isValid() ) {
		CloseHandle( handle );
		handle = INVALID_HANDLE_VALUE;
	}
}



maschinen::file::FileHandleBase::~FileHandleBase() { close(); }

maschinen::file::FileHandleBase& maschinen::file::FileHandleBase::shareDelete( bool share_delete ) noexcept {
	if( share_delete ) share_mode |= FILE_SHARE_DELETE;
	else {
		share_mode |= FILE_SHARE_DELETE;
		share_mode ^= FILE_SHARE_DELETE;
	}
	return *this;
}
maschinen::file::FileHandleBase& maschinen::file::FileHandleBase::shareRead( bool share_read ) noexcept {
	if ( share_read ) share_mode |= FILE_SHARE_READ;
	else {
		share_mode |= FILE_SHARE_READ;
		share_mode ^= FILE_SHARE_READ;
	}
	return *this;
}
maschinen::file::FileHandleBase& maschinen::file::FileHandleBase::shareWrite( bool share_write ) noexcept {
	if ( share_write ) share_mode |= FILE_SHARE_WRITE;
	else {
		share_mode |= FILE_SHARE_WRITE;
		share_mode ^= FILE_SHARE_WRITE;
	}
	return *this;
}
maschinen::file::FileHandleBase& maschinen::file::FileHandleBase::shareAll() noexcept {
	share_mode |= FILE_SHARE_WRITE;
	share_mode |= FILE_SHARE_READ;
	share_mode |= FILE_SHARE_DELETE;
	return *this;
}
maschinen::file::FileHandleBase& maschinen::file::FileHandleBase::setShareMode( DWORD share_mode ) noexcept {
	this->share_mode = share_mode;
	return *this;
}
maschinen::file::FileHandleBase& maschinen::file::FileHandleBase::setSecurityAttributes( maschinen::security::SecurityAttributes* security_attributes ) noexcept { this->security_attributes = security_attributes; return *this; }
maschinen::file::FileHandleBase& maschinen::file::FileHandleBase::attributeNormal( bool attr ) noexcept {
	if ( attr ) attrs_and_flags |= FILE_ATTRIBUTE_NORMAL;
	else {
		attrs_and_flags |= FILE_ATTRIBUTE_NORMAL;
		attrs_and_flags ^= FILE_ATTRIBUTE_NORMAL;
	}
	return *this;
}
maschinen::file::FileHandleBase& maschinen::file::FileHandleBase::attributeArchive( bool attr ) noexcept {
	if ( attr ) attrs_and_flags |= FILE_ATTRIBUTE_ARCHIVE;
	else {
		attrs_and_flags |= FILE_ATTRIBUTE_ARCHIVE;
		attrs_and_flags ^= FILE_ATTRIBUTE_ARCHIVE;
	}
	return *this;
}
maschinen::file::FileHandleBase& maschinen::file::FileHandleBase::attributeEncrypted( bool attr ) noexcept {
	if ( attr ) attrs_and_flags |= FILE_ATTRIBUTE_ENCRYPTED;
	else {
		attrs_and_flags |= FILE_ATTRIBUTE_ENCRYPTED;
		attrs_and_flags ^= FILE_ATTRIBUTE_ENCRYPTED;
	}
	return *this;
}
maschinen::file::FileHandleBase& maschinen::file::FileHandleBase::attributeHidden( bool attr ) noexcept {
	if ( attr ) attrs_and_flags |= FILE_ATTRIBUTE_HIDDEN;
	else {
		attrs_and_flags |= FILE_ATTRIBUTE_HIDDEN;
		attrs_and_flags ^= FILE_ATTRIBUTE_HIDDEN;
	}
	return *this;
}
maschinen::file::FileHandleBase& maschinen::file::FileHandleBase::attributeOffline( bool attr ) noexcept {
	if ( attr ) attrs_and_flags |= FILE_ATTRIBUTE_OFFLINE;
	else {
		attrs_and_flags |= FILE_ATTRIBUTE_OFFLINE;
		attrs_and_flags ^= FILE_ATTRIBUTE_OFFLINE;
	}
	return *this;
}
maschinen::file::FileHandleBase& maschinen::file::FileHandleBase::attributeReadOnly( bool attr ) noexcept {
	if ( attr ) attrs_and_flags |= FILE_ATTRIBUTE_READONLY;
	else {
		attrs_and_flags |= FILE_ATTRIBUTE_READONLY;
		attrs_and_flags ^= FILE_ATTRIBUTE_READONLY;
	}
	return *this;
}
maschinen::file::FileHandleBase& maschinen::file::FileHandleBase::attributeSystem( bool attr ) noexcept {
	if ( attr ) attrs_and_flags |= FILE_ATTRIBUTE_SYSTEM;
	else {
		attrs_and_flags |= FILE_ATTRIBUTE_SYSTEM;
		attrs_and_flags ^= FILE_ATTRIBUTE_SYSTEM;
	}
	return *this;
}
maschinen::file::FileHandleBase& maschinen::file::FileHandleBase::attributeTemporary( bool attr ) noexcept {
	if ( attr ) attrs_and_flags |= FILE_ATTRIBUTE_TEMPORARY;
	else {
		attrs_and_flags |= FILE_ATTRIBUTE_TEMPORARY;
		attrs_and_flags ^= FILE_ATTRIBUTE_TEMPORARY;
	}
	return *this;
}

maschinen::file::FileHandleBase& maschinen::file::FileHandleBase::setAttributesAndFlags( DWORD attrs_and_flags ) {
	this->attrs_and_flags = attrs_and_flags; return *this;
}
maschinen::file::FileHandleBase& maschinen::file::FileHandleBase::setAccessMode( DWORD access ) {
	this->access = access; return *this;
}
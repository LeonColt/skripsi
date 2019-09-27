#pragma once
#include "Local.h"
class MetaDataUploadDetail {
private:
	QWORD id;
	tstring name;
	tstring path;
	QWORD parent;
	QWORD size;
	bool as_directory;
public:
	MetaDataUploadDetail();
	void setId( QWORD id ) noexcept;
	void setName( LPCTSTR name ) noexcept;
	void setName( tstring& name ) noexcept;
	void setName( tstring* name ) noexcept;
	void setPath( LPCTSTR path ) noexcept;
	void setPath( tstring& path ) noexcept;
	void setPath( tstring* path ) noexcept;
	void asDirectory( bool as_directory = true ) noexcept;
	QWORD getId() const noexcept;
	std::unique_ptr<tstring> getPath() const noexcept;
	std::unique_ptr<tstring> getName() const noexcept;
	QWORD getSize() const;
	bool isDirectory() const noexcept;
	~MetaDataUploadDetail();
};


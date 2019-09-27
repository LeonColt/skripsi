#pragma once
#include "Maschinen.h"
class MetaDataDownloadBaseFile {
private:
	QWORD id;
	tstring name;
	ULONGLONG size;
	bool original;
	MetaDataDownloadBaseFile* parent = nullptr;
public:
	MetaDataDownloadBaseFile();
	void setId( QWORD id ) noexcept; QWORD getId() const noexcept;
	void setName( tstring& name ) noexcept; void setName( tstring* name ) noexcept;
	void setName( LPCTSTR name ) noexcept; std::unique_ptr<tstring> getName() const noexcept;
	void setSize( ULONGLONG size ) noexcept; ULONGLONG getSize() const noexcept;
	void setOriginal( bool original ) noexcept; bool isOriginal() const noexcept;
	void setParent( MetaDataDownloadBaseFile* parent ) noexcept; MetaDataDownloadBaseFile* getParent() const noexcept;
	virtual std::unique_ptr<tstring> getPath() noexcept = 0;
	~MetaDataDownloadBaseFile();
};


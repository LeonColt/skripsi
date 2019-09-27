#pragma once
#include <memory>
#include <vector>
#include "SeraphCommon.h"
#include "Maschinen.h"
#include "Exception.h"
class SERAPHCOMMON_API BaseFile {
protected:
	tstring name;
	tstring path;
public:
	BaseFile();
	void setName( LPCTSTR name );
	void setName( tstring& name );
	void setName( tstring* name );
	virtual std::unique_ptr<tstring> getName() const noexcept;
	void setPath( LPCTSTR path );
	void setPath( tstring& path );
	void setPath( tstring* path );
	virtual std::unique_ptr<tstring> getPath() const noexcept;
	virtual bool operator==( BaseFile& base_file ) const noexcept;
	virtual bool operator==( BaseFile* base_file ) const;
	virtual bool operator!=( BaseFile& base_file ) const noexcept;
	virtual bool operator!=( BaseFile* base_file ) const;
	virtual bool compare( BaseFile& base_file ) const noexcept;
	virtual bool compare( BaseFile* base_file ) const;
	virtual void operator=( BaseFile& base_file ) noexcept;
	virtual void operator=( BaseFile* base_file ) noexcept;
	virtual void assign( BaseFile& base_file ) noexcept;
	virtual void assign( BaseFile* base_file ) noexcept;
	virtual ~BaseFile();
};


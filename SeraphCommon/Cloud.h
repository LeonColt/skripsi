#pragma once
#include "SeraphCommon.h"
#include "BaseFile.h"
class SERAPHCOMMON_API Cloud : public BaseFile {
protected:
	QWORD parent_id;
	QWORD id;
	LONGLONG size;
	tstring owner;
	QWORD owner_id;
	bool downloadable;
	bool deletable;
	bool seen_by_all;
	bool download_by_all;
	bool modify_by_all;
public:
	Cloud();
	void setId( QWORD id ) noexcept;
	QWORD getId() const noexcept;
	void setParentId( QWORD parent_id );
	QWORD getParentId();
	void setOwner( LPCTSTR owner );
	std::unique_ptr<tstring> getOwner() const noexcept;
	QWORD getOwnerId() const noexcept;

	void setSize( LONGLONG size ); LONGLONG getSize();
	void setDeletable( bool deletable = true ) noexcept;
	bool isDeletable() const noexcept;
	void setDownloadable( bool downloadable = true ) noexcept;
	bool isDownloadable() const noexcept;

	void setSeenByAll( bool seen_by_all = true );
	void setDownloadByAll( bool download_by_all = true );
	void setModifyByAll( bool modify_by_all = true );

	bool isSeenByAll();
	bool isDownloadByAll();
	bool isModifyByAll();

	void operator=( Cloud& cloud ) noexcept;
	void operator=( Cloud* cloud ) noexcept;
	void assign( Cloud& cloud ) noexcept;
	void assign( Cloud* cloud ) noexcept;

	~Cloud();
};


#pragma once
#include <list>
#include "SeraphCommon.h"
#include "Cloud.h"
class SERAPHCOMMON_API DirectoryCloud : public Cloud {
protected:
	std::list<Cloud*> children;
public:
	DirectoryCloud();
	bool create();
	Cloud* getChild( QWORD id ) const noexcept;
	std::list<Cloud*>* getChildren() noexcept;
	void addChildren( Cloud* child );
	void addChildren( Cloud& child );
	bool hasAnyChildren();
	bool hasChildren( Cloud* child ) const noexcept;
	bool hasChildren( QWORD id ) const noexcept;
	void erase( Cloud* child );//remove and delete children
	void erase( QWORD id ); //remove and delete children by id
	void clear();
	Cloud* findChildrenById( QWORD id, Cloud* begin_search = nullptr );
	std::list<Cloud*>::const_iterator begin() const noexcept;
	std::list<Cloud*>::const_iterator end() const noexcept;

	void operator=( DirectoryCloud& cloud ) noexcept;
	void operator=( DirectoryCloud* cloud ) noexcept;
	void assign( DirectoryCloud& cloud ) noexcept;
	void assign( DirectoryCloud* cloud ) noexcept;

	~DirectoryCloud();
};


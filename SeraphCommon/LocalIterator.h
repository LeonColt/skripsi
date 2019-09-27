#pragma once
#include <iterator>
#include "FileLocal.h"
#include "DirectoryLocal.h"
class DirectoryLocal;
class SERAPHCOMMON_API LocalIterator : std::iterator<std::forward_iterator_tag, Local, ptrdiff_t, Local*, Local&> {
	friend class DirectoryLocal;
	friend class Drive;
private:
	Local* pointer;
	maschinen::file::DirectoryHandler* dh;
	maschinen::file::DirectoryReadingIterator* dri;
	Local* parent;
public:
	LocalIterator(Local* pointer = nullptr);
	LocalIterator& operator=( const LocalIterator& pointer ) = default;
	LocalIterator& operator=( Local* pointer );
	operator bool() const;
	bool operator==(LocalIterator& pointer) const;
	bool operator!=( LocalIterator& pointer ) const;
	LocalIterator& operator++();
	LocalIterator& operator++( int );
	Local* operator*() const noexcept;
	Local* get() const;
	const Local* getConst() const;
	~LocalIterator();
};


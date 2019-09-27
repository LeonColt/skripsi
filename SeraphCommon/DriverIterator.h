#pragma once
#include <iterator>
#include "Drive.h"
#include "DriverIterator.h"
#include "DriveFixed.h"
#include "DriveRemovable.h"
#include "DriveCDRom.h"
#include "DriveUnknown.h"
class SERAPHCOMMON_API DriverIterator : public std::iterator<std::forward_iterator_tag, Drive, ptrdiff_t, Drive*, Drive&> {
	friend class DriveLoader;
private:
	Drive* pointer;
	int indexer;
	DWORD drives_letter;
public:
	DriverIterator( Drive* pointer = nullptr );
	DriverIterator& operator=( const DriverIterator& pointer ) = default;
	DriverIterator& operator=( Drive* pointer );
	bool operator==( DriverIterator& pointer ) const;
	bool operator!=( DriverIterator& pointer ) const;
	DriverIterator& operator++();
	DriverIterator& operator++( int );
	Drive* operator*() const noexcept;
	Drive* operator->() const noexcept;
	Drive* get() const;
	const Drive* getConst() const;
	~DriverIterator();
};


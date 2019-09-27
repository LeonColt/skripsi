#pragma once
#include "UncontainedController.h"
#include "ChildrenKeeper.h"
namespace maschinen {
	class MASCHINEN_API StatusBar : public maschinen::UncontainedController {
	public:
		StatusBar();
		void create();

		void simple( bool simple = true ) const;

		void setText( LPCTSTR text, WPARAM mode = 0 ) const;

		void setParent( maschinen::ContainedController* parent );
		void setParentAndId( maschinen::ContainedController* parent, UINT id );
		bool operator==( int type ) const noexcept;
		bool operator!=( int type ) const noexcept;
		bool compare( int type ) const noexcept;
		~StatusBar();
	};
}


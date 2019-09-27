#pragma once
#include "UncontainedController.h"
#include "ChildrenKeeper.h"
namespace maschinen {
	class MASCHINEN_API Label : public maschinen::UncontainedController {
	public:
		Label();
		Label( UINT id, ContainedController* parent );
		void setParent( maschinen::ContainedController* parent );
		void setParentAndId( maschinen::ContainedController* parent, UINT id );
		void create();
		void setText(LPCTSTR text) const;
		void setText( tstring& text ) const;
		std::unique_ptr<tstring> getText() const;
		~Label();
	};
}
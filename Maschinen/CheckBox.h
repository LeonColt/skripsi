#pragma once
#include <windowsx.h>
#include "UncontainedController.h"
#include "ChildrenKeeper.h"
#include "OnCheckedChangeListener.h"
namespace maschinen
{
	namespace event {
		namespace listener {
			class OnCheckedChangeListener;
		}
	}
	class MASCHINEN_API CheckBox : public UncontainedController
	{
	public:
		CheckBox();
		CheckBox( UINT id, ContainedController* parent);
		void create();
		void setOnCheckedChangeListener( maschinen::event::listener::OnCheckedChangeListener* on_checked_change_listener, bool auto_delete = false);
		void setChecked(bool checked = true) const;
		bool isChecked() const;
		void setText( LPCTSTR text ) const noexcept;
		void setText( tstring& text ) const noexcept;
		void setTexT( tstring* text ) const noexcept;

		void setParent( maschinen::ContainedController* parent );
		void setParentAndId( maschinen::ContainedController* parent, UINT id );
		bool operator==( int type ) const noexcept;
		bool operator!=( int type ) const noexcept;
		bool compare( int type ) const noexcept;
		~CheckBox();
	};
}
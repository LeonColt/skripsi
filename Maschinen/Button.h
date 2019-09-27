#pragma once
#include "UncontainedController.h"
#include "AllEventHandler.h"
#include "ChildrenKeeper.h"

namespace maschinen
{
	class MASCHINEN_API Button : public maschinen::UncontainedController
	{
	private:
		static LRESULT CALLBACK ButtonProcessStarter( HWND handler, UINT message, WPARAM wparam, LPARAM lparam, UINT_PTR uid_sub_class, DWORD_PTR dw_ref_data );
	protected:
		virtual LRESULT ButtonProcess( HWND handler, UINT message, WPARAM wparam, LPARAM lparam );
	public:
		Button();
		Button( UINT id, ContainedController* parent);
		void create();
		void setText( LPCTSTR text ) const noexcept;
		void setText( tstring& text ) const noexcept;
		void setText( const tstring& text ) const noexcept;
		void setText( tstring* text ) const noexcept;
		std::unique_ptr<tstring> getText() const noexcept;
		void setOnClickListener(maschinen::event::OnClickListener* on_click_listener, bool auto_delete = false);
		void setParent( maschinen::ContainedController* parent );
		void setParentAndId( maschinen::ContainedController* parent, UINT id );
		bool operator==( int type ) const noexcept;
		bool operator!=( int type ) const noexcept;
		bool compare( int type ) const noexcept;
		~Button();
	};
}


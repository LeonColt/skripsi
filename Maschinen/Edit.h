#pragma once
#include "UncontainedController.h"
#include "ChildrenKeeper.h"
#include "OnClickListener.h"
#include "OnEditTextChangedListener.h"
#include "OnKeyPressedListener.h"
#include "OnFocusListener.h"
namespace maschinen
{
	class MASCHINEN_API Edit : public maschinen::UncontainedController
	{
	private:
		static LRESULT CALLBACK EditProcessStarter( HWND handler, UINT message, WPARAM wparam, LPARAM lparam, UINT_PTR uid_sub_class, DWORD_PTR dw_ref_data );
		maschinen::event::listener::OnKeyPressedListener* key_pressed_listener;
		maschinen::event::listener::OnFocusListener* focus_listener;
	protected:
		virtual LRESULT EditProcess( HWND handler, UINT message, WPARAM wparam, LPARAM lparam );
		virtual void onDestroy();
	public:
		Edit();
		Edit(UINT id, ContainedController* parent);
		void create();
		void Draw();
		void update();
		void applyPassword(bool password = true);
		void applyNumber(bool number = true);
		void applyReadOnly(bool read_only = true);
		void applyRichText(bool rich_text = true);
		void setPlaceHolder(bool appear_after_focus, LPCWSTR str) const noexcept;
		void setText(LPCTSTR text) const noexcept;
		void setText( tstring& text ) const noexcept;
		void setText( tstring* text ) const noexcept;
		void clearText() const noexcept;
		bool isTextEmpty() const noexcept;
		std::unique_ptr<tstring> getText() const noexcept;

		void setOnClickListener(maschinen::event::OnClickListener* listener, bool auto_delete = false);
		void setOnEditTextChangedListener(maschinen::event::listener::OnEditTextChangedListener* listener, bool auto_delete = false);
		void setOnKeyPressedChangeListener( maschinen::event::listener::OnKeyPressedListener* listener, bool auto_delete = false );
		void setOnFocusListener( maschinen::event::listener::OnFocusListener* listener, bool auto_delete = false );

		void setParent( maschinen::ContainedController* parent );
		void setParentAndId( maschinen::ContainedController* parent, UINT id );
		bool operator==( int type ) const noexcept;
		bool operator!=( int type ) const noexcept;
		bool compare( int type ) const noexcept;
		~Edit();
	};
}
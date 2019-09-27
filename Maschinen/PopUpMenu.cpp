#include "stdafx.h"
#include "PopUpMenu.h"


maschinen::menu::PopUpMenu::PopUpMenu()
{
	handler = NULL;
	x = y = 0;
}

void maschinen::menu::PopUpMenu::append( maschinen::menu::MenuItem* m_item ) {
	UINT positioner = ( UINT ) ( items.size() );
	InsertMenuItem( handler, positioner, TRUE, m_item->get() );
	MenuItemInformationen* mii = new MenuItemInformationen();
	mii->item = m_item;
	mii->visible = true;
	mii->position = positioner;
	items.push_back( mii );
}

void maschinen::menu::PopUpMenu::append( UINT id, LPCTSTR text ) {
	MenuItem* mi = new MenuItem();
	mi->asText( id, text );
	append( mi );
}

void maschinen::menu::PopUpMenu::setOnClickListener( maschinen::event::OnClickListener* on_click_listener, bool auto_delete ) {
	for ( auto i = items.begin(); i != items.end(); i++ ) unSetMenuOnClickListener( parent, ( *i )->item->getId() );
	for ( auto i = items.begin(); i != items.end(); i++ ) 
		setMenuOnClickListener( parent, ( *i )->item->getId(), on_click_listener, auto_delete );
}

void maschinen::menu::PopUpMenu::show()
{
	if (position_by_cursor)
	{
		POINT p;
		GetCursorPos(&p);
		x = p.x;
		y = p.y;
		if ( TrackPopupMenu( handler, flag_popup_menu, x, y, 0, parent->getHandler(), NULL ) == 0 ) {
			TCHAR error[ MAX_LOADSTRING ];
			DWORD error_code;
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) ) 
				throw maschinen::exception::Exception( error, error_code );
			else throw maschinen::exception::Exception();
		}
	}
}

void maschinen::menu::PopUpMenu::create() {
	handler = CreatePopupMenu();
}

void maschinen::menu::PopUpMenu::positionbyCursor( bool position_by_cursor ) {this->position_by_cursor = position_by_cursor;}

void maschinen::menu::PopUpMenu::setVisible( UINT id, bool visible ) {
	if ( visible ) {
		for ( auto i = items.begin(); i != items.end(); i++ ) {
			if ( ( *i )->item->getId() == id && !( *i )->visible ) {
				if ( InsertMenuItem( handler, ( *i )->position, TRUE, ( *i )->item->get() ) == FALSE ) {
					TCHAR error[ MAX_LOADSTRING ];
					DWORD error_code;
					if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
						throw maschinen::exception::Exception( error, error_code );
					else throw maschinen::exception::Exception();
				}
				( *i )->visible = true;
			}
		}
	}
	else {
		for ( auto i = items.begin(); i != items.end(); i++ ) {
			if ( ( *i )->item->getId() == id ) {
				RemoveMenu( handler, id, MF_BYCOMMAND );
				( *i )->visible = false;
			}
		}
	}
}

void maschinen::menu::PopUpMenu::setVisibleVisible( std::vector<UINT> ids ) {
}

void maschinen::menu::PopUpMenu::setVisibleInvisible( std::vector<UINT> ids ) {
	for ( auto i = ids.begin(); i != ids.end(); i++ ) {
		for ( auto j = items.begin(); j != items.end(); j++ ) {
			if ( ( *j )->item->getId() == *i ) {
				RemoveMenu( handler, *i, MF_BYCOMMAND );
				( *j )->visible = false;
			}
		}
	}
}

void maschinen::menu::PopUpMenu::setVisibleVisible( std::initializer_list<UINT> ids ) {
	bool target_visible = false;
	for ( auto i = items.begin(); i != items.end(); i++ ) {
		target_visible = false;
		for ( auto j = ids.begin(); j != ids.end(); j++ ) {
			if ( *j == ( *i )->item->getId() ) {
				target_visible = true;
				break;
			}
		}
		if ( target_visible ) {
			if ( !( *i )->visible ) {
				InsertMenuItem( handler, ( *i )->position, TRUE, ( *i )->item->get() );
				( *i )->visible = true;
			}
		}
		else {
			RemoveMenu( handler, ( *i )->item->getId(), MF_BYCOMMAND );
			( *i )->visible = false;
		}
	}
}

void maschinen::menu::PopUpMenu::setVisibleInvisible( std::initializer_list<UINT> ids ) {
	for ( auto i = ids.begin(); i != ids.end(); i++ ) {
		for ( auto j = items.begin(); j != items.end(); j++ ) {
			if ( ( *j )->item->getId() == *i ) {
				RemoveMenu( handler, *i, MF_BYCOMMAND );
				( *j )->visible = false;
			}
		}
	}
}

void maschinen::menu::PopUpMenu::setEnable( UINT id, bool enable ) const {
	if(enable  )EnableMenuItem( handler, id, MF_ENABLED ); else EnableMenuItem( handler, id, MF_DISABLED );
}

void maschinen::menu::PopUpMenu::setGrayed( UINT id, bool grayed ) const {
	if ( grayed )EnableMenuItem( handler, id, MF_ENABLED ); else EnableMenuItem( handler, id, MF_GRAYED );
}

void maschinen::menu::PopUpMenu::clear() {
	for ( auto i = items.begin(); i != items.end(); i++ ) {
		unSetMenuOnClickListener( parent, ( *i )->item->getId() );
	}
	for ( auto i = items.begin(); i != items.end(); i++ ) {
		delete *i;
	}
	items.clear();
}

void maschinen::menu::PopUpMenu::destroy() {
	maschinen::menu::Menu::destroy();
	clear();
}

void maschinen::menu::PopUpMenu::setParent( maschinen::Window* parent ) {
	this->parent = parent;
}

maschinen::menu::PopUpMenu::~PopUpMenu(){}

void maschinen::menu::PopUpMenu::flagPopUpMenuCenterAlign() {
	flag_popup_menu |= TPM_CENTERALIGN;
	flag_popup_menu &= TPM_LEFTALIGN;
	flag_popup_menu &= TPM_RIGHTALIGN;
}
void maschinen::menu::PopUpMenu::flagPopUpMenuLeftAlign() {
	flag_popup_menu &= TPM_CENTERALIGN;
	flag_popup_menu |= TPM_LEFTALIGN;
	flag_popup_menu &= TPM_RIGHTALIGN;
}
void maschinen::menu::PopUpMenu::flagPopUpMenuRightAlign() {
	flag_popup_menu &= TPM_CENTERALIGN;
	flag_popup_menu &= TPM_LEFTALIGN;
	flag_popup_menu |= TPM_RIGHTALIGN;
}
void maschinen::menu::PopUpMenu::flagPopUpMenuBottomAlign() {
	flag_popup_menu |= TPM_BOTTOMALIGN;
	flag_popup_menu &= TPM_TOPALIGN;
	flag_popup_menu &= TPM_VCENTERALIGN;
}
void maschinen::menu::PopUpMenu::flagPopUpMenuTopAlign() {
	flag_popup_menu &= TPM_BOTTOMALIGN;
	flag_popup_menu |= TPM_TOPALIGN;
	flag_popup_menu &= TPM_VCENTERALIGN;
}
void maschinen::menu::PopUpMenu::flagPopUpMenuVCenterAlign() {
	flag_popup_menu &= TPM_BOTTOMALIGN;
	flag_popup_menu &= TPM_TOPALIGN;
	flag_popup_menu |= TPM_VCENTERALIGN;
}
void maschinen::menu::PopUpMenu::flagPopUpMenuNoNotify() {flag_popup_menu |= TPM_NONOTIFY;}

void maschinen::menu::PopUpMenu::flagPopUpMenuLeftButton() {flag_popup_menu |= TPM_LEFTBUTTON;}
void maschinen::menu::PopUpMenu::flagPopUpMenuRightButton() {flag_popup_menu |= TPM_RIGHTBUTTON;}

void maschinen::menu::PopUpMenu::flagPopUpMenuAnimationRightToLeft() { flag_popup_menu |= TPM_HORNEGANIMATION; }
void maschinen::menu::PopUpMenu::flagPopUpMenuAnimationLeftToRight() { flag_popup_menu |= TPM_HORPOSANIMATION; }
void maschinen::menu::PopUpMenu::flagPopUpMenuAnimationTopToBottom() { flag_popup_menu |= TPM_VERPOSANIMATION; }
void maschinen::menu::PopUpMenu::flagPopUpMenuAnimationBottomToTop() { flag_popup_menu |= TPM_VERNEGANIMATION; }
void maschinen::menu::PopUpMenu::flagPopUpMenuNoAnimation() { flag_popup_menu |= TPM_NOANIMATION; }

#include "stdafx.h"
#include "ComboBox.h"


maschinen::ComboBox::ComboBox() {
}

void maschinen::ComboBox::create() {
	style |= WS_CHILD;
	style |= WS_VISIBLE;
	handler = CreateWindowEx( WS_EX_CLIENTEDGE, WC_COMBOBOX, TEXT( "" ), style, getX(), getY(), getWidth(), getHeight(), getWindowHandler(), ( HMENU ) ( UINT_PTR ) getId(), getInstance(), NULL );
}

LONGLONG maschinen::ComboBox::addText( LPCTSTR string ) {
	LRESULT result = SendMessage( handler, CB_ADDSTRING, NULL, ( LPARAM ) string );
	if ( result == CB_ERR || result == CB_ERRSPACE ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) ) {
			throw maschinen::exception::Exception( error, error_code );
		}
		else throw maschinen::exception::Exception();
	}
	else return (LONGLONG)result;
}

void maschinen::ComboBox::setText( LONGLONG index, LPCTSTR string ) const {
	LRESULT result = SendMessage( handler, CB_SETITEMDATA, index, ( LPARAM ) index );
	if ( result == CB_ERR ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) ) {
			throw maschinen::exception::Exception( error, error_code );
		}
		else throw maschinen::exception::Exception();
	}
}

std::unique_ptr<tstring> maschinen::ComboBox::getText( LONGLONG index ) const {
	LRESULT length = SendMessage( handler, CB_GETLBTEXTLEN, index, NULL );
	TCHAR* buffer = new TCHAR[ length + 1 ];
	std::unique_ptr<tstring> temp = std::make_unique<tstring>();
	if ( SendMessage( handler, CB_GETLBTEXT, index, (LPARAM)buffer ) != CB_ERR )
		temp->assign( buffer );
	delete[] buffer;
	return temp;
}

LONGLONG maschinen::ComboBox::setCurrentSelection( LONGLONG index ) const {
	return SendMessage( handler, CB_SETCURSEL, index, NULL );
}

LONGLONG maschinen::ComboBox::setCurrentSelectionText( LPCTSTR string ) const {
	for ( auto i = 0; i < getItemsCount(); i++ ) {
		if ( getText( i )->compare( string ) == 0 )
			return setCurrentSelection( i );
	}
	return CB_ERR;
}

void maschinen::ComboBox::clearSelection() const {
	SendMessage( handler, CB_SETCURSEL, -1, NULL );
}

LONGLONG maschinen::ComboBox::getCurrentSelection() const {
	return SendMessage( handler, CB_GETCURSEL, NULL, NULL );
}

std::unique_ptr<tstring> maschinen::ComboBox::getCurrentSelectionText() const {
	LONGLONG index = getCurrentSelection();
	if ( index != CB_ERR ) return getText( index );
	return std::make_unique<tstring>( TEXT( "" ) );
}

LONGLONG maschinen::ComboBox::getItemsCount() const {
	return SendMessage( handler, CB_GETCOUNT, NULL, NULL );
}

void maschinen::ComboBox::styleAutoHorizontalScroll( bool style ) {
	if ( style ) this->style |= CBS_AUTOHSCROLL;
	else {
		this->style |= CBS_AUTOHSCROLL;
		this->style ^= CBS_AUTOHSCROLL;
	}
}

void maschinen::ComboBox::styleDisableNoScroll( bool style ) {
	if ( style ) this->style |= CBS_DISABLENOSCROLL;
	else {
		this->style |= CBS_DISABLENOSCROLL;
		this->style ^= CBS_DISABLENOSCROLL;
	}
}

void maschinen::ComboBox::styleDropDown( bool style ) {
	if ( style ) this->style |= CBS_DROPDOWN;
	else {
		this->style |= CBS_DROPDOWN;
		this->style ^= CBS_DROPDOWN;
	}
}

void maschinen::ComboBox::styleDropDownList( bool style ) {
	if ( style ) this->style |= CBS_DROPDOWNLIST;
	else {
		this->style |= CBS_DROPDOWNLIST;
		this->style ^= CBS_DROPDOWNLIST;
	}
}

void maschinen::ComboBox::styleHasStrings( bool style ) {
	if ( style ) this->style |= CBS_HASSTRINGS;
	else {
		this->style |= CBS_HASSTRINGS;
		this->style ^= CBS_HASSTRINGS;
	}
}

void maschinen::ComboBox::styleLowerCase( bool style ) {
	if ( style ) this->style |= CBS_LOWERCASE;
	else {
		this->style |= CBS_LOWERCASE;
		this->style ^= CBS_LOWERCASE;
	}
}

void maschinen::ComboBox::styleNoIntegralHeight( bool style ) {
	if ( style ) this->style |= CBS_NOINTEGRALHEIGHT;
	else {
		this->style |= CBS_NOINTEGRALHEIGHT;
		this->style ^= CBS_NOINTEGRALHEIGHT;
	}
}

void maschinen::ComboBox::styleOEMConvert( bool style ) {
	if ( style ) this->style |= CBS_OEMCONVERT;
	else {
		this->style |= CBS_OEMCONVERT;
		this->style ^= CBS_OEMCONVERT;
	}
}

void maschinen::ComboBox::styleOwnerDrawFixed( bool style ) {
	if ( style ) this->style |= CBS_OWNERDRAWFIXED;
	else {
		this->style |= CBS_OWNERDRAWFIXED;
		this->style ^= CBS_OWNERDRAWFIXED;
	}
}

void maschinen::ComboBox::styleSimple( bool style ) {
	if ( style ) this->style |= CBS_SIMPLE;
	else {
		this->style |= CBS_SIMPLE;
		this->style ^= CBS_SIMPLE;
	}
}
void maschinen::ComboBox::styleSort( bool style ) {
	if ( style ) this->style |= CBS_SORT;
	else {
		this->style |= CBS_SORT;
		this->style ^= CBS_SORT;
	}
}

void maschinen::ComboBox::styleUpperCase( bool style ) {
	if ( style ) this->style |= CBS_UPPERCASE;
	else {
		this->style |= CBS_UPPERCASE;
		this->style ^= CBS_UPPERCASE;
	}
}

bool maschinen::ComboBox::operator==( int type ) const noexcept { return type == COMBOBOX; }
bool maschinen::ComboBox::operator!=( int type ) const noexcept { return type == COMBOBOX; }
bool maschinen::ComboBox::compare( int type ) const noexcept { return type == COMBOBOX; }

maschinen::ComboBox::~ComboBox() {
}

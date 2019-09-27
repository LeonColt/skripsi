#include "stdafx.h"
#include "ListViewGroup.h"


maschinen::list_view::ListViewGroup::ListViewGroup() {
	group.cbSize = sizeof( LVGROUP );
	ZeroMemory( &group, sizeof( LVGROUP ) );
}


maschinen::list_view::ListViewGroup::~ListViewGroup() {
}

//begin set
void maschinen::list_view::ListViewGroup::setId( int id ) {
	group.mask |= LVGF_GROUPID;
	group.iGroupId = id;
}
void maschinen::list_view::ListViewGroup::setHeader( LPCTSTR header ) noexcept {
	group.mask |= LVGF_HEADER;
	group.pszHeader = (LPWSTR)header;
	group.cchHeader = ( int ) tcslen( header );
}

void maschinen::list_view::ListViewGroup::setHeader( tstring& header ) noexcept {
	group.mask |= LVGF_HEADER;
	group.pszHeader = ( LPWSTR ) header.c_str();
	group.cchHeader = ( int ) header.length();
}

void maschinen::list_view::ListViewGroup::setHeader( tstring* header ) noexcept {
	group.mask |= LVGF_HEADER;
	group.pszHeader = ( LPWSTR ) header->c_str();
	group.cchHeader = ( int ) header->length();
}
void maschinen::list_view::ListViewGroup::setFooter( LPCTSTR footer ) noexcept {
	group.mask |= LVGF_FOOTER;
	group.pszFooter = ( LPWSTR ) footer;
	group.cchFooter = ( int ) tcslen( footer );
}

void maschinen::list_view::ListViewGroup::setFooter( tstring& footer ) noexcept {
	group.mask |= LVGF_FOOTER;
	group.pszFooter = ( LPWSTR ) footer.c_str();
	group.cchFooter = ( int ) footer.length();
}

void maschinen::list_view::ListViewGroup::setFooter( tstring* footer ) noexcept {
	group.mask |= LVGF_FOOTER;
	group.pszFooter = ( LPWSTR ) footer->c_str();
	group.cchFooter = ( int ) footer->length();
}
//end set

//begin get
PLVGROUP maschinen::list_view::ListViewGroup::get() noexcept {return &group;}
std::unique_ptr<tstring> maschinen::list_view::ListViewGroup::getHeader() const noexcept { return std::make_unique<tstring>( group.pszHeader ); }
std::unique_ptr<tstring> maschinen::list_view::ListViewGroup::getFooter() const noexcept { return std::make_unique<tstring>( group.pszFooter ); }
int maschinen::list_view::ListViewGroup::getId() const noexcept { return group.iGroupId; }
//end get
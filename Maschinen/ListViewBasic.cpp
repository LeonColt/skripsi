#include "stdafx.h"
#include "ListViewBasic.h"


maschinen::list_view::ListViewBasic::ListViewBasic()
{
	jumlah_column = 0;
}

maschinen::list_view::ListViewBasic::ListViewBasic( UINT id, maschinen::ContainedController* parent) : maschinen::UncontainedController(id, parent){}
void maschinen::list_view::ListViewBasic::create(){handler = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, (LPCTSTR)NULL, style, getX(), getY(), getWidth(), getHeight(), getWindowHandler(), (HMENU)(UINT_PTR)getId(), getInstance(), this);}

void maschinen::list_view::ListViewBasic::setMask(DWORD mask)
{
	this->mask = mask;
}
void maschinen::list_view::ListViewBasic::setFlag(DWORD flag)
{
	this->flag = flag;
}

void maschinen::list_view::ListViewBasic::applyImageListView()
{
	ListView_SetImageList(handler, image_list.getImageList(), LVSIL_NORMAL);
}
void maschinen::list_view::ListViewBasic::applyImageListViewSmall()
{
	ListView_SetImageList(handler, image_list.getImageList(), LVSIL_SMALL);
}
void maschinen::list_view::ListViewBasic::applyImageListViewState()
{
	ListView_SetImageList(handler, image_list.getImageList(), LVSIL_STATE);
}
void maschinen::list_view::ListViewBasic::applyImageListViewGroupHeader()
{
	ListView_SetImageList(handler, image_list.getImageList(), LVSIL_GROUPHEADER);
}

std::unique_ptr<std::vector<int>> maschinen::list_view::ListViewBasic::getSelectedItemIndex() const noexcept {
	std::unique_ptr<std::vector<int>> res = std::make_unique<std::vector<int>>();
	int index = -1;
	do {
		index = ListView_GetNextItem( handler, index, LVNI_SELECTED );
		if ( index != -1 ) res->push_back( index );
	} while ( index != -1 );
	return res;
}

int maschinen::list_view::ListViewBasic::getFocusedItemIndex() const noexcept {return ListView_GetNextItem( handler, -1, LVNI_FOCUSED );}
void maschinen::list_view::ListViewBasic::deleteAllItems() const noexcept {ListView_DeleteAllItems(handler);}

void maschinen::list_view::ListViewBasic::twoClickActive(bool two_click_active)
{
	if (two_click_active) extended_style |= LVS_EX_TWOCLICKACTIVATE;
	else
	{
		extended_style |= LVS_EX_TWOCLICKACTIVATE;
		extended_style ^= LVS_EX_TWOCLICKACTIVATE;
	}
}

void maschinen::list_view::ListViewBasic::setStyleAlignTop( bool style ) {
	if ( style ) this->style |= LVS_ALIGNTOP;
	else {
		this->style |= LVS_ALIGNTOP;
		this->style ^= LVS_ALIGNTOP;
	}
}

void maschinen::list_view::ListViewBasic::setItemStateCutPaste(int position){}
void maschinen::list_view::ListViewBasic::setItemStateDragDrop(int position){}
void maschinen::list_view::ListViewBasic::setItemStateFocused(int position){}
void maschinen::list_view::ListViewBasic::setItemStateSelected(int position){}
UINT maschinen::list_view::ListViewBasic::getSelectedCount() const noexcept {return ListView_GetSelectedCount( handler );}
int maschinen::list_view::ListViewBasic::getGroupCount() const noexcept { return (int)ListView_GetGroupCount( handler ); }
bool maschinen::list_view::ListViewBasic::operator==( int type ) const noexcept { return type == UNKNOW_LIST_VIEW; }
bool maschinen::list_view::ListViewBasic::operator!=( int type ) const noexcept { return type == UNKNOW_LIST_VIEW; }
bool maschinen::list_view::ListViewBasic::compare( int type ) const noexcept { return type == UNKNOW_LIST_VIEW; }

maschinen::list_view::ListViewBasic::~ListViewBasic(){}
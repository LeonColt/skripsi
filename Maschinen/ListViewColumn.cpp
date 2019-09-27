#include "stdafx.h"
#include "ListViewColumn.h"


maschinen::list_view::ListViewColumn::ListViewColumn()
{
	column = { 0 };
}

void maschinen::list_view::ListViewColumn::setText( LPCTSTR text ) {
	column.mask = column.mask | LVCF_TEXT;
	column.pszText = (LPTSTR)text;
	column.cchTextMax = (int)tcslen(text);
}

void maschinen::list_view::ListViewColumn::setImage(int image_index)
{
	column.mask = column.mask | LVCF_IMAGE;
	column.iImage = image_index;
}

void maschinen::list_view::ListViewColumn::setWidth(int pixel)
{
	column.mask = column.mask | LVCF_WIDTH;
	column.cx = pixel;
}

void maschinen::list_view::ListViewColumn::setPosition(int position)
{
	column.mask = column.mask | LVCF_SUBITEM;
	index = position;
	column.iSubItem = index;
	
}

LPLVCOLUMN maschinen::list_view::ListViewColumn::get()
{
	return &column;
}

int maschinen::list_view::ListViewColumn::getPosition()
{
	return index;
}

void maschinen::list_view::ListViewColumn::textAlignLeft()
{
	column.mask = column.mask | LVCF_FMT;
	column.fmt = column.fmt | LVCFMT_LEFT;
}
void maschinen::list_view::ListViewColumn::textAlignRight()
{
	column.mask = column.mask | LVCF_FMT;
	column.fmt = column.fmt | LVCFMT_RIGHT;
}
void maschinen::list_view::ListViewColumn::textAlignCenter()
{
	column.mask = column.mask | LVCF_FMT;
	column.fmt = column.fmt | LVCFMT_CENTER;
}

void maschinen::list_view::ListViewColumn::applySplitButton()
{
	column.fmt = column.fmt | LVCFMT_SPLITBUTTON;
}

void maschinen::list_view::ListViewColumn::setText( tstring& text ) {setText( text.c_str() );}
void maschinen::list_view::ListViewColumn::setText( tstring* text ) {setText( text->c_str() );}

maschinen::list_view::ListViewColumn::~ListViewColumn()
{
}

#include "stdafx.h"
#include "ListViewImageList.h"

maschinen::list_view::ListViewImageList::ListViewImageList()
{
}

void maschinen::list_view::ListViewImageList::create(int init, int grow)
{
	if (flag == 0) flag = ILC_MASK;
	image_list = ImageList_Create(GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), flag, init, grow);
	if (image_list == NULL)
	{
		TCHAR buffer[MAX_LOADSTRING];
		OutputDebugStringW(L"ERROR code : ");
		_itow_s(GetLastError(), buffer, 10);
		OutputDebugStringW(buffer);
	}
}

void maschinen::list_view::ListViewImageList::create(int width, int height, int init, int grow)
{
	if (flag == 0) flag = ILC_MASK;
	image_list = ImageList_Create(width, height, flag, init, grow);
	if (image_list == NULL)
	{
		TCHAR buffer[MAX_LOADSTRING];
		OutputDebugStringW(L"ERROR code : ");
		_itow_s(GetLastError(), buffer, 10);
		OutputDebugStringW(buffer);
	}
}

void maschinen::list_view::ListViewImageList::create(int width, int height, DWORD flag, int init, int grow)
{
	image_list = ImageList_Create(width, height, flag, init, grow);
	if (image_list == NULL)
	{
		TCHAR buffer[MAX_LOADSTRING];
		OutputDebugStringW(L"ERROR code : ");
		_itow_s(GetLastError(), buffer, 10);
		OutputDebugStringW(buffer);
	}
}

void maschinen::list_view::ListViewImageList::addIcon(int image_id)
{
	ListViewImageList_image_container kontainer;
	kontainer.icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(image_id));
	assert(kontainer.icon != NULL);
	kontainer.index_in_image_list = ImageList_AddIcon(image_list, kontainer.icon);
	if (kontainer.index_in_image_list == -1)
	{
		TCHAR buffer[MAX_LOADSTRING];
		OutputDebugStringW(L"ERROR code : ");
		_itow_s(GetLastError(), buffer, 10);
		OutputDebugStringW(buffer);
	}
	images.push_back(kontainer);
}



void maschinen::list_view::ListViewImageList::applyMask() { flag = flag | ILC_MASK; }
void maschinen::list_view::ListViewImageList::applyColor()
{
	flag = flag ^ ILC_COLORDDB;
	flag = flag ^ ILC_COLOR4;
	flag = flag ^ ILC_COLOR8;
	flag = flag ^ ILC_COLOR16;
	flag = flag ^ ILC_COLOR24;
	flag = flag ^ ILC_COLOR32;
	flag = flag | ILC_COLOR;
}
void maschinen::list_view::ListViewImageList::applyColorDDB() {
	flag = flag ^ ILC_COLOR;
	flag = flag ^ ILC_COLOR4;
	flag = flag ^ ILC_COLOR8;
	flag = flag ^ ILC_COLOR16;
	flag = flag ^ ILC_COLOR24;
	flag = flag ^ ILC_COLOR32;
	flag = flag | ILC_COLORDDB;
}
void maschinen::list_view::ListViewImageList::applyColor4() {
	flag = flag ^ ILC_COLOR;
	flag = flag ^ ILC_COLORDDB;
	flag = flag ^ ILC_COLOR8;
	flag = flag ^ ILC_COLOR16;
	flag = flag ^ ILC_COLOR24;
	flag = flag ^ ILC_COLOR32;
	flag = flag ^ ILC_COLOR4;
}
void maschinen::list_view::ListViewImageList::applyColor8() {
	flag = flag ^ ILC_COLOR;
	flag = flag ^ ILC_COLORDDB;
	flag = flag ^ ILC_COLOR4;
	flag = flag ^ ILC_COLOR16;
	flag = flag ^ ILC_COLOR24;
	flag = flag ^ ILC_COLOR32;
	flag = flag | ILC_COLOR8;
}
void maschinen::list_view::ListViewImageList::applyColor16() {
	flag = flag ^ ILC_COLOR;
	flag = flag ^ ILC_COLORDDB;
	flag = flag ^ ILC_COLOR4;
	flag = flag ^ ILC_COLOR8;
	flag = flag ^ ILC_COLOR24;
	flag = flag ^ ILC_COLOR32;
	flag = flag | ILC_COLOR16;
}
void maschinen::list_view::ListViewImageList::applyColor24() {
	flag = flag ^ ILC_COLOR;
	flag = flag ^ ILC_COLORDDB;
	flag = flag ^ ILC_COLOR4;
	flag = flag ^ ILC_COLOR8;
	flag = flag ^ ILC_COLOR16;
	flag = flag ^ ILC_COLOR24;
	flag = flag ^ ILC_COLOR32;
	flag = flag | ILC_COLOR24;
}
void maschinen::list_view::ListViewImageList::applyColor32() {
	flag = flag ^ ILC_COLOR;
	flag = flag ^ ILC_COLORDDB;
	flag = flag ^ ILC_COLOR4;
	flag = flag ^ ILC_COLOR8;
	flag = flag ^ ILC_COLOR16;
	flag = flag ^ ILC_COLOR24;
	flag = flag | ILC_COLOR32;
}
void maschinen::list_view::ListViewImageList::applyPalette() { flag = flag | ILC_PALETTE; }
void maschinen::list_view::ListViewImageList::applyMirror() { flag = flag | ILC_MIRROR; }
void maschinen::list_view::ListViewImageList::applyPeritemMirror() { flag = flag | ILC_PERITEMMIRROR; }
void maschinen::list_view::ListViewImageList::applyOriginalMask() { flag = flag | ILC_ORIGINALSIZE; }
void maschinen::list_view::ListViewImageList::applyHighQualityScale() { flag = flag | ILC_HIGHQUALITYSCALE; }
HIMAGELIST& maschinen::list_view::ListViewImageList::getImageList() { return image_list; }
int maschinen::list_view::ListViewImageList::getImageIndex(int index)
{
	return images[index].index_in_image_list;
}


maschinen::list_view::ListViewImageList::~ListViewImageList()
{
	ImageList_Destroy(image_list);
	for (auto i = images.begin(); i != images.end(); i++)
	{
		DestroyIcon(i->icon);
	}
	images.clear();
}
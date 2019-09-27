#include "stdafx.h"
#include "Menu.h"


maschinen::menu::Menu::Menu()
{
}

void maschinen::menu::Menu::destroy() {if(handler != NULL) DestroyMenu(handler);}
void maschinen::menu::Menu::flagBitMap(){menu_flag |= MF_BITMAP;}
void maschinen::menu::Menu::flagChecked(){menu_flag |= MF_CHECKED;}
void maschinen::menu::Menu::flagDisabled(){menu_flag |= MF_DISABLED;}
void maschinen::menu::Menu::flagEnabled(){menu_flag |= MF_ENABLED;}
void maschinen::menu::Menu::flagGrayed(){menu_flag |= MF_GRAYED;}
void maschinen::menu::Menu::flagMenuBarBreak(){menu_flag |= MF_MENUBARBREAK;}
void maschinen::menu::Menu::flagMenuBreak(){menu_flag |= MF_MENUBREAK;}
void maschinen::menu::Menu::flagOwnerDraw(){menu_flag |= MF_OWNERDRAW;}
void maschinen::menu::Menu::flagPopUp(){menu_flag |= MF_POPUP;}
void maschinen::menu::Menu::flagSeperator(){menu_flag |= MF_SEPARATOR;}
void maschinen::menu::Menu::flagUnchecked(){menu_flag |= MF_UNCHECKED;}
void maschinen::menu::Menu::clearFlag(){menu_flag = 0;}

HMENU maschinen::menu::Menu::getHandler() {
	return handler;
}

maschinen::menu::Menu::~Menu()
{
	destroy();
}

#pragma once
#include "Maschinen.h"
#include "Menu.h"
#include "MenuItem.h"
#include "Window.h"
namespace maschinen
{
	namespace menu
	{
		class MenuItem;
		class MASCHINEN_API PopUpMenu : public maschinen::menu::Menu
		{
		private:
			struct MenuItemInformationen {
				MenuItem* item;
				bool visible;
				UINT position;
			};
			bool position_by_cursor = true;
			UINT flag_popup_menu;
			int x, y;
			std::vector<PopUpMenu*> sub_menus;
			std::vector<MenuItemInformationen*> items;
			maschinen::Window* parent;
		public:
			PopUpMenu();
			void create();
			void positionbyCursor(bool position_by_cursor = true);
			void show();
			void setOnClickListener( maschinen::event::OnClickListener* on_click_listener, bool auto_delete = false );
			void append( UINT id, LPCTSTR text );
			void append( maschinen::menu::MenuItem* m_item );
			void setVisible(UINT id, bool visible = true);
			void setVisibleVisible(std::vector<UINT> ids);
			void setVisibleInvisible( std::vector<UINT> ids );
			void setVisibleVisible( std::initializer_list<UINT> ids );
			void setVisibleInvisible( std::initializer_list<UINT> ids );
			void setEnable(UINT id, bool enable = true) const;
			void setGrayed(UINT id, bool grayed = true) const;
			void destroy();

			void setParent( maschinen::Window* parent );
			void flagPopUpMenuCenterAlign();
			void flagPopUpMenuLeftAlign();
			void flagPopUpMenuRightAlign();
			void flagPopUpMenuBottomAlign();
			void flagPopUpMenuTopAlign();
			void flagPopUpMenuVCenterAlign();
			void flagPopUpMenuNoNotify();
			void flagPopUpMenuLeftButton();
			void flagPopUpMenuRightButton();

			void flagPopUpMenuAnimationRightToLeft();
			void flagPopUpMenuAnimationLeftToRight();
			void flagPopUpMenuAnimationTopToBottom();
			void flagPopUpMenuAnimationBottomToTop();
			void flagPopUpMenuNoAnimation();

			void clear();

			~PopUpMenu();
		};
	}
}


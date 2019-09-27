#pragma once
#include "Maschinen.h"
#include "PopUpMenu.h"
#include "Bitmap.h"

namespace maschinen {
	namespace menu {
		class PopUpMenu;
		class MASCHINEN_API MenuItem {
		private:
			MENUITEMINFO item;
		public:
			MenuItem();
			void setSubMenu( maschinen::menu::PopUpMenu* sub_menu );
			void asIcon( UINT id, maschinen::Bitmap* bitmap );
			void asSeperator();
			void asText( UINT id, LPCTSTR text);
			void setImageForChecked( maschinen::Bitmap* bitmap );
			void setImageForUnchecked( maschinen::Bitmap* bitmap );
			void setData(LPVOID data);
			void setTextOrderRightToLeft(bool value = true);
			void setRightJustify(bool value = true);
			LPMENUITEMINFO get();
			void operator=(maschinen::menu::MenuItem* menu);
			void operator=( MENUITEMINFO item );
			UINT getId();
			~MenuItem();
		};
	}
}


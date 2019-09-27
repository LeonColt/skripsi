#pragma once
#include "Maschinen.h"
namespace maschinen
{
	namespace menu
	{
		class MASCHINEN_API Menu
		{
		protected:
			HMENU handler;
			UINT menu_flag = 0;
		public:
			Menu();
			virtual void create() = 0;
			virtual void destroy();
			void flagBitMap();
			void flagChecked();
			void flagDisabled();
			void flagEnabled();
			void flagGrayed();
			void flagMenuBarBreak();
			void flagMenuBreak();
			void flagOwnerDraw();
			void flagPopUp();
			void flagSeperator();
			void flagUnchecked();
			void clearFlag();
			virtual void show() = 0;
			HMENU getHandler();
			~Menu();
		};
	}
}
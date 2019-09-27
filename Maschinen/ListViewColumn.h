#pragma once
#include <CommCtrl.h>
#include "Maschinen.h"
namespace maschinen
{
	namespace list_view
	{
		class MASCHINEN_API ListViewColumn
		{
		private:
			UINT mask;
			int fmt;
			LVCOLUMN column;
			int index;
		public:
			ListViewColumn();
			void setText( LPCTSTR text );
			void setText( tstring& text );
			void setText( tstring* text );
			void setImage(int image_index);
			void setWidth(int pixel);
			void setPosition(int position);
			LPLVCOLUMN get();
			int getPosition();
			void textAlignLeft();
			void textAlignRight();
			void textAlignCenter();
			void applySplitButton();
			~ListViewColumn();
		};
	}
}


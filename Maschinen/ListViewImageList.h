#pragma once
#include <vector>
#include <CommCtrl.h>
#include <assert.h>
#include "Maschinen.h"
namespace maschinen
{
	namespace list_view
	{
		struct MASCHINEN_API ListViewImageList_image_container
		{
			HICON icon;
			int index_in_image_list;
		};
		class MASCHINEN_API ListViewImageList
		{
		private:
			UINT flag;
			HIMAGELIST image_list;
			std::vector<ListViewImageList_image_container> images;
		public:
			ListViewImageList();
			//begin:flag
			void applyMask();
			void applyColor();
			void applyColorDDB();
			void applyColor4();
			void applyColor8();
			void applyColor16();
			void applyColor24();
			void applyColor32();
			void applyPalette();
			void applyMirror();
			void applyPeritemMirror();
			void applyOriginalMask();
			void applyHighQualityScale();
			//end:flag

			void create(int init, int grow);
			void create(int width, int height, int init, int grow);
			void create(int width, int height, DWORD flag, int init, int grow);
			void addIcon(int image_id);
			int getImageIndex(int index);
			HIMAGELIST& getImageList();
			~ListViewImageList();
		};
	}
}


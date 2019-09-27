#pragma once
#include <CommCtrl.h>
#include "Maschinen.h"
namespace maschinen {
	namespace list_view {
		class MASCHINEN_API ListViewGroup {
		private:
			LVGROUP group;
		public:
			ListViewGroup();
			//begin set
			void setId( int id );
			void setHeader(LPCTSTR header) noexcept;
			void setHeader( tstring& header ) noexcept;
			void setHeader( tstring* header ) noexcept;
			void setFooter( LPCTSTR footer ) noexcept;
			void setFooter( tstring& footer ) noexcept;
			void setFooter( tstring* footer ) noexcept;
			//end set

			//begin get
			PLVGROUP get() noexcept;
			std::unique_ptr<tstring> getHeader() const noexcept;
			std::unique_ptr<tstring> getFooter() const noexcept;
			int getId() const noexcept;
			//end get
			~ListViewGroup();
		};
	}
}


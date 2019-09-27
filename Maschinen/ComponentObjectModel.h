#pragma once
#include <objbase.h>
#include <shtypes.h>
#include <ShlObj.h>
#include <comdef.h>
#include "Maschinen.h"
#include "Exception.h"
namespace maschinen {
	namespace com {
		class MASCHINEN_API ComponentObjectModel {
		private:
			HRESULT init_res;
		public:
			ComponentObjectModel( const DWORD co_init );
			HRESULT openFolderAndSelectItem(LPCTSTR path) const;
			HRESULT openFolderAndSelectItems( LPCTSTR path, const std::vector<tstring>* const files ) const;
			~ComponentObjectModel();
		};
	}
}


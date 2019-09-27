#pragma once
#include <ShlObj.h>
#include "Maschinen.h"
#include "Exception.h"
namespace maschinen {
	class MASCHINEN_API ClipBoard {
	private:
		BOOL state;
	public:
		ClipBoard();
		void empty();
		void setText( LPCTSTR text );
		void setText( tstring& text );
		void setPath(LPCTSTR path);
		void setPath( tstring& path );
		~ClipBoard();
	};
}


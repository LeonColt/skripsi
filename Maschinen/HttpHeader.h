#pragma once
#include "Maschinen.h"
namespace maschinen {
	namespace wininet {
		class MASCHINEN_API HttpHeader {
		private:
			tstring variable;
			tstring value;
		public:
			HttpHeader();
			void setVariable( LPCTSTR variable );
			void setValue( LPCTSTR value );

			void setVariable( tstring& variable );
			void setValue( tstring& value );

			void setVariable( tstring* variable );
			void setValue( tstring* value );

			void setVariable( const tstring& variable );
			void setValue( const tstring& value );

			void setVariable( const tstring* variable );
			void setValue( const tstring* value );
			

			const TCHAR* getVariable();
			const TCHAR* getValue();
			~HttpHeader();
		};
	}
}


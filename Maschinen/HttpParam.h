#pragma once
#include "Maschinen.h"
namespace maschinen
{
	namespace wininet
	{
		class MASCHINEN_API HttpParam
		{
		private:
			tstring variable, value;
		public:
			HttpParam();
			void setVariable(LPCTSTR variable) noexcept;
			void setValue(LPCTSTR value) noexcept;
			void setVariable( tstring& variable ) noexcept;
			void setValue( tstring& value ) noexcept;
			void setVariable( tstring* variable );
			void setValue( tstring* value );
			const TCHAR* getVariable();
			const TCHAR* getValue();
			~HttpParam();
		};
	}
}
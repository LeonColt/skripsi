#pragma once
#include "Maschinen.h"
#include "HttpHeader.h"
namespace maschinen {
	namespace wininet {
		class MASCHINEN_API HttpHeaders {
		private:
			std::vector<maschinen::wininet::HttpHeader> headers;
		public:
			HttpHeaders();
			void add( LPCTSTR variable, LPCTSTR value );
			void add( const tstring& variable, const tstring& value );
			void add( const tstring* variable, const tstring* value );

			void add( LPCTSTR variable, const tstring& value );
			void add( const tstring& variable, LPCTSTR value );

			void add( LPCTSTR variable, const tstring* value );
			void add( const tstring* variable, LPCTSTR value );

			void add( const tstring& variable, const tstring* value );
			void add( const tstring* variable, const tstring& value );

			void add( maschinen::wininet::HttpHeader& header );

			void add( maschinen::wininet::HttpHeader* header );

			std::unique_ptr<tstring> serialize();
			~HttpHeaders();
		};
	}
}


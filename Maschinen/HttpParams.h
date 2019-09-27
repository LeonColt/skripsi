#pragma once
#include <list>
#include <memory>
#include "Maschinen.h"
#include "HttpParam.h"
namespace maschinen
{
	namespace wininet
	{
		class MASCHINEN_API HttpParams
		{
		private:
			std::unique_ptr<std::vector<std::unique_ptr<maschinen::wininet::HttpParam>>> post_params;
			std::unique_ptr<std::vector<std::unique_ptr<maschinen::wininet::HttpParam>>> get_params;
			bool method_iterator = true;
		public:
			const bool METHOD_POST = true;
			const bool METHOD_GET = false;
			HttpParams();
			void add(LPCTSTR variable, LPCTSTR value, const bool method) const;
			void add( LPCTSTR variable, const tstring& value, const bool method ) const;
			void add( LPCTSTR variable, const tstring* value, const bool method ) const;
			void add( const tstring& variable, LPCTSTR value, const bool method ) const;
			void add( const tstring* variable, LPCTSTR value, const bool method ) const;

			void add( const tstring& variable, const tstring& value, const bool method ) const;
			void add( const tstring* variable, const tstring* value, const bool method ) const;
			void add( const tstring& variable, const tstring* value, const bool method ) const;
			void add( const tstring* variable, const tstring& value, const bool method ) const;

			void deleteParam( LPCTSTR variable, const bool method );
			void deleteParam( const tstring& variable, const bool method );
			void deleteParam( const tstring* variable, const bool method );
			std::vector<std::unique_ptr<maschinen::wininet::HttpParam>>::iterator begin(const bool method);
			std::vector<std::unique_ptr<maschinen::wininet::HttpParam>>::iterator end();
			void clear() const;
			~HttpParams();
		};
	}
}


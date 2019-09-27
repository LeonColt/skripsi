#pragma once
#include "Maschinen.h"
namespace maschinen
{
	class MASCHINEN_API Controller
	{
	private:
		UINT id;
	public:
		Controller();
		Controller( UINT id );
		inline void setId( UINT id){this->id = id;}
		inline UINT getId() const  noexcept{ return id; }
		virtual bool operator==( int type ) const noexcept;
		virtual bool operator!=(int type) const noexcept;
		virtual bool compare(int type) const noexcept;
		~Controller();
	};
}
#pragma once
#include "Maschinen.h"
namespace maschinen {
	namespace event {
		class MASCHINEN_API Event {
		private:
			const UINT ID;
		public:
			Event( const UINT id );
			const UINT getSourceId() const noexcept;
			~Event();
		};
	}
}
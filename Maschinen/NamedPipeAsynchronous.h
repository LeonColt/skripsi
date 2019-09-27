#pragma once
#include "Maschinen.h"
#include "NamedPipeBase.h"
namespace maschinen {
	namespace pipe {
		class MASCHINEN_API NamedPipeAsynchronous : public NamedPipeBase {
		private:
			std::vector<HANDLE> events;
		public:
			NamedPipeAsynchronous();
			HANDLE create();
			~NamedPipeAsynchronous();
		};
	}
}


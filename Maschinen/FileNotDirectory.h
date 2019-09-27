#pragma once
#include "Maschinen.h"
#include "Exception.h"
namespace maschinen {
	namespace exception {
		class MASCHINEN_API FileNotDirectory : public Exception {
		public:
			FileNotDirectory();
			~FileNotDirectory();
		};
	}
}


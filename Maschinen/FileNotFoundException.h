#pragma once
#include "Exception.h"
namespace maschinen {
	namespace exception {
		class FileNotFoundException : public maschinen::exception::Exception {
		public:
			FileNotFoundException();
			~FileNotFoundException();
		};
	}
}


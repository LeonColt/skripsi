#pragma once
#include "Maschinen.h"
#include "Exception.h"
namespace maschinen {
	namespace exception {
		class ReaderIteratorNullHandleException : public Exception {
		public:
			ReaderIteratorNullHandleException();
			~ReaderIteratorNullHandleException();
		};
	}
}


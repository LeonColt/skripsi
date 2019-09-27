#pragma once
#include "Maschinen.h"
#include "Exception.h"
namespace maschinen {
	namespace exception {
		class MASCHINEN_API ReaderIteratorNullBufferException : public Exception   {
		public:
			ReaderIteratorNullBufferException();
			~ReaderIteratorNullBufferException();
		};
	}
}


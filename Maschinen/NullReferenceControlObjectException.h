#pragma once
#include "Exception.h"
namespace maschinen {
	namespace exception {
		class Exception;
		class MASCHINEN_API NullReferenceControlObjectException : public maschinen::exception::Exception {
		public:
			NullReferenceControlObjectException() : Exception(L"Control Object is null", 0) {

			}
			~NullReferenceControlObjectException() {

			}
		};
	}
}


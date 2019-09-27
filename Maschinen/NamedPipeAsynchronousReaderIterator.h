#pragma once
#include "Maschinen.h"
namespace maschinen {
	namespace pipe {
		namespace iterator {
			class MASCHINEN_API NamedPipeAsynchronousReaderIterator {
			private:
				OVERLAPPED overlapped;
			public:
				NamedPipeAsynchronousReaderIterator();
				//void operator++();
				//bool operator!=();
				//bool operator==();
				~NamedPipeAsynchronousReaderIterator();
			};
		}
	}
}


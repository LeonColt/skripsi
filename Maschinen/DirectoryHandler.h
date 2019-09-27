#pragma once
#include <memory>
#include <string>
#include "Maschinen.h"
#include "DirectoryReadingIterator.h"
#include "Exception.h"
namespace maschinen {
	namespace file {
		class DirectoryReadingIterator;
		class MASCHINEN_API DirectoryHandler {
		private:
			tstring path;
			HANDLE handler = INVALID_HANDLE_VALUE;
			std::unique_ptr<DirectoryReadingIterator> reader;
			std::unique_ptr<DirectoryReadingIterator> finish_reader;
		public:
			DirectoryHandler();
			DirectoryHandler(LPCTSTR path);
			DirectoryHandler( tstring& path );
			size_t getNumberOfChildren();
			void setPath(LPCTSTR path);
			void setPath( tstring& path );
			DirectoryReadingIterator& begin();
			DirectoryReadingIterator& end() const;
			HANDLE& get();
			~DirectoryHandler();
		};
	}
}


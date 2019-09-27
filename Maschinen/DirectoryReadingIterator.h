#pragma once
#include "DirectoryHandler.h"
namespace maschinen {
	namespace file {
		class DirectoryHandler;
		class MASCHINEN_API DirectoryReadingIterator {
		public:
			DirectoryHandler* dh;
			DWORD file_attributes, nfile_size_high, nfile_size_low;
			FILETIME creation_time, last_access_time, last_write_time;
			tstring file_name;
			TCHAR alternate_file_name[ 14 ];
			DirectoryReadingIterator();
			void operator++();
			void operator++( int );
			bool operator!=( const DirectoryReadingIterator& finish_reader ) const;
			~DirectoryReadingIterator();
		};
	}
}


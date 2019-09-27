#pragma once
#include "FileHandleBase.h"
#include "Exception.h"
#include "ReaderIteratorAsynchronous.h"
namespace maschinen {
	namespace iterator {
		class ReaderIteratorAsynchronous;
	}
	namespace file {
		class MASCHINEN_API FileHandleAsynchronous : public FileHandleBase {
		private:
			LPVOID buffer;
			DWORD buffer_size;
			DWORD number_of_byte_read;
			std::unique_ptr<maschinen::iterator::ReaderIteratorAsynchronous> ria;
			std::unique_ptr<maschinen::iterator::ReaderIteratorAsynchronous> riaf;
		public:
			FileHandleAsynchronous();
			HANDLE createAlways( LPCTSTR file_name );
			HANDLE createNew( LPCTSTR file_name );
			HANDLE openAlways( LPCTSTR file_name );
			HANDLE openExisting( LPCTSTR file_name );
			HANDLE truncateExisting( LPCTSTR file_name );
			void setHandle( HANDLE handle );
			maschinen::iterator::ReaderIteratorAsynchronous& beginRead( LPVOID buffer, DWORD buffer_size );
			const maschinen::iterator::ReaderIteratorAsynchronous& endRead() const noexcept;
			~FileHandleAsynchronous();
		};
	}
}


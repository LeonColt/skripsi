#pragma once
#include "Maschinen.h"
#include "Exception.h"
#include "FileHandleAsynchronous.h"
namespace maschinen {
	namespace file {
		class FileHandleAsynchronous;
	}
	namespace iterator {
		class MASCHINEN_API ReaderIteratorAsynchronous {
			friend class maschinen::file::FileHandleAsynchronous;
		private:
			HANDLE handle;
			OVERLAPPED overlapped;
			LARGE_INTEGER pointer;

			DWORD read_state;
			DWORD overlapped_state;
		public:
			LPVOID buffer;
			DWORD buffer_size;
			DWORD number_of_byte_read;

			ReaderIteratorAsynchronous();
			bool isIncomplete() const noexcept;
			bool isComplete() const noexcept;
			bool isEndOfFile() const noexcept;
			DWORD getResultElse() const noexcept;
			DWORD getNumberOfByteRead() const noexcept;

			ReaderIteratorAsynchronous& operator++();
			ReaderIteratorAsynchronous operator++( int );

			bool operator!=( const ReaderIteratorAsynchronous& that ) const noexcept;
			bool operator==( const ReaderIteratorAsynchronous& that ) const noexcept;

			~ReaderIteratorAsynchronous();
		};
	}
}


#pragma once
#include "Maschinen.h"
#include "Exception.h"
#include "ReaderIteratorNullBufferException.h"
#include "ReaderIteratorNullHandleException.h"
namespace maschinen {
	namespace iterator {
		class MASCHINEN_API ReaderIterator {
		public:
			HANDLE handle;
			LPVOID buffer;
			DWORD buffer_size;
			DWORD number_of_byte_read;
			ReaderIterator();
			ReaderIterator& operator++();
			ReaderIterator operator++( int );
			ReaderIterator& preNext();
			ReaderIterator postNext();
			bool operator!=( const HANDLE finish_reader ) const noexcept;
			bool operator==( const HANDLE finish_reader ) const noexcept;
			bool operator!=( const ReaderIterator& finish_reader ) const noexcept;
			bool operator==( const ReaderIterator& finish_reader ) const noexcept;
			bool operator!=( const ReaderIterator* finish_reader ) const noexcept;
			bool operator==( const ReaderIterator* finish_reader ) const noexcept;
			void operator=( const ReaderIterator& that ) noexcept;
			void operator=( const ReaderIterator* that ) noexcept;
			~ReaderIterator();
		};
	}
}


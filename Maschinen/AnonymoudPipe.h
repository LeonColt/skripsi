#pragma once
#include "Maschinen.h"
#include "Exception.h"
#include "ReaderIterator.h"
namespace maschinen {
	namespace pipe {
		class MASCHINEN_API AnonymoudPipe {
		private:
			HANDLE in_read;
			HANDLE in_write;
			HANDLE out_read;
			HANDLE out_write;
			HANDLE handle;
			void createChildProcess( LPCTSTR command_line );
			maschinen::iterator::ReaderIterator reader;
			const maschinen::iterator::ReaderIterator freader;
		public:
			AnonymoudPipe( LPCTSTR command_line );
			void write( LPVOID buffer, DWORD size, DWORD& number_of_byte_written );
			maschinen::iterator::ReaderIterator& beginRead( LPVOID buffer, DWORD buffer_size );
			const maschinen::iterator::ReaderIterator& endRead() const noexcept;
			void closeRead();
			void closeWrite();
			void terminate( UINT exit_code );
			HANDLE getWrite();
			HANDLE getRead();
			HANDLE get();
			~AnonymoudPipe();
		};
	}
}


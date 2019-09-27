#pragma once
#include <memory>
#include <AclAPI.h>
#include "Maschinen.h"
#include "FileHandleBase.h"
#include "ReaderIterator.h"
#include "Exception.h"
namespace maschinen {
	namespace file {
		class MASCHINEN_API FileHandler {
		private:
			HANDLE handle = INVALID_HANDLE_VALUE;
			void close();
			maschinen::iterator::ReaderIterator reader;
			const maschinen::iterator::ReaderIterator finish_reader;
		public:
			FileHandler();
			FileHandler( LPCTSTR file_name, DWORD desired_access, DWORD share_mode, LPSECURITY_ATTRIBUTES security_attribute, DWORD creation_disposition, DWORD flag_and_attributes, HANDLE template_file_handler );
			FileHandler( tstring& file_name, DWORD desired_access, DWORD share_mode, LPSECURITY_ATTRIBUTES security_attribute, DWORD creation_disposition, DWORD flag_and_attributes, HANDLE template_file_handler );
			FileHandler( tstring* file_name, DWORD desired_access, DWORD share_mode, LPSECURITY_ATTRIBUTES security_attribute, DWORD creation_disposition, DWORD flag_and_attributes, HANDLE template_file_handler );

			FileHandler( LPCTSTR file_name, DWORD desired_access, DWORD share_mode, DWORD creation_disposition, DWORD flag_and_attributes );
			FileHandler( tstring& file_name, DWORD desired_access, DWORD share_mode, DWORD creation_disposition, DWORD flag_and_attributes );
			FileHandler( tstring* file_name, DWORD desired_access, DWORD share_mode, DWORD creation_disposition, DWORD flag_and_attributes );

			void init( LPCTSTR file_name,DWORD desired_access,DWORD share_mode, LPSECURITY_ATTRIBUTES security_attribute,DWORD creation_disposition,DWORD flag_and_attributes,HANDLE template_file_handler );
			void init( tstring& file_name,DWORD desired_access,DWORD share_mode, LPSECURITY_ATTRIBUTES security_attribute,DWORD creation_disposition,DWORD flag_and_attributes,HANDLE template_file_handler );
			void init( tstring* file_name,DWORD desired_access,DWORD share_mode, LPSECURITY_ATTRIBUTES security_attribute,DWORD creation_disposition,DWORD flag_and_attributes,HANDLE template_file_handler );

			void init( LPCTSTR file_name,DWORD desired_access,DWORD share_mode,DWORD creation_disposition,DWORD flag_and_attributes );
			void init( tstring& file_name,DWORD desired_access,DWORD share_mode,DWORD creation_disposition,DWORD flag_and_attributes );
			void init( tstring* file_name,DWORD desired_access,DWORD share_mode,DWORD creation_disposition,DWORD flag_and_attributes );

			maschinen::iterator::ReaderIterator& beginRead( LPVOID buffer, DWORD buffer_size );
			const maschinen::iterator::ReaderIterator& endRead() const;
			void setPointer(LONGLONG pointer);
			void setPointer( LARGE_INTEGER pointer );
			void write( const LPVOID buffer, const DWORD buffer_size, const LPDWORD number_of_byte_written, const LPOVERLAPPED lp_overlapped ) const;
			void write( const LPVOID buffer, const DWORD buffer_size, const LPDWORD number_of_byte_written) const;
			HANDLE get() const;
			void set( HANDLE handler );
			bool isValid() const;
			LONGLONG getSize();
			LONGLONG getSize( LPCTSTR path );
			LONGLONG getSize( tstring& path );
			LONGLONG getSize( tstring* path );
			std::unique_ptr<LARGE_INTEGER> getSizeEx();
			std::unique_ptr<tstring> getOwner();
			std::unique_ptr<FILETIME> getCreatedTime() const;
			std::unique_ptr<FILETIME> getCreatedTime( LPCTSTR path ) const;
			std::unique_ptr<FILETIME> getLastAccessTime() const;
			std::unique_ptr<FILETIME> getLastAccessTime( LPCTSTR path ) const;
			std::unique_ptr<FILETIME> getLastWriteTime() const;
			std::unique_ptr<FILETIME> getLastWriteTime( LPCTSTR path ) const;
			~FileHandler();
		};
	}
}


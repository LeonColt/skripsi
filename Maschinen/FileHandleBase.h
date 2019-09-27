#pragma once
#include "Maschinen.h"
#include "SecurityAttributes.h"
namespace maschinen {
	namespace file {
		class MASCHINEN_API FileHandleBase {
		protected:
			DWORD attrs_and_flags;
			maschinen::security::SecurityAttributes* security_attributes;
			DWORD share_mode;
			DWORD access;
			tstring file_name;
			HANDLE handle;
		public:
			FileHandleBase();
			FileHandleBase& shareDelete( bool share_delete = true ) noexcept;
			FileHandleBase& shareRead( bool share_read = true ) noexcept;
			FileHandleBase& shareWrite( bool share_write = true ) noexcept;
			FileHandleBase& shareAll() noexcept;
			FileHandleBase& setShareMode( DWORD share_mode ) noexcept;
			FileHandleBase& setSecurityAttributes( maschinen::security::SecurityAttributes* lp_security_attributes ) noexcept;

			FileHandleBase& attributeNormal( bool attr = true ) noexcept;
			FileHandleBase& attributeArchive( bool attr = true ) noexcept;
			FileHandleBase& attributeEncrypted( bool attr = true ) noexcept;
			FileHandleBase& attributeHidden( bool attr = true ) noexcept;
			FileHandleBase& attributeOffline( bool attr = true ) noexcept;
			FileHandleBase& attributeReadOnly( bool attr = true ) noexcept;
			FileHandleBase& attributeSystem( bool attr = true ) noexcept;
			FileHandleBase& attributeTemporary( bool attr = true ) noexcept;
			FileHandleBase& setAttributesAndFlags( DWORD attrs_and_flags );
			FileHandleBase& setAccessMode( DWORD access );

			virtual HANDLE createAlways( LPCTSTR file_name ) = 0;
			virtual HANDLE createNew( LPCTSTR file_name ) = 0;
			virtual HANDLE openAlways( LPCTSTR file_name ) = 0;
			virtual HANDLE openExisting( LPCTSTR file_name ) = 0;
			virtual HANDLE truncateExisting( LPCTSTR file_name ) = 0;

			HANDLE get() const noexcept;
			void set( HANDLE handle ) noexcept;
			void close();
			bool isValid() const noexcept;
			~FileHandleBase();
		};
	}
}


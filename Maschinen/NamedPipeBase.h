#pragma once
#include "Maschinen.h"
#include "SecurityAttributes.h"
namespace maschinen {
	namespace pipe {
		class MASCHINEN_API NamedPipeBase {
		private:
			HANDLE handle;
			tstring name;
			maschinen::security::SecurityAttributes* security_attributes;
		protected:
			DWORD open_mode, mode, max_instance, out_buffer_size, in_buffer_size, default_time_out;
		public:
			const DWORD INSTANCE_UNLIMITED;
			NamedPipeBase();
			void setName( LPCTSTR name );
			void setName( tstring& name );
			void setName( tstring* name );
			const TCHAR* getName() const noexcept;
			void setSecurityAttributes( maschinen::security::SecurityAttributes* security_attributes );
			maschinen::security::SecurityAttributes* getSecurityAttributes() const noexcept;
			void setPipeModeMessage( bool mode_message = true );
			void setPipeReadModeMessage(bool read_mode_message = true );
			void setPipeModeNoWait( bool mode_no_wait = true );
			void setMaxInstance( DWORD max_instance = PIPE_UNLIMITED_INSTANCES );
			void setInOutBufferSize( DWORD in_buffer_size, DWORD out_buffer_size );
			void setDefaultTimeOut( DWORD default_time_out );
			void setAccessDuplex();
			void setAccessInBound();
			void setAcessOutBound();
			void setFlagFirstInstance( bool flag_first_instance = true );
			void setFlagWriteThrough( bool flag_write_through = true );
			void set( HANDLE handle ) noexcept;
			HANDLE get() const noexcept;
			virtual HANDLE create() = 0;
			void close();
			~NamedPipeBase();
		};
	}
}


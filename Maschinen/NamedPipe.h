#pragma once
#include "Maschinen.h"
namespace maschinen {
	namespace pipe {
		class MASCHINEN_API NamedPipe {
		private:
			HANDLE handle;
		public:
			NamedPipe();
			NamedPipe( LPCTSTR name, DWORD open_mode, DWORD pipe_mode, DWORD max_instance, DWORD out_buffer_size, DWORD in_buffer_size, DWORD default_time_out );
			NamedPipe( tstring& name );
			NamedPipe( tstring* name );
			void connect();
			void close();
			bool isValid() const noexcept;
			HANDLE get() const noexcept;
			~NamedPipe();
		};
	}
}


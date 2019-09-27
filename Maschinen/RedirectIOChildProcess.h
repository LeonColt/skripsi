#pragma once
#include "Maschinen.h"
#include "Exception.h"
namespace maschinen {
	namespace pipe {
		class MASCHINEN_API RedirectIOChildProcess {
		private:
			SECURITY_ATTRIBUTES sa;
			tstring pipe_name;
			HANDLE out, in;
			HANDLE out_read, out_write, in_read, in_write, error_write;
			HANDLE handle;
		public:
			RedirectIOChildProcess( LPCTSTR pipe_name );
			void createProcess( LPTSTR command_line );
			void closeRead();
			void closeWrite();
			HANDLE getOutRead();
			HANDLE getOutWrite();
			void terminate(UINT exit_code);
			~RedirectIOChildProcess();
		};
	}
}


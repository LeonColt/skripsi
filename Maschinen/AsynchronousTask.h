#pragma once
#include <exception>
#include "InvisibleController.h"

#define ASYNTASK_SUCCESS 1
#define ASYNTASK_EXCEPTION 2
#define ASYNTASK_PROGRESS_PUBLISH 3
#define ASYNTASK_SET_RESULT 4
#define ASYNTASK_SET_ERROR 5
#define ASYNTASK_CANCEL 6

DWORD WINAPI eksekusiTask(LPVOID param);
namespace maschinen
{
	namespace thread
	{
		namespace task
		{
			class AsynchronousTask;
			struct AstTransporter
			{
				AsynchronousTask* ast = nullptr;
				LPVOID input = nullptr;
				LPVOID result = nullptr;
				LPVOID progress = nullptr;
				std::exception_ptr error = nullptr;
				HWND handler_message = NULL;
			};
			class InvisibleController;
			class MASCHINEN_API AsynchronousTask : public maschinen::InvisibleController
			{
				friend class Window;
			private:
				AstTransporter astp;
				HANDLE handle;
				HWND getWindowHandler();
				bool cancel_signer;
				friend LPVOID executeBackground(AsynchronousTask* ast, LPVOID input);
				LPVOID input = nullptr;
				bool auto_delete;
				bool flag_alive;
				friend void setState( AsynchronousTask* ast, bool state );
			protected:
				//method is called before task is executed
				virtual void onPreExecute();
				//method where task is executed
				virtual LPVOID doInBackground(LPVOID input) = 0;
				//method is called after task is executed
				virtual void onPostExecute(LPVOID result) = 0;
				//method is called when publishProgess is executed
				virtual void onProgressUpdate(LPVOID progress, bool on_task = false);
				//method is called when task is cancelled
				virtual void onCancelled(LPVOID result);
				//to publis progress task is executed, onProgressUpdate will executed in caller thread
				void publishProgress(LPVOID progress);
				//to publis progress task is executed
				void publishProgressOnTask(LPVOID progress);
				//method is called before task is interrupted
				virtual void beforeInterrupt();
				//method is called after task is interrupted
				virtual void afterInterrupt();
			public:
				AsynchronousTask();
				void execute();
				void execute(LPVOID input);
				void cancel(bool may_interrupt_if_running);
				bool isCanceled() const noexcept;

				bool operator==( int type ) const noexcept;
				bool operator!=( int type ) const noexcept;
				bool compare( int type ) const noexcept;
				void setAutoDelete( bool auto_delete = true );
				HANDLE get() const noexcept;
				bool isAlive();
				~AsynchronousTask();
			};
		}
	}
}


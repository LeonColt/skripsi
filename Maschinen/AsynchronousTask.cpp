#include "stdafx.h"
#include "AsynchronousTask.h"

DWORD WINAPI eksekusiTask(LPVOID param)
{
	maschinen::thread::task::AstTransporter* astp = (maschinen::thread::task::AstTransporter*)param;
	maschinen::thread::task::AsynchronousTask* ast = astp->ast;
	maschinen::thread::task::setState( ast, true );
	try {
		astp->result = maschinen::thread::task::executeBackground( ast, astp->input );
		if ( ast->isCanceled() ) {
			maschinen::thread::task::setState( ast, false );
			PostMessage( astp->handler_message, WM_ASYNCHRONOUS_TASK, ASYNTASK_CANCEL, ( LPARAM ) astp );
		}
		else {
			maschinen::thread::task::setState( ast, false );
			PostMessage( astp->handler_message, WM_ASYNCHRONOUS_TASK, ASYNTASK_SUCCESS, ( LPARAM ) astp );
		}
	}
	catch ( ... ) {
		astp->error = std::current_exception();
		maschinen::thread::task::setState( ast, false );
		PostMessage( astp->handler_message, WM_ASYNCHRONOUS_TASK, ASYNTASK_EXCEPTION, ( LPARAM ) astp );
	}
	maschinen::thread::task::setState( ast, false );
	return 0;
}

maschinen::thread::task::AsynchronousTask::AsynchronousTask() { auto_delete = false; flag_alive = false; }

void maschinen::thread::task::AsynchronousTask::execute()
{
	cancel_signer = false;
	if (!cancel_signer) onPreExecute();
	if (!cancel_signer)
	{
		astp.ast = this;
		astp.input = this->input;
		astp.handler_message = getWindowHandler();
		handle = CreateThread(NULL, NULL, eksekusiTask, &astp, NULL, NULL);
	}
}

void maschinen::thread::task::AsynchronousTask::execute(LPVOID input)
{
	this->input = input;
	execute();
}

void maschinen::thread::task::AsynchronousTask::publishProgress(LPVOID progress)
{
	astp.ast = this;
	astp.progress = progress;
	PostMessage(getWindowHandler(), WM_ASYNCHRONOUS_TASK, ASYNTASK_PROGRESS_PUBLISH, (LPARAM)&astp);
}

void maschinen::thread::task::AsynchronousTask::publishProgressOnTask(LPVOID progress)
{
	onProgressUpdate(progress, true);
}

HWND maschinen::thread::task::AsynchronousTask::getWindowHandler()
{
	maschinen::ContainedController* temp = getParent();
	while (!temp->compare(WINDOW))temp = temp->getParent();
	return temp->getHandler();
}

LPVOID maschinen::thread::task::executeBackground(AsynchronousTask* ast, LPVOID input)
{
	return ast->doInBackground(input);
}

void maschinen::thread::task::setState( AsynchronousTask* ast, bool state ) {
	ast->flag_alive = state;
}

void maschinen::thread::task::AsynchronousTask::cancel(bool may_interrupt_if_running)
{
	cancel_signer = true;
	if (may_interrupt_if_running)
	{
		beforeInterrupt();
		TerminateThread(handle, 0);
		afterInterrupt();
		flag_alive = false;
	}
}

bool maschinen::thread::task::AsynchronousTask::isCanceled() const noexcept{return cancel_signer;}

void maschinen::thread::task::AsynchronousTask::onPreExecute() {}
void maschinen::thread::task::AsynchronousTask::onProgressUpdate(LPVOID progress, bool on_task) {}
void maschinen::thread::task::AsynchronousTask::onCancelled(LPVOID result) {}
void maschinen::thread::task::AsynchronousTask::beforeInterrupt(){}
void maschinen::thread::task::AsynchronousTask::afterInterrupt(){}

bool maschinen::thread::task::AsynchronousTask::operator==( int type ) const noexcept { return type == ASYNC_TASK_TYPE; }
bool maschinen::thread::task::AsynchronousTask::operator!=( int type ) const noexcept { return type == ASYNC_TASK_TYPE; }
bool maschinen::thread::task::AsynchronousTask::compare( int type ) const noexcept { return type == ASYNC_TASK_TYPE; }
void maschinen::thread::task::AsynchronousTask::setAutoDelete( bool auto_delete ) {
	this->auto_delete = auto_delete;
}
HANDLE maschinen::thread::task::AsynchronousTask::get() const noexcept {return handle;}
maschinen::thread::task::AsynchronousTask::~AsynchronousTask() { CloseHandle( handle ); }
bool maschinen::thread::task::AsynchronousTask::isAlive() { return flag_alive; }

#pragma once
#include <memory>
#include "CustomWindow.h"
#include "LayoutMatrix.h"
#include "Akey.h"
#include "Setting.h"
#include "UserCredential.h"
#include "Login.h"
#include "Registry.h"
#include "FileControl.h"
#include "Resource.h"
#include "Notification.h"
#include "WindowsInternet.h"
#include "WininetConnect.h"
#include "FileOperationWindow.h"
#include "FileUploadOperationWindow.h"
#include "OnClickListener.h"
#include "StatusBar.h"
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
class Login;
class Registry;
class FileControl;
class MainWindow : public CustomWindow, maschinen::event::OnClickListener
{
private:
	std::unique_ptr<Login> login;
	std::unique_ptr<Registry> registry;
	std::unique_ptr<FileControl> file_control;
	maschinen::StatusBar status;
	FileOperationWindow fow;
	FileUploadOperationWindow fuow;
	std::vector < std::unique_ptr<MainWindow>> siblings;
	maschinen::Notification::Notification notification;
	void onShow();
	void onCreate();
	void onDestroy();
	void onClose();
	bool onAdditional(UINT message, WPARAM wparam, LPARAM lparam);
	//void onPaint( HDC hdc );
public:
	MainWindow();
	void showRegistry();
	void showLogin();
	void showFileControl();
	void loginSuccess();
	void showWindowDownload();
	void showWindowUpload();
	void onClick( maschinen::event::OnClickEvent& e );
	maschinen::StatusBar& getStatus();
	~MainWindow();
};


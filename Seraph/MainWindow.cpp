#include "stdafx.h"
#include "MainWindow.h"

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

MainWindow::MainWindow(){}

void MainWindow::onShow() {
	if( login.get() != nullptr) login->show();
	if ( file_control.get() != nullptr ) file_control->show();
}

void MainWindow::showRegistry() {
	if (!user_credential->isCredential())
	{
		login->hide();
		file_control->hide();
		registry->show();
	}
}

void MainWindow::showLogin() {
	if (!user_credential->isCredential())
	{
		registry->hide();
		login->show();
	}
}

void MainWindow::showFileControl() {
	if(!registry->isDestroyed()) registry->hide();
	if ( user_credential->isCredential())
	{
		if (!login->isDestroyed()) login->hide();
	}
	file_control->show();
}

void MainWindow::onDestroy()
{
	try {
		setting->write();
	} catch ( std::exception& e ) {
		TCHAR error[ MAX_LOADSTRING ];
		size_t size = strlen( e.what() );
		mbstowcs_s( &size, error, e.what(), MAX_LOADSTRING );
		MessageBox( NULL, error, L"Error Write Setting", MB_OK | MB_ICONERROR );
	}
	fow.destroy();
	fuow.destroy();
	notification.remove();
	if(file_control.get() != nullptr ) file_control->stopRefresher();
	maschinen::Window::onDestroy();
}

bool MainWindow::onAdditional(UINT message, WPARAM wparam, LPARAM lparam)
{
	if (message == WM_MASCHINEN )
	{
		if (LOWORD(lparam) == WM_LBUTTONDBLCLK)
		{
			show();
			return true;
		}
	}
	return false;
}

void MainWindow::onClose()
{
	hide();
	notification.setWindow(this);
	notification.setId(1500);
	notification.setIcon(LoadIcon(getInstance(), MAKEINTRESOURCE(IDI_SERAPH)));
	notification.setEventId(WM_MASCHINEN);
	notification.setTip(L"Seraph");
	notification.add();
}

void MainWindow::onCreate()
{
	status.setParentAndId( this, MAIN_WINDOW_STATUS_BAR );
	status.create();

	maschinen::layout::LayoutMatrix* layout_login = new maschinen::layout::LayoutMatrix();
	layout_login->setNumberOfCell( 70, 1 );
	login = std::make_unique<Login>();
	login->setLayout( layout_login );
	login->setParentAndId(this, unit_login);
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( login.get(), 0, 0, 0, 0, 2, 0 );
	login->setSetting(setting);
	login->setAkey(akey);
	login->setUserCredential( user_credential );
	login->create();

	maschinen::layout::LayoutMatrix* layout_registry = new maschinen::layout::LayoutMatrix();
	layout_registry->setNumberOfCell( 30, 25 );
	registry = std::make_unique<Registry>();
	registry->setParentAndId(this, unit_registration);
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( registry.get(), 0, 0, 0, 0, 49, 0 );
	registry->setLayout( layout_registry );
	registry->setSetting(setting);
	registry->setAkey(akey);
	registry->setUserCredential( user_credential );
	registry->create();

	maschinen::layout::LayoutMatrix* layout_file_control = new maschinen::layout::LayoutMatrix();
	layout_file_control->setNumberOfCell( 70, 70 );
	file_control = std::make_unique<FileControl>();
	file_control->setParentAndId(this, unit_file_control);
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( file_control.get(), 0, 3, 0, 0, 46, 0 );
	file_control->setLayout( layout_file_control );
	file_control->setSetting(setting);
	file_control->setAkey(akey);
	file_control->setUserCredential( user_credential );
	file_control->setStatusBar( &status );
	file_control->create();

	try {
		maschinen::layout::LayoutMatrix* layout_fow = new maschinen::layout::LayoutMatrix();
		layout_fow->setNumberOfCell( 1, 50 );
		fow.setLayout( layout_fow );
		fow.setStyle( CS_HREDRAW | CS_VREDRAW );
		fow.setIcon( LoadIcon( getInstance(), MAKEINTRESOURCE( IDI_SERAPH ) ) );
		fow.setCursor( LoadCursor( nullptr, IDC_ARROW ) );
		fow.setBackground( ( HBRUSH ) ( COLOR_WINDOW + 1 ) );
		//fdw.setMenuName(MAKEINTRESOURCE(IDC_SERAPH));
		fow.setSmallIcon( LoadIcon( getInstance(), MAKEINTRESOURCE( IDI_SMALL ) ) );
		fow.setAkey( akey );
		fow.setSetting( setting );
		fow.setUserCredential( user_credential );
		fow.create();
		fow.setTitle( L"Download" );
	} catch ( maschinen::exception::Exception& e ) {
		TCHAR error[ MAX_LOADSTRING ];
		e.getMessage( error, MAX_LOADSTRING );
		MessageBox( NULL, error, L"error", MB_OK | MB_ICONERROR );
		SeraphException se;
		se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
	}

	try {
		maschinen::layout::LayoutMatrix* layout_fuow = new maschinen::layout::LayoutMatrix();
		layout_fuow->setNumberOfCell( 1, 50 );
		fuow.setLayout( layout_fuow );
		fuow.setStyle( CS_HREDRAW | CS_VREDRAW );
		fuow.setIcon( LoadIcon( getInstance(), MAKEINTRESOURCE( IDI_SERAPH ) ) );
		fuow.setCursor( LoadCursor( nullptr, IDC_ARROW ) );
		fuow.setBackground( ( HBRUSH ) ( COLOR_WINDOW + 1 ) );
		//fdw.setMenuName(MAKEINTRESOURCE(IDC_SERAPH));
		fuow.setSmallIcon( LoadIcon( getInstance(), MAKEINTRESOURCE( IDI_SMALL ) ) );
		fuow.setAkey( akey );
		fuow.setSetting( setting );
		fuow.setUserCredential( user_credential );
		fuow.create();
		fuow.setTitle( L"Upload" );
	} catch ( maschinen::exception::Exception& e ) {
		MessageBox( NULL, e.getMessage()->c_str(), L"error", MB_OK | MB_ICONERROR );
		SeraphException se;
		se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
	}
	setMenuOnClickListener( this, IDM_FILE_DOWNLOAD, this, false );
	setMenuOnClickListener( this, IDM_FILE_UPLOAD, this, false );
	setMenuOnClickListener( this, IDM_ABOUT, this, false );
	setMenuOnClickListener( this, IDM_EXIT, this, false );
}

void MainWindow::loginSuccess()
{
	login->hide();
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( file_control.get(), 0, 0, 0, FILL_PARENT, FILL_PARENT, 0 );
	invalidate();
	file_control->restartRequest();
	file_control->updateListView();
	HMENU sub_menu = GetSubMenu( GetMenu( handler ), 0 );
	InsertMenu( sub_menu, 2, MF_BYPOSITION | MF_STRING, (UINT_PTR)MENU_UNIT_LOGIN_LOGOUT, TEXT( "Logout" ) );
	setMenuOnClickListener( this, MENU_UNIT_LOGIN_LOGOUT, this, false );
}

void MainWindow::showWindowDownload() {
	fow.show();	
}

void MainWindow::showWindowUpload() {
	fuow.show();
}

void MainWindow::onClick( maschinen::event::OnClickEvent& e ) {
	switch ( e.getSourceId() ) {
		case IDM_FILE_DOWNLOAD: fow.show();break;
		case IDM_FILE_UPLOAD: fuow.show(); break;
		case IDM_ABOUT: DialogBox( getInstance(), MAKEINTRESOURCE( IDD_ABOUTBOX ), handler, About ); break;
		case MENU_UNIT_LOGIN_LOGOUT:
		{
			user_credential->uncredential();
			login->show();
			( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( file_control.get(), 0, 3, 0, 0, 46, 0 );
			invalidate();
			file_control->restartRequest();
			file_control->updateListView();
			HMENU sub_menu = GetSubMenu( GetMenu( handler ), 0 );
			DeleteMenu( sub_menu, MENU_UNIT_LOGIN_LOGOUT, MF_BYCOMMAND );
			//unSetMenuOnClickListener( this, MENU_UNIT_LOGIN_LOGOUT );
		} break;
		case IDM_EXIT: destroy(); break;
	}
}

maschinen::StatusBar& MainWindow::getStatus() {
	return status;
}

/*
void MainWindow::onPaint( HDC hdc ) {
	RECT rect;
	GetClientRect( handler, &rect );
	OutputDebugStringW( L"rect bottom : " );
	OutputDebugStringW( to_tstring( rect.bottom ).c_str() );
	OutputDebugStringW( L"\n" );
	OutputDebugStringW( L"rect top : " );
	OutputDebugStringW( to_tstring( rect.top ).c_str() );
	OutputDebugStringW( L"\n" );
	OutputDebugStringW( L"height window : " );
	OutputDebugStringW( to_tstring( rect.bottom - rect.top ).c_str() );
	OutputDebugStringW( L"\n" );
	OutputDebugStringW( L"height per cell : " );
	OutputDebugStringW( to_tstring( ( LONG ) ( ( rect.bottom - rect.top ) / ( ( maschinen::layout::LayoutMatrix* )getLayout() )->getNumberOfCellPerHeight() ) ).c_str() );
	OutputDebugStringW( L"\n" );
	HPEN drawer;
	HPEN hpen;
	COLORREF line_color;
	line_color = RGB( 255, 0, 0 );
	hpen = CreatePen( PS_SOLID, 1, line_color );
	drawer = ( HPEN ) SelectObject( hdc, hpen );
	for ( LONG i = 0; i < ( ( maschinen::layout::LayoutMatrix* )getLayout() )->getNumberOfCellPerHeight(); i++ ) {
		for ( LONG j = 0; j < ( ( maschinen::layout::LayoutMatrix* )getLayout() )->getNumberOfCellPerWidth(); j++ ) {
			std::unique_ptr<Position> pos = std::move( ( ( maschinen::layout::LayoutMatrix* )getLayout() )->getCoordinate( j, i, 0, 0, 0, 0 ) );
			if ( i == 0 && j == 0 ) {
				OutputDebugStringW( L"first row : " );
				OutputDebugStringW( to_tstring( pos->height ).c_str() );
				OutputDebugStringW( L"\n" );
			}
			else if( i == ( ( maschinen::layout::LayoutMatrix* )getLayout() )->getNumberOfCellPerHeight() - 1) {
				OutputDebugStringW( L"last row : " );
				OutputDebugStringW( to_tstring( pos->height ).c_str() );
				OutputDebugStringW( L"\n" );
			}
			else {
				OutputDebugStringW( to_tstring( i ).c_str() );
				OutputDebugStringW( L" row : " );
				OutputDebugStringW( to_tstring( pos->height ).c_str() );
				OutputDebugStringW( L"\n" );
			}
			MoveToEx( hdc, pos->x, pos->y, NULL );
			LineTo( hdc, pos->x + pos->width, pos->y );
			MoveToEx( hdc, pos->x + pos->width, pos->y, NULL );
			LineTo( hdc, pos->x + pos->width, pos->y + pos->height );
			MoveToEx( hdc, pos->x + pos->width, pos->y + pos->height, NULL );
			LineTo( hdc, pos->x, pos->y + pos->height );
			MoveToEx( hdc, pos->x, pos->y + pos->height, NULL );
			LineTo( hdc, pos->x, pos->y );
		}
	}


	SelectObject( hdc, drawer );
	DeleteObject( hpen );
}
*/
MainWindow::~MainWindow()
{
}

// Seraph.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Seraph.h"

// Global Variables:
HINSTANCE hInst;                                // current instance
Akey akey;
Setting setting;
UserCredential uc;
maschinen::Maschinen handler;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	setInstance( hInstance );
	try {
		setting.read();
	} catch ( SeraphException& e ) {
		MessageBox( NULL, e.getMessage()->c_str(), L"Error Reading Setting", MB_ICONERROR | MB_OK );
		return 0;
	} catch ( maschinen::exception::Exception& e ) {
		MessageBox( NULL, e.getMessage()->c_str(), L"Error Reading Setting", MB_ICONERROR | MB_OK );
		return 0;
	} catch ( std::exception& e ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		size_t size = strlen( e.what() );
		mbstowcs_s( &size, buffer, e.what(), MAX_LOADSTRING );
		MessageBox( NULL, buffer, L"Error Reading Setting", MB_ICONERROR | MB_OK );
		return 0;
	}
	try {
		akey.setSetting( &setting );
		akey.execute();
	} catch ( SeraphException& e ) {
		MessageBox( NULL, e.getMessage()->c_str(), L"Error Program Key", MB_ICONERROR | MB_OK );
		return 0;
	} catch ( maschinen::exception::Exception& e ) {
		MessageBox( NULL, e.getMessage()->c_str(), L"Error Program Key", MB_ICONERROR | MB_OK );
		return 0;
	}
	MainWindow main_window;
	WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
	WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SERAPH, szWindowClass, MAX_LOADSTRING);

	setWindowClassNameSeed( szWindowClass );

	InitCommonControls();

	try {
		//main_window.setStyleHorizontalRedraw();
		//main_window.setStyleVerticalRedraw();
		maschinen::layout::LayoutMatrix* layout = new maschinen::layout::LayoutMatrix();
		layout->setNumberOfCell( 1, 50 );
		main_window.setStyle( CS_HREDRAW | CS_VREDRAW );
		main_window.setIcon( LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SERAPH ) ) );
		main_window.setCursor( LoadCursor( nullptr, IDC_ARROW ) );
		main_window.setBackground( ( HBRUSH ) ( COLOR_WINDOW + 1 ) );
		main_window.setMenuName( MAKEINTRESOURCE( IDC_SERAPH ) );
		main_window.setSmallIcon( LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SMALL ) ) );
		main_window.setLayout( layout );
		main_window.setAkey( &akey );
		main_window.setSetting( &setting );
		main_window.setUserCredential( &uc );

		hInst = hInstance; // Store instance handle in our global variable
		main_window.create();
		main_window.setTitle( szTitle );

		if ( !main_window.getHandler() ) {
			TCHAR buffer[ MAX_LOADSTRING ];
			getLastErrorAsString( buffer, MAX_LOADSTRING );
			MessageBox( NULL, buffer, L"Error", MB_ICONERROR | MB_OK );
			return FALSE;
		}
	} catch ( maschinen::exception::Exception& e ) {
		TCHAR error[ MAX_LOADSTRING ];
		e.getMessage( error, MAX_LOADSTRING );
		MessageBox( NULL, error, L"Error", MB_ICONERROR | MB_OK );
		SeraphException se;
		se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
	}
	handler.setAccelarator(LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SERAPH)));
	main_window.show();
	handler.beginProcess();
	return handler.getResultStatus();
}
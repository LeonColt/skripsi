#include "stdafx.h"
#include "Window.h"

int maschinen::Window::jumlah_windows = 0;// dieksekusi sekali selama program berjalan, tidak peduli berapa kali class window di inisialisasi karena static
bool maschinen::Window::process_started = false;

DWORD WINAPI initCell(LPVOID lpparameter)
{
	maschinen::InitCellTransporter* initator = (maschinen::InitCellTransporter*)lpparameter;
	bool hasil;
	BOOL inner, outer;
	do
	{
		outer = GetWindowRect( initator->handler, &initator->rect );
		inner = GetClientRect(initator->handler, &initator->inner_rect);
		hasil = true;
		if ( inner && outer ) {
			hasil = true;
			if ( initator->rect.bottom == 0 && initator->rect.left == 0 && initator->rect.right == 0 && initator->rect.top == 0 )
				hasil = false;
			if ( initator->inner_rect.bottom == 0 && initator->inner_rect.left == 0 && initator->inner_rect.right == 0 && initator->inner_rect.top == 0 )
				hasil = false;
		}
	} while (!hasil);
	PostMessage(initator->handler, WM_SHOWED, NULL, NULL);
	return 0;
}

maschinen::Window::Window()
{
	ZeroMemory( &window_class, sizeof( WNDCLASSEX ) );
	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.lpfnWndProc = WindowProcessStarter;
	window_class.hInstance = getInstance();
}

void maschinen::Window::create()
{
	tstring class_name;
	class_name.append( window_class_name_seed.c_str() );
	class_name.append( TEXT( "-" ) );
	class_name.append( to_tstring( window_class_name_counter++ ).c_str() );
	window_class.lpszClassName = class_name.c_str();
	ATOM window_id = RegisterClassEx( &window_class );
	setId( window_id );
	if ( window_id == 0 ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
	wek = std::make_unique<WindowEventKeeper>();
	wek->window_id = window_id;
	listener_keepers.push_back( wek.get() );
	children = std::make_unique<Children>( this );
	window_children.push_back( children.get() );
	if(getX() == 0 && getY() == 0 && getWidth() == 0 && getHeight() == 0)
		handler = CreateWindowEx(WS_EX_CLIENTEDGE, window_class.lpszClassName, LPCTSTR(NULL), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, window_class.hInstance, this);
	else if (getX() == 0 && getY() == 0)
		handler = CreateWindowEx(WS_EX_CLIENTEDGE, window_class.lpszClassName, LPCTSTR(NULL), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, getWidth(), getHeight(), nullptr, nullptr, window_class.hInstance, this);
	else if(getWidth() == 0 && getHeight() == 0)
		handler = CreateWindowEx(WS_EX_CLIENTEDGE, window_class.lpszClassName, LPCTSTR(NULL), WS_OVERLAPPEDWINDOW, getX(), getY(), CW_USEDEFAULT, 0, nullptr, nullptr, window_class.hInstance, this);
	else handler = CreateWindowEx(WS_EX_CLIENTEDGE, window_class.lpszClassName, LPCTSTR(NULL), WS_OVERLAPPEDWINDOW, getX(), getY(), getWidth(), getHeight(), nullptr, nullptr, window_class.hInstance, this);
	if (handler == NULL)
	{
		listener_keepers.erase( std::remove( listener_keepers.begin(), listener_keepers.end(), wek.get() ), listener_keepers.end() );
		window_children.erase( std::remove( window_children.begin(), window_children.end(), children.get() ), window_children.end() );
		TCHAR buffer[MAX_LOADSTRING];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
}

LRESULT CALLBACK maschinen::Window::WindowProcessStarter(HWND handler, UINT message, WPARAM wparam, LPARAM lparam)
{
	Window* window;
	if (message == WM_NCCREATE)
	{
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lparam);
		window = static_cast<Window*>(lpcs->lpCreateParams);
		SetWindowLongPtr( handler, GWLP_USERDATA, reinterpret_cast< LONG_PTR >( window ) );
	}
	else window = reinterpret_cast<Window*>(GetWindowLongPtr(handler, GWLP_USERDATA));
	if (window) return window->WindowProcess(handler, message, wparam, lparam);
	else return DefWindowProc(handler, message, wparam, lparam);
}

void maschinen::Window::setWindowProcess(WNDPROC window_process){window_class.lpfnWndProc = window_process;}
void maschinen::Window::setExtraByte(int extra_byte){window_class.cbClsExtra = extra_byte;}
void maschinen::Window::setWindowExtraByte(int window_extra_byte){window_class.cbWndExtra = window_extra_byte;}
void maschinen::Window::setIcon(HICON icon){window_class.hIcon = icon;}
void maschinen::Window::setSmallIcon(HICON small_icon){window_class.hIconSm = small_icon;}
void maschinen::Window::setCursor(HCURSOR cursor){window_class.hCursor = cursor;}
void maschinen::Window::setBackground(HBRUSH background){window_class.hbrBackground = background;}
void maschinen::Window::setMenuName(LPCTSTR menu_name){window_class.lpszMenuName = menu_name;}
void maschinen::Window::setParent(Window* window){SetParent(handler, window->getHandler());}
void maschinen::Window::update() const noexcept { UpdateWindow( handler ); }
void maschinen::Window::invalidate() noexcept { 
	InvalidateRect( handler, NULL, FALSE ); 
	onSize( NULL, NULL );
}

void maschinen::Window::setStyle(UINT style) noexcept {window_class.style = style;}

void maschinen::Window::setStyleByteAlignClient(bool byte_align_client)
{
	if (byte_align_client) window_class.style |= CS_BYTEALIGNCLIENT;
	else
	{
		window_class.style |= CS_BYTEALIGNCLIENT;
		window_class.style ^= CS_BYTEALIGNCLIENT;
	}
}

void maschinen::Window::setStyleByteAlignWindow(bool byte_align_window)
{
	if (byte_align_window) window_class.style |= CS_BYTEALIGNWINDOW;
	else
	{
		window_class.style |= CS_BYTEALIGNWINDOW;
		window_class.style ^= CS_BYTEALIGNWINDOW;
	}
}

void maschinen::Window::setStyleClassDC(bool class_dc)
{
	if (class_dc) window_class.style |= CS_DBLCLKS;
	else
	{
		window_class.style |= CS_DBLCLKS;
		window_class.style ^= CS_DBLCLKS;
	}
}
void maschinen::Window::setStyleDoubleClicks(bool double_clicks)
{
	if (double_clicks) window_class.style |= CS_DBLCLKS;
	else
	{
		window_class.style |= CS_DBLCLKS;
		window_class.style ^= CS_DBLCLKS;
	}
}

void maschinen::Window::setStyleDropShadow(bool drop_shadow)
{
	if (drop_shadow) window_class.style |= CS_DROPSHADOW;
	else
	{
		window_class.style |= CS_DROPSHADOW;
		window_class.style ^= CS_DROPSHADOW;
	}
}

void maschinen::Window::setStyleGlobalClass(bool global_class)
{
	if (global_class) window_class.style |= CS_GLOBALCLASS;
	else
	{
		window_class.style |= CS_GLOBALCLASS;
		window_class.style ^= CS_GLOBALCLASS;
	}
}
void maschinen::Window::setStyleHorizontalRedraw(bool horizontal_redraw)
{
	if (horizontal_redraw) window_class.style |= CS_HREDRAW;
	else
	{
		window_class.style |= CS_HREDRAW;
		window_class.style ^= CS_HREDRAW;
	}
}

void maschinen::Window::setStyleVerticalRedraw(bool vertical_redraw)
{
	if (vertical_redraw) window_class.style |= CS_VREDRAW;
	else
	{
		window_class.style |= CS_VREDRAW;
		window_class.style ^= CS_VREDRAW;
	}
}

void maschinen::Window::setStyleNoClose(bool noclose)
{
	if (noclose) window_class.style |= CS_NOCLOSE;
	else
	{
		window_class.style |= CS_NOCLOSE;
		window_class.style ^= CS_NOCLOSE;
	}
}

void maschinen::Window::setStyleOwnDC(bool own_dc)
{
	if (own_dc) window_class.style |= CS_OWNDC;
	else
	{
		window_class.style |= CS_OWNDC;
		window_class.style ^= CS_OWNDC;
	}
}

void maschinen::Window::setStyleParentDC(bool parent_dc)
{
	if (parent_dc) window_class.style |= CS_PARENTDC;
	else
	{
		window_class.style |= CS_PARENTDC;
		window_class.style ^= CS_PARENTDC;
	}
}

void maschinen::Window::setStyleSaveBits(bool save_bits)
{
	if (save_bits) window_class.style |= CS_SAVEBITS;
	else
	{
		window_class.style |= CS_SAVEBITS;
		window_class.style ^= CS_SAVEBITS;
	}
}

LRESULT maschinen::Window::WindowProcess(HWND handler, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		this->handler = handler;
		onCreate();
		on_init = true;
		jumlah_windows++;
	} break;
	case WM_SHOWWINDOW:
	{
		if ( wparam == TRUE && lparam == 0 ) {
			if ( lparam == 0 ) {
				if ( on_init ) onCalculate();
				else onShow();
			}
			else return DefWindowProc( handler, message, wparam, lparam );
		}
		else onHide();
	} break;

	case WM_SHOWED:
	{
		onInit();
		on_init = false;
		onShow();
	} break;
	case WM_SIZE:
	{
		if ( !on_init && ( wparam == SIZE_MAXIMIZED || wparam == SIZE_RESTORED ) ) onSize( wparam, lparam );
	} break;
	case WM_COMMAND:
	{
		switch ( HIWORD( wparam ) ) {
			case EN_UPDATE:
			{
				for ( auto i = wek->edit_text_changed_listener_keeper.cbegin(); i != wek->edit_text_changed_listener_keeper.cend(); ++i ) {
					if ( ( *i )->id == LOWORD( wparam ) ) {
						( *i )->listener->beforeTextChanged();
						return 0;
					}
				}
			} break;

			case EN_CHANGE:
			{
				for ( auto i = wek->edit_text_changed_listener_keeper.cbegin(); i != wek->edit_text_changed_listener_keeper.cend(); ++i ) {
					if ( ( *i )->id == LOWORD( wparam ) ) {
						( *i )->listener->afterTextChanged();
						return 0;
					}
				}
			} break;

			default:
			{
				for ( auto i = wek->click_listener_keepers.cbegin(); i != wek->click_listener_keepers.cend(); ++i ) {
					if ( ( *i )->id == LOWORD( wparam ) ) {
						( *i )->ocl->onClick( maschinen::event::OnClickEvent( LOWORD( wparam ) ) );
						return 0;
					}
				}
				for ( auto i = wek->checked_change_listener_keeper.cbegin(); i != wek->checked_change_listener_keeper.cend(); ++i ) {
					if ( ( *i )->id == LOWORD( wparam ) ) {
						for ( auto j = children->beginCheckBox(); j != children->endCheckBox(); ++j ) {
							if ( ( *j )->control->getId() == LOWORD( wparam ) ) {
								( *j )->control->isChecked() ? ( *j )->control->setChecked( false ) : ( *j )->control->setChecked();
								( *i )->occl->onCheckedChanged( ( *j )->control, ( *j )->control->isChecked() );
								return 0;
							}
						}
					}
				}
			} break;
		}
	} break;
	case WM_NOTIFY:
	{
		LPNMHDR lpnmhdr = ( LPNMHDR ) lparam;
		for ( auto i = wek->table_select_keeper.cbegin(); i != wek->table_select_keeper.cend(); ++i ) {
			if ( ( *i )->id == lpnmhdr->idFrom ) {
				for ( auto j = children->beginTable(); j != children->endTable(); ++j ) {
					if ( ( *j )->control->getId() == lpnmhdr->idFrom ) {
						( *i )->listener->onListViewSelect( maschinen::event::OnListViewSelectEvent( (UINT)lpnmhdr->idFrom, lpnmhdr->code, ( *j )->control ) );
						return 0;
					}
				}
			}
		}
	} break;
	case WM_PAINT: { 
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint( handler, &ps );
		onPaint( hdc );
		EndPaint( handler, &ps );
		return 0;
	}
	case WM_CLOSE:
	{
		onClose();
	} break;
	case WM_DESTROY:
	{
		onDestroy();
		jumlah_windows--;
		if (jumlah_windows < 1) PostQuitMessage(0);
	} break;

	case WM_ASYNCHRONOUS_TASK:
	{
		maschinen::thread::task::AstTransporter* astp = ( maschinen::thread::task::AstTransporter* )lparam;
		switch ( wparam ) {
			case ASYNTASK_SUCCESS: astp->ast->onPostExecute( astp->result ); break;
			case ASYNTASK_PROGRESS_PUBLISH: astp->ast->onProgressUpdate( astp->progress ); break;
			case ASYNTASK_EXCEPTION: std::rethrow_exception( astp->error ); break;
			case ASYNTASK_CANCEL: astp->ast->onCancelled( astp->result ); break;
		}
		if ( astp->ast->auto_delete && wparam != ASYNTASK_PROGRESS_PUBLISH ) {
			delete astp->ast; astp->ast = nullptr;
		}
	} break;

	default:
	{
		if ( !onAdditional( message, wparam, lparam ) ) return DefWindowProc( handler, message, wparam, lparam ); else return 0;
	} break;
	}
	return 0;
}

void maschinen::Window::onCalculate() {
	ict.handler = handler;
	CreateThread( NULL, 0, initCell, &ict, NULL, 0 );
}

void maschinen::Window::onInit()
{
	wlayout->calculate(&ict.inner_rect);
	for ( auto i = children->beginContainer(); i != children->endContainer(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) {
			std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
			( *i )->control->setPosition( pos->x, pos->y, pos->width, pos->height );
		}
	}
	for ( auto i = children->beginLabel(); i != children->endLabel(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) {
			std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
			( *i )->control->setPosition( pos->x, pos->y, pos->width, pos->height );
		}
	}
	for ( auto i = children->beginButton(); i != children->endButton(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) {
			std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
			( *i )->control->setPosition( pos->x, pos->y, pos->width, pos->height );
		}
	}
	for ( auto i = children->beginEdit(); i != children->endEdit(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) {
			std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
			( *i )->control->setPosition( pos->x, pos->y, pos->width, pos->height );
		}
	}
	for ( auto i = children->beginCheckBox(); i != children->endCheckBox(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) {
			std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
			( *i )->control->setPosition( pos->x, pos->y, pos->width, pos->height );
		}
	}
	for ( auto i = children->beginProgressBar(); i != children->endProgressBar(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) {
			std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
			( *i )->control->setPosition( pos->x, pos->y, pos->width, pos->height );
		}
	}
	for ( auto i = children->beginTable(); i != children->endTable(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) {
			std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
			( *i )->control->setPosition( pos->x, pos->y, pos->width, pos->height );
		}
	}
	for ( auto i = children->beginCustomControl(); i != children->endCustomControl(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) {
			std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
			( *i )->control->setPosition( pos->x, pos->y, pos->width, pos->height );
		}
	}
	on_init = false;
}

void maschinen::Window::onSize( WPARAM wparam, LPARAM lparam ) 
{
	if ( wparam != SIZE_MINIMIZED ) {
		wlayout->calculate();
		size_t number_of_children = 0;
		for ( auto i = children->beginContainer(); i != children->endContainer(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) number_of_children++;
		}
		for ( auto i = children->beginLabel(); i != children->endLabel(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) number_of_children++;
		}
		for ( auto i = children->beginButton(); i != children->endButton(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) number_of_children++;
		}
		for ( auto i = children->beginEdit(); i != children->endEdit(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) number_of_children++;
		}
		for ( auto i = children->beginCheckBox(); i != children->endCheckBox(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) number_of_children++;
		}
		for ( auto i = children->beginProgressBar(); i != children->endProgressBar(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) number_of_children++;
		}
		for ( auto i = children->beginTable(); i != children->endTable(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) number_of_children++;
		}
		for ( auto i = children->beginCustomControl(); i != children->endCustomControl(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) number_of_children++;
		}
		HDWP hdwp = BeginDeferWindowPos( ( int ) number_of_children );
		for ( auto i = children->beginContainer(); i != children->endContainer(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) {
				std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
				DeferWindowPos( hdwp, ( *i )->control->getHandler(), NULL, pos->x, pos->y, pos->width, pos->height, SWP_NOZORDER );
			}
		}
		for ( auto i = children->beginLabel(); i != children->endLabel(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) {
				std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
				DeferWindowPos( hdwp, ( *i )->control->getHandler(), NULL, pos->x, pos->y, pos->width, pos->height, SWP_NOZORDER );
			}
		}
		for ( auto i = children->beginButton(); i != children->endButton(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) {
				std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
				DeferWindowPos( hdwp, ( *i )->control->getHandler(), NULL, pos->x, pos->y, pos->width, pos->height, SWP_NOZORDER );
			}
		}
		for ( auto i = children->beginEdit(); i != children->endEdit(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) {
				std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
				DeferWindowPos( hdwp, ( *i )->control->getHandler(), NULL, pos->x, pos->y, pos->width, pos->height, SWP_NOZORDER );
			}
		}
		for ( auto i = children->beginCheckBox(); i != children->endCheckBox(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) {
				std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
				DeferWindowPos( hdwp, ( *i )->control->getHandler(), NULL, pos->x, pos->y, pos->width, pos->height, SWP_NOZORDER );
			}
		}
		for ( auto i = children->beginProgressBar(); i != children->endProgressBar(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) {
				std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
				DeferWindowPos( hdwp, ( *i )->control->getHandler(), NULL, pos->x, pos->y, pos->width, pos->height, SWP_NOZORDER );
			}
		}
		for ( auto i = children->beginTable(); i != children->endTable(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) {
				std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
				DeferWindowPos( hdwp, ( *i )->control->getHandler(), NULL, pos->x, pos->y, pos->width, pos->height, SWP_NOZORDER );
			}
		}
		for ( auto i = children->beginCustomControl(); i != children->endCustomControl(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) {
				std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
				DeferWindowPos( hdwp, ( *i )->control->getHandler(), NULL, pos->x, pos->y, pos->width, pos->height, SWP_NOZORDER );
			}
		}
		EndDeferWindowPos( hdwp );

		for ( auto i = children->beginStatusBar(); i != children->endStatusBar(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) {
				SendMessage( ( *i )->control->getHandler(), WM_SIZE, wparam, lparam );
			}
		}
	}
}

bool maschinen::Window::onAdditional(UINT message, WPARAM wparam, LPARAM lparam){return false;}
void maschinen::Window::onClose(){DestroyWindow(handler);}
maschinen::Window* maschinen::Window::getParent() const noexcept {return ((maschinen::Window*)parent);}
maschinen::Children* maschinen::Window::getChildren() const noexcept {return children.get();}
WindowEventKeeper* maschinen::Window::getListeners() const noexcept { return wek.get(); }

void maschinen::Window::onHide() {
	for ( auto i = children->beginContainer(); i != children->endContainer(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->hide();
	}
	for ( auto i = children->beginLabel(); i != children->endLabel(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->hide();
	}
	for ( auto i = children->beginButton(); i != children->endButton(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->hide();
	}
	for ( auto i = children->beginCheckBox(); i != children->endCheckBox(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->hide();
	}
	for ( auto i = children->beginProgressBar(); i != children->endProgressBar(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->hide();
	}
	for ( auto i = children->beginEdit(); i != children->endEdit(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->hide();
	}
	for ( auto i = children->beginTable(); i != children->endTable(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->hide();
	}
	for ( auto i = children->beginCustomControl(); i != children->endCustomControl(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->hide();
	}
}

void maschinen::Window::onShow() {
	for ( auto i = children->beginContainer(); i != children->endContainer(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->show();
	}
	for ( auto i = children->beginLabel(); i != children->endLabel(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->show();
	}
	for ( auto i = children->beginButton(); i != children->endButton(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->show();
	}
	for ( auto i = children->beginCheckBox(); i != children->endCheckBox(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->show();
	}
	for ( auto i = children->beginProgressBar(); i != children->endProgressBar(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->show();
	}
	for ( auto i = children->beginEdit(); i != children->endEdit(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->show();
	}
	for ( auto i = children->beginTable(); i != children->endTable(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->show();
	}
	for ( auto i = children->beginCustomControl(); i != children->endCustomControl(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->show();
	}
}

void maschinen::Window::onDestroy() {
	for ( auto i = wek->click_listener_keepers.begin(); i != wek->click_listener_keepers.end(); ++i ) {
		if( *i != nullptr ) {
			if ( ( *i )->auto_delete )
				delete ( *i )->ocl;
			delete *i;
		}
	}
	wek->click_listener_keepers.clear();

	for ( auto i = wek->checked_change_listener_keeper.cbegin(); i != wek->checked_change_listener_keeper.cend(); ++i ) {
		if ( ( *i )->auto_delete ) delete ( *i )->occl;
		delete *i;
	}
	wek->checked_change_listener_keeper.clear();

	for ( auto i = wek->edit_text_changed_listener_keeper.cbegin(); i != wek->edit_text_changed_listener_keeper.cend(); ++i ) {
		if ( ( *i )->auto_delete ) delete ( *i )->listener;
		delete *i;
	}
	wek->edit_text_changed_listener_keeper.clear();

	for ( auto i = wek->table_select_keeper.cbegin(); i != wek->table_select_keeper.cend(); ++i ) {
		if ( ( *i )->auto_delete ) delete ( *i )->listener;
		delete *i;
	}
	wek->table_select_keeper.clear();

	listener_keepers.erase( std::remove( listener_keepers.begin(), listener_keepers.end(), wek.get() ), listener_keepers.end() );
	children.reset();
}

maschinen::Window::~Window(){}

void maschinen::Window::setTitle( LPCTSTR title ) {SetWindowText( handler, title );}
void maschinen::Window::setTitle( tstring& title ) {SetWindowText( handler, title.c_str() );}
void maschinen::Window::setTitle( tstring* title ) {SetWindowText( handler, title->c_str() );}


bool maschinen::Window::operator==( int type )  const noexcept { return type == WINDOW; }
bool maschinen::Window::operator!=( int type )  const noexcept { return type != WINDOW; }
bool maschinen::Window::compare( int type )  const noexcept { return type == WINDOW; }
bool maschinen::Window::operator==( maschinen::Window* window ) const noexcept {
	if ( window == nullptr ) return false;
	return getId() == window->getId();
}
bool maschinen::Window::operator!=( maschinen::Window* window ) const noexcept {
	if ( window == nullptr ) return false;
	return getId() != window->getId();
}
bool maschinen::Window::compare( maschinen::Window* window ) const noexcept {
	if ( window == nullptr ) return false;
	return getId() == window->getId();
}



bool maschinen::Window::operator==( maschinen::ContainedController& control ) const noexcept {
	if ( !control.compare( WINDOW ) ) return false;
	return getId() == ( ( maschinen::Window* )&control )->getId();
}
bool maschinen::Window::operator!=( maschinen::ContainedController& control ) const noexcept {
	if ( !control.compare( WINDOW ) ) return false;
	return getId() == ( ( maschinen::Window* )&control )->getId();
}
bool maschinen::Window::compare( maschinen::ContainedController& control ) const noexcept {
	if ( !control.compare( WINDOW ) ) return false;
	return getId() == ( ( maschinen::Window* )&control )->getId();
}
bool maschinen::Window::operator==( maschinen::ContainedController* control ) const noexcept {
	if ( control == nullptr ) return false;
	if ( !control->compare( WINDOW ) ) return false;
	return getId() == ( ( maschinen::Window* )control )->getId();
}
bool maschinen::Window::operator!=( maschinen::ContainedController* control ) const noexcept {
	if ( control == nullptr ) return false;
	if ( !control->compare( WINDOW ) ) return false;
	return getId() == ( ( maschinen::Window* )control )->getId();
}
bool maschinen::Window::compare( maschinen::ContainedController* control ) const noexcept {
	if ( control == nullptr ) return false;
	if ( !control->compare( WINDOW ) ) return false;
	return getId() == ( ( maschinen::Window* )control )->getId();
}
#pragma once
#include "Controller.h"
namespace maschinen
{
	class MASCHINEN_API VisibleController : public maschinen::Controller
	{
	private:
		Position pos;
	protected:
		HWND handler;
		DWORD window_style, style;
		VisibleController* parent = nullptr;
	public:
		VisibleController();
		VisibleController( UINT id, maschinen::VisibleController* parent);
		inline HWND getHandler() const noexcept { return handler; }
		inline LONG getX() const noexcept { return pos.x; }
		inline LONG getY() const noexcept { return pos.y; }
		inline LONG getWidth() const noexcept { return pos.width; }
		inline LONG getHeight() const noexcept { return pos.height; }
		void setPosition( LONG x, LONG y, LONG width, LONG height ) noexcept;
		void setStyleClipSibling( bool style = true );
		void enable() const noexcept;
		void disable() const noexcept;
		virtual void create() = 0;
		void destroy() noexcept;
		bool isDestroyed() const noexcept;
		bool isVisible() const noexcept;
		//begin show window
		virtual void show() const noexcept;
		void hide() const noexcept;
		void forceMinimize() const noexcept;
		void maximize() const noexcept;
		void minimize() const noexcept;
		void restore() const noexcept;
		void showDefault() const noexcept;
		//end show window
		~VisibleController();
	};
}


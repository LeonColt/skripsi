#pragma once
#include "UncontainedController.h"
namespace maschinen
{
	class MASCHINEN_API ProgressBar : public maschinen::UncontainedController
	{
	private:
		bool marquee = false;
		static LRESULT CALLBACK ProgressBarProcessStarter( HWND handler, UINT message, WPARAM wparam, LPARAM lparam, UINT_PTR uid_sub_class, DWORD_PTR dw_ref_data );
	protected:
		virtual LRESULT ProgressBarProcess( HWND handler, UINT message, WPARAM wparam, LPARAM lparam );
	public:
		ProgressBar();
		ProgressBar(UINT id, ContainedController* parent);
		void setParent(ContainedController* parent);
		void setParentAndId(ContainedController* parent, UINT id);
		void create();
		void styleMarquee(bool marquee = true);
		void setMarqueeSpeed(bool on, int speed);
		bool operator==( int type ) const noexcept;
		bool operator!=( int type ) const noexcept;
		bool compare( int type ) const noexcept;
		~ProgressBar();
	};
}


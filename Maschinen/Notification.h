#pragma once
#include <shellapi.h>
#include "Window.h"
namespace maschinen
{
	namespace Notification
	{
		class MASCHINEN_API Notification
		{
		private:
			NOTIFYICONDATA nid;
			maschinen::Window* window;
		public:
			Notification();
			void setWindow(maschinen::Window* window);
			void setId(UINT id);
			void setEventId(UINT event_id);
			void setIcon(HICON icon);
			void setTip(LPCTSTR tip);
			void setIconHidden();
			void setIconShared();
			void setInfo(LPCTSTR info);
			void setInfoTitle(LPCTSTR info_title);
			void setTimeOut(UINT timeout);
			void setVerionPrior();
			void setVersion2000();
			void setBaloonIcon(HICON icon);
			void setBaloonNoIcon();
			void setBaloonInfo();
			void setBaloonWarning();
			void setBaloonError();
			void setBaloonUser();
			void setBaloonNoSound();
			void setBaloonLargeIcon();
			void setBaloonRespectQuietTime();
			void setGUID(LPGUID guid);
			BOOL add();
			BOOL modify();
			BOOL remove();
			BOOL focus();
			~Notification();
		};
	}
}


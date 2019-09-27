#include "stdafx.h"
#include "Notification.h"


maschinen::Notification::Notification::Notification()
{
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.uVersion = NOTIFYICON_VERSION_4;
}

void maschinen::Notification::Notification::setWindow(maschinen::Window* window)
{
	this->window = window;
	nid.hWnd = window->getHandler();
}

void maschinen::Notification::Notification::setId(UINT id)
{
	nid.uID = id;
}

void maschinen::Notification::Notification::setEventId(UINT event_id)
{
	nid.uCallbackMessage = event_id;
	nid.uFlags |= NIF_MESSAGE;
}

void maschinen::Notification::Notification::setIcon(HICON icon)
{
	nid.hIcon = icon;
	nid.uFlags |= NIF_ICON;
}

void maschinen::Notification::Notification::setTip(LPCTSTR tip)
{
	wcscpy_s(nid.szTip, tip);
	nid.uFlags |= NIF_TIP;
}

void maschinen::Notification::Notification::setIconHidden()
{
	nid.dwState |= NIS_HIDDEN;
	nid.dwStateMask |= NIS_HIDDEN;
	nid.uFlags |= NIF_STATE;
}

void maschinen::Notification::Notification::setIconShared()
{
	nid.dwState |= NIS_SHAREDICON;
	nid.dwStateMask |= NIS_SHAREDICON;
	nid.uFlags = NIF_STATE;
}

void maschinen::Notification::Notification::setInfo(LPCTSTR info)
{
	wcscpy_s(nid.szInfo, info);
	nid.uFlags = NIF_INFO;
}

void maschinen::Notification::Notification::setInfoTitle(LPCTSTR info_title)
{
	wcscpy_s(nid.szInfoTitle, info_title);
	nid.uFlags = NIF_INFO;
}

void maschinen::Notification::Notification::setTimeOut(UINT timeout)
{
	nid.uTimeout = timeout;
	nid.uFlags = NIF_INFO;
}

void maschinen::Notification::Notification::setVerionPrior()
{
	nid.uVersion = 0;
}

void maschinen::Notification::Notification::setVersion2000()
{
	nid.uVersion = NOTIFYICON_VERSION;
}

void maschinen::Notification::Notification::setBaloonIcon(HICON icon)
{
	nid.hBalloonIcon = icon;
}

void maschinen::Notification::Notification::setBaloonNoIcon()
{
	nid.dwInfoFlags = NIIF_NONE;
	nid.uFlags = NIF_ICON;
}

void maschinen::Notification::Notification::setBaloonInfo()
{
	nid.dwInfoFlags = NIIF_INFO;
	nid.uFlags = NIF_ICON;
}

void maschinen::Notification::Notification::setBaloonWarning()
{
	nid.dwInfoFlags = NIIF_WARNING;
	nid.uFlags = NIF_ICON;
}
void maschinen::Notification::Notification::setBaloonError()
{
	nid.dwInfoFlags = NIIF_ERROR;
	nid.uFlags = NIF_ICON;
}

void maschinen::Notification::Notification::setBaloonUser()
{
	nid.dwInfoFlags = NIIF_USER;
	nid.uFlags = NIF_ICON;
}

void maschinen::Notification::Notification::setBaloonNoSound()
{
	nid.dwInfoFlags = NIIF_LARGE_ICON;
	nid.uFlags = NIF_ICON;
}

void maschinen::Notification::Notification::setBaloonLargeIcon()
{
	nid.uFlags = NIF_ICON;
}

void maschinen::Notification::Notification::setBaloonRespectQuietTime()
{
	nid.dwInfoFlags = NIIF_RESPECT_QUIET_TIME;
	nid.uFlags = NIF_ICON;
}

void maschinen::Notification::Notification::setGUID(LPGUID guid)
{
	nid.uFlags = NIF_GUID;
	nid.guidItem = *guid;
}

BOOL maschinen::Notification::Notification::add()
{
	return Shell_NotifyIcon(NIM_ADD, &nid);
}

BOOL maschinen::Notification::Notification::modify()
{
	return Shell_NotifyIcon(NIM_MODIFY, &nid);
}

BOOL maschinen::Notification::Notification::remove()
{
	return Shell_NotifyIcon(NIM_DELETE, &nid);
}

BOOL maschinen::Notification::Notification::focus()
{
	return Shell_NotifyIcon(NIM_SETFOCUS, &nid);
}

maschinen::Notification::Notification::~Notification()
{
}

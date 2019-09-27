#pragma once
#include <string>
#include "Maschinen.h"
class UserCredential
{
private:
	const int NONE, SUPERADMIN, ADMIN = 2, MEMBER = 3;
	tstring php_session_id, login_id, username, email;
	QWORD benutzer_id;
	TCHAR request_get_file[MAX_LOADSTRING] = L"";
	TCHAR request_download_file[MAX_LOADSTRING] = L"";
	int privilege_group;
public:
	UserCredential();
	void setPhpSessionId( LPCTSTR php_session_id );
	tstring& getPhpSessionId();
	void setBenutzerID( LONGLONG benutzer_id );
	QWORD getBenutzerID() const;
	void setLoginId(LPCTSTR login_id);
	void setUserName(LPCTSTR username);
	std::unique_ptr<tstring> getUserName() const noexcept;
	std::unique_ptr<tstring> getLoginId() const noexcept;
	void setEmail(LPCTSTR email);
	void setRequestGetFile(LPCTSTR request_get_file);
	void setRequestDownloadFiles(LPCTSTR request_download_file);
	bool isCredential();
	bool read();
	bool write();

	void setGroupPrivilege( int privilege_group );
	int getGroupPrivilege();
	bool isSuperAdmin();
	bool isAdmin();
	bool isMember();

	void uncredential() noexcept;

	~UserCredential();
};


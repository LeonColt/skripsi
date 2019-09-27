// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MASCHINEN_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MASCHINEN_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once
#ifdef MASCHINEN_EXPORTS
#define MASCHINEN_API __declspec(dllexport)
//begin common_constanta
#define MAX_LOADSTRING 2048
#define FILL_PARENT 65535

#ifdef _UNICODE
#define tstring std::wstring
#define tstrSplit(source, result, delimiter) wstrSplit(source, result, delimiter)
#define to_tstring(PARAM) std::to_wstring(PARAM)
#define tcslen(PARAM) wcslen(PARAM)
#define tcscmp(PARAM1, PARAM2) wcscmp(PARAM1, PARAM2)
#define tstrToLower(PARAM) wstrToLower(PARAM)
#define tstrToUpper(PARAM) wstrToUpper(PARAM)
#else
#define tstring std::string
#define tstrSplit(source, result, delimiter) strSplit(source, result, delimiter)
#define to_tstring(PARAM) std::to_string(PARAM)
#define tcslen(PARAM) strlen(PARAM)
#define tcscmp(PARAM1, PARAM2) strcmp(PARAM1, PARAM2)
#define tstrToLower(PARAM) strToLower(PARAM)
#define tstrToUpper(PARAM) strToUpper(PARAM)
#endif // _UNICODE

//end common_constanta
//begin window_message_constanta
#define WM_SHOWED WM_USER + 1
#define WM_EVENT WM_USER + 2
#define WM_MASCHINEN WM_USER + 3
#define WM_ASYNCHRONOUS_TASK WM_APP + 1
//end window_message_constanta
//begin common_controller_type_constanta
#define UNKNOW_TYPE 0
#define UNKNOW_LIST_VIEW 1
//end common_controller_type_constanta
//begin visible_controller_type_constanta
#define WINDOW 1000
#define CONTAINER 1001
#define BUTTON 1002
#define EDIT 1003
#define CHECKBOX 1004
#define COMBOBOX 1005
#define PROGRESS_BAR 1006
#define TABLE 1007
#define LISTVIEW 1008
#define STATUSBAR 1009
//end visible_controller_type_constanta
//begin invisible_controller_type_constanta
#define ASYNC_TASK_TYPE 100
//end invisible_controller_type_constanta
#else
#define MASCHINEN_API __declspec(dllimport)
//begin common_constanta
#define MAX_LOADSTRING 2048
#define FILL_PARENT 65535

#ifdef _UNICODE
#define tstring std::wstring
#define tstrSplit(source, result, delimiter) wstrSplit(source, result, delimiter)
#define to_tstring(PARAM) std::to_wstring(PARAM)
#define tcslen(PARAM) wcslen(PARAM)
#define tcscmp(PARAM1, PARAM2) wcscmp(PARAM1, PARAM2)
#define tstrToLower(PARAM) wstrToLower(PARAM)
#define tstrToUpper(PARAM) wstrToUpper(PARAM)
#else
#define tstring std::string
#define tstrSplit(source, result, delimiter) strSplit(source, result, delimiter)
#define to_tstring(PARAM) std::to_string(PARAM)
#define tcslen(PARAM) strlen(PARAM)
#define tcscmp(PARAM1, PARAM2) strcmp(PARAM1, PARAM2)
#define tstrToLower(PARAM) strToLower(PARAM)
#define tstrToUpper(PARAM) strToUpper(PARAM)
#endif // _UNICODE

//end common_constanta
//begin window_message_constanta
#define WM_SHOWED WM_USER + 1
#define WM_EVENT WM_USER + 2
#define WM_MASCHINEN WM_USER + 3
#define WM_ASYNCHRONOUS_TASK WM_APP + 1
//end window_message_constanta
//begin common_controller_type_constanta
#define UNKNOW_TYPE 0
#define UNKNOW_LIST_VIEW 1
//end common_controller_type_constanta
//begin visible_controller_type_constanta
#define WINDOW 1000
#define CONTAINER 1001
#define BUTTON 1002
#define EDIT 1003
#define CHECKBOX 1004
#define COMBOBOX 1005
#define PROGRESS_BAR 1006
#define TABLE 1007
#define LISTVIEW 1008
#define STATUSBAR 1009
//end visible_controller_type_constanta
//begin invisible_controller_type_constanta
#define ASYNC_TASK_TYPE 100
//end invisible_controller_type_constanta
#endif

#pragma warning(disable:4251)
#pragma warning(disable:4275)
#pragma warning(disable:4090)
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <stdio.h>
#include <stdlib.h>
#include <wincrypt.h>
#include <string>
#include <vector>
#include <memory>
#include <regex>

typedef unsigned long long QWORD;
typedef QWORD* PQWORD;

#define MAKEQWORD(LOWLEFT, HILEFT, LOWRIGHT, HIRIGHT) ((QWORD)LOWLEFT<<48) | ((QWORD)HILEFT<<32) | ((QWORD)LOWRIGHT<<16) | HIRIGHT
#define LOWORDLEFT(VAL) ((WORD)(VAL>>48) & 0xffff)
#define HIWORDLEFT(VAL) ((WORD)(VAL>>32) & 0xffff)
#define LOWORDRIGHT(VAL) ((WORD)(VAL>>16) & 0xffff)
#define HIWORDRIGHT(VAL) (((WORD)VAL) & 0xffff)

// This class is exported from the Maschinen.dll
namespace maschinen
{
	class MASCHINEN_API Maschinen {
	private:
		static bool process_started;
		HACCEL accelerator;
		MSG message;
		UINT filter_min, filter_max;
	protected:
		virtual void onProcess();
	public:
		Maschinen(void);
		// TODO: add your methods here.
		int getResultStatus();
		void beginProcess();
		void setFilterMin();
		void setFilterMax();
		void setAccelarator(HACCEL haccel);
	};
}

extern MASCHINEN_API int nMaschinen;
extern HINSTANCE instance;
extern QWORD window_class_name_counter;
extern tstring window_class_name_seed;

struct MASCHINEN_API Position {
	LONG x;
	LONG y;
	LONG width;
	LONG height;
}; // must be zero before use, use ZeroMemory(pointer, sizeof(Position));

MASCHINEN_API int fnMaschinen(void);
MASCHINEN_API void setInstance( HINSTANCE p_instance );
MASCHINEN_API HINSTANCE getInstance();
MASCHINEN_API void setWindowClassNameSeed( LPCTSTR window_class_name );
MASCHINEN_API tstring& getWindowClassNameSeed();

MASCHINEN_API std::unique_ptr<tstring> translateErrorCode(DWORD error_code);
MASCHINEN_API bool getLastErrorAsString(LPTSTR buffer, size_t size);
MASCHINEN_API bool getLastErrorAsString(LPTSTR buffer, size_t size, DWORD& error_code);
MASCHINEN_API bool sha512String(LPTSTR buffer, size_t size);
MASCHINEN_API size_t strSplit( LPCSTR source, std::vector<std::string>* result, LPCSTR delimiter );
MASCHINEN_API size_t strSplit(const std::string& source, std::vector<std::string>* result, LPCSTR delimiter );
MASCHINEN_API size_t wstrSplit( LPCTSTR source, std::vector<std::wstring>* result, LPCTSTR delimiter );
MASCHINEN_API size_t wstrSplit( const std::wstring& source, std::vector<std::wstring>* result, LPCTSTR delimiter );

MASCHINEN_API std::unique_ptr<std::wstring> strToWstr( LPCSTR input );
MASCHINEN_API std::unique_ptr<std::wstring> strToWstr( std::string& input );
MASCHINEN_API std::unique_ptr<std::wstring> strToWstr( std::string* input );
MASCHINEN_API std::unique_ptr<std::wstring> strToWstr( const std::string& input );
MASCHINEN_API std::unique_ptr<std::wstring> strToWstr( const std::string* const input );

MASCHINEN_API std::unique_ptr<std::string> wstrToStr(LPCWSTR input);
MASCHINEN_API std::unique_ptr<std::string> wstrToStr( std::wstring& input );
MASCHINEN_API std::unique_ptr<std::string> wstrToStr( std::wstring* input );
MASCHINEN_API std::unique_ptr<std::string> wstrToStr( const std::wstring& input );
MASCHINEN_API std::unique_ptr<std::string> wstrToStr( const std::wstring* const input );

MASCHINEN_API std::unique_ptr<std::string> strToLower(LPCSTR input);
MASCHINEN_API std::unique_ptr<std::wstring> wstrToLower(LPCWSTR input);
MASCHINEN_API std::unique_ptr<std::string> strToUpper( LPCSTR input );
MASCHINEN_API std::unique_ptr<std::wstring> wstrToUpper( LPCWSTR input );

MASCHINEN_API void copySystemTime(LPSYSTEMTIME source, LPSYSTEMTIME target) noexcept;
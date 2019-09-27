// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SERAPHCOMMON_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SERAPHCOMMON_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SERAPHCOMMON_EXPORTS
#define SERAPHCOMMON_API __declspec(dllexport)
#define DEFAULT_JUMLAH_CELL_PER_PANJANG 150
#define DEFAULT_JUMLAH_CELL_PER_LEBAR 150
#define VAR_JSON_STATUS "nmlf-json-42323"
#define VAR_JSON_RESULT "nmlf-json-24352"
#define VAR_JSON_MESSAGE "nmlf-json-23432"
#else
#define SERAPHCOMMON_API __declspec(dllimport)
#define DEFAULT_JUMLAH_CELL_PER_PANJANG 150
#define DEFAULT_JUMLAH_CELL_PER_LEBAR 150
#define VAR_JSON_STATUS "nmlf-json-42323"
#define VAR_JSON_RESULT "nmlf-json-24352"
#define VAR_JSON_MESSAGE "nmlf-json-23432"
#endif

#include "Maschinen.h"
#include <KnownFolders.h>

namespace string {
	namespace file {
		namespace name {
			extern SERAPHCOMMON_API const tstring SETTING;
			extern SERAPHCOMMON_API const tstring REQUEST_LOG_NAME;
			extern SERAPHCOMMON_API const tstring ERROR_LOG_NAME;
			extern SERAPHCOMMON_API const tstring DATA_DEFAULT;
			extern SERAPHCOMMON_API const tstring FILE_TYPE_MAPPING;
		}
	}
	namespace directory {
		namespace name {
			extern SERAPHCOMMON_API const tstring FILE_META_DATA;
		}
	}
	namespace process {
		extern SERAPHCOMMON_API const tstring LOCAL_TO_LOCAL;
		extern SERAPHCOMMON_API const tstring LOCAL_TO_CLOUD;
		extern SERAPHCOMMON_API const tstring CLOUD_TO_LOCAL;
		extern SERAPHCOMMON_API const tstring CLOUD_TO_CLOUD;
	}
}

namespace number {
	namespace integer {
		namespace operation {
			extern SERAPHCOMMON_API const int LOCAL_TO_LOCAL;
			extern SERAPHCOMMON_API const int LOCAL_TO_CLOUD;
			extern SERAPHCOMMON_API const int CLOUD_TO_LOCAL;
			extern SERAPHCOMMON_API const int CLOUD_TO_CLOUD;
		}
	}
}

namespace dword {
	namespace error_code {
		//const DWORD BUFFER_SIZE_NOT_FOR_PATH = 333;
		extern SERAPHCOMMON_API const DWORD APP_DATA_DOES_NOT_HAVE_PATH;
		extern SERAPHCOMMON_API const DWORD APP_DATA_NOT_DIRECTORY;
		extern SERAPHCOMMON_API const DWORD APP_DATA_DOES_NOT_HAVE_PATH_ON_SYSTEM;
		namespace meta_data {
			extern SERAPHCOMMON_API const DWORD META_DATA_DOES_NOT_MATCH;
		}
	}
	namespace filetype {
		extern SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_UNKNOWN;
		extern SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_DRIVE;
		extern SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_DRIVE_UNKNOWN;
		extern SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_DRIVE_FIXED;
		extern SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_DRIVE_REMOVABLE;
		extern SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_DRIVE_CDROM;
		extern SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_LOCAL_ROOT;
		extern SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_LOCAL;
		extern SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_LOCAL_DIRECTORY;
		extern SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_LOCAL_FILE;
		extern SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_CLOUD;
		extern SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_CLOUD_DIRECTORY;
		extern SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_CLOUD_FILE;
	}
}

namespace guid {
	namespace folder_id {
		extern SERAPHCOMMON_API const GUID DATA_DEFAULT_PATH;
	}
}


namespace internet {
	namespace variable {
		extern SERAPHCOMMON_API const tstring VAR_PHPSESSID_SEND;
		extern SERAPHCOMMON_API const tstring VAR_REQUEST_TYPE;
		extern SERAPHCOMMON_API const tstring VAR_APP_TYPE;
		extern SERAPHCOMMON_API const tstring VAR_REQUEST;
		extern SERAPHCOMMON_API const tstring VAR_APP_KEY;
		extern SERAPHCOMMON_API const tstring VAR_GET_FILE_TIME_REQUEST;
		extern SERAPHCOMMON_API const tstring FILE_DOWNLOAD_START;
		extern SERAPHCOMMON_API const tstring FILE_DOWNLOAD_END;
		extern SERAPHCOMMON_API const tstring FILE_UPLOAD_ID;
		extern SERAPHCOMMON_API const std::string VAR_PHPSESSID;
	}
	namespace value {
		extern SERAPHCOMMON_API const tstring REQUEST_TYPE;
		extern SERAPHCOMMON_API const tstring APP_TYPE;

		namespace request {
			namespace upload {
				extern SERAPHCOMMON_API const tstring BEGIN;
				extern SERAPHCOMMON_API const tstring EXECUTE;
				extern SERAPHCOMMON_API const tstring END;
				extern SERAPHCOMMON_API const tstring GET_SIZE_PER_FILE_ID;
				extern SERAPHCOMMON_API const tstring GET_TOTAL_SIZE;
			}
			namespace download {
				extern SERAPHCOMMON_API const tstring BEGIN;
			}
			extern SERAPHCOMMON_API const tstring APP_KEY;
			extern SERAPHCOMMON_API const tstring CONFIRM_APP_KEY;
			extern SERAPHCOMMON_API const tstring LOGIN;
			extern SERAPHCOMMON_API const tstring LOGOUT;
			extern SERAPHCOMMON_API const tstring FORGET_PASSWORD;
			extern SERAPHCOMMON_API const tstring GET_APP_KEY;
			extern SERAPHCOMMON_API const tstring REGISTRATION;
			extern SERAPHCOMMON_API const tstring REGISTRATION_GET_ID;
			extern SERAPHCOMMON_API const tstring GET_FILE;
			extern SERAPHCOMMON_API const tstring DOWNLOAD_FILE_SEEN_BY_ALL;
			extern SERAPHCOMMON_API const tstring STOP_FILE_REFRESH;
			extern SERAPHCOMMON_API const tstring DELETE_FILES;
			extern SERAPHCOMMON_API const tstring MOVE_FILE;
			extern SERAPHCOMMON_API const tstring CREATE_NEW_FOLDER;
			extern SERAPHCOMMON_API const tstring COPY_FILE_CLOUD;
			extern SERAPHCOMMON_API const tstring RENAME_FILE;
			extern SERAPHCOMMON_API const tstring RESTORE_FILES;
			extern SERAPHCOMMON_API const tstring MARK_FILES_AS_PRIVILEGE_BY_ALL;
			extern SERAPHCOMMON_API const tstring ADD_USER_LIST_ON_FILE_PRIVILEGE;
			extern SERAPHCOMMON_API const tstring CONFIRM_REGISTRATION;
			extern SERAPHCOMMON_API const tstring DELETE_DELETED_FILE;

			namespace get_file {
				extern SERAPHCOMMON_API const tstring ID;
				extern SERAPHCOMMON_API const tstring TYPE;
				extern SERAPHCOMMON_API const tstring ICON;
			}

			namespace upload_file_begin {
				extern SERAPHCOMMON_API const tstring VAR_BENUTZER;
				extern SERAPHCOMMON_API const tstring VAR_NAME;
				extern SERAPHCOMMON_API const tstring VAR_SIZE;
				extern SERAPHCOMMON_API const tstring VAR_PARENT_ID;
			}
			namespace upload_file_finish {
				extern SERAPHCOMMON_API const tstring VAR_ID;
			}
			namespace create_new_folder {
				extern SERAPHCOMMON_API const tstring PARENT;
				extern SERAPHCOMMON_API const tstring BENUTZER;
			}
		}
	}
}
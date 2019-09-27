// SeraphCommon.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SeraphCommon.h"


namespace string {
	namespace file {
		namespace name {
			SERAPHCOMMON_API const tstring SETTING = TEXT( "seraph.cfg" );
			SERAPHCOMMON_API const tstring REQUEST_LOG_NAME = TEXT( "request.log" );
			SERAPHCOMMON_API const tstring ERROR_LOG_NAME = TEXT( "error.log" );
			SERAPHCOMMON_API const tstring DATA_DEFAULT = TEXT( "SERAPH" );
			SERAPHCOMMON_API const tstring FILE_TYPE_MAPPING = TEXT( "FileTypeMapping.ftm" );
		}

	}
	namespace directory {
		namespace name {
			SERAPHCOMMON_API const tstring FILE_META_DATA = TEXT( "MetaData" );
		}
	}
	namespace process {
		SERAPHCOMMON_API const tstring LOCAL_TO_LOCAL = TEXT( "Local to Local" );
		SERAPHCOMMON_API const tstring LOCAL_TO_CLOUD = TEXT( "Local to Cloud" );
		SERAPHCOMMON_API const tstring CLOUD_TO_LOCAL = TEXT( "Cloud to Local" );
		SERAPHCOMMON_API const tstring CLOUD_TO_CLOUD = TEXT( "Cloud to Cloud" );
	}
}

namespace number {
	namespace integer {
		namespace operation {
			SERAPHCOMMON_API const int LOCAL_TO_LOCAL = 1;
			SERAPHCOMMON_API const int LOCAL_TO_CLOUD = 2;
			SERAPHCOMMON_API const int CLOUD_TO_LOCAL = 3;
			SERAPHCOMMON_API const int CLOUD_TO_CLOUD = 4;
		}
	}
}

namespace dword {
	namespace error_code {
		//const DWORD BUFFER_SIZE_NOT_FOR_PATH = 333;
		SERAPHCOMMON_API const DWORD APP_DATA_DOES_NOT_HAVE_PATH = 665;
		SERAPHCOMMON_API const DWORD APP_DATA_NOT_DIRECTORY = 666;
		SERAPHCOMMON_API const DWORD APP_DATA_DOES_NOT_HAVE_PATH_ON_SYSTEM = 667;
		namespace meta_data {
			SERAPHCOMMON_API const DWORD META_DATA_DOES_NOT_MATCH = 554;
		}
	}
	namespace filetype {
		SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_UNKNOWN = 0;
		SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_DRIVE = 1;
		SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_DRIVE_UNKNOWN = 2;
		SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_DRIVE_FIXED = 3;
		SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_DRIVE_REMOVABLE = 4;
		SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_DRIVE_CDROM = 5;
		SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_LOCAL_ROOT = 6;
		SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_LOCAL = 7;
		SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_LOCAL_DIRECTORY = 8;
		SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_LOCAL_FILE = 9;
		SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_CLOUD = 10;
		SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_CLOUD_DIRECTORY = 11;
		SERAPHCOMMON_API const DWORD FILE_BASE_TYPE_CLOUD_FILE = 12;
	}
}

namespace guid {
	namespace folder_id {
		SERAPHCOMMON_API const GUID DATA_DEFAULT_PATH = FOLDERID_LocalAppData;
	}
}


namespace internet {
	namespace variable {
		SERAPHCOMMON_API const tstring VAR_PHPSESSID_SEND = TEXT( "PHPSESSID" );
		SERAPHCOMMON_API const tstring VAR_REQUEST_TYPE = TEXT( "nmlf-req-9583" );
		SERAPHCOMMON_API const tstring VAR_APP_TYPE = TEXT( "erhalten-daten-1558" );
		SERAPHCOMMON_API const tstring VAR_REQUEST = TEXT( "nmlf-req-9959" );
		SERAPHCOMMON_API const tstring VAR_APP_KEY = TEXT( "erhlanten-daten-9493" );
		SERAPHCOMMON_API const tstring VAR_GET_FILE_TIME_REQUEST = TEXT( "cf-gf-897531590" );
		SERAPHCOMMON_API const tstring FILE_DOWNLOAD_START = TEXT( "cf-df-22948938402" );
		SERAPHCOMMON_API const tstring FILE_DOWNLOAD_END = TEXT( "cf-df-235984035847" );
		SERAPHCOMMON_API const tstring FILE_UPLOAD_ID = TEXT( "cf-uf-3445693958694505465789487888796546484" );
		SERAPHCOMMON_API const std::string VAR_PHPSESSID = "erhalten-daten-7748";
	}
	namespace value {
		SERAPHCOMMON_API const tstring REQUEST_TYPE = TEXT( "nmlf-req-8341" );
		SERAPHCOMMON_API const tstring APP_TYPE = TEXT( "erhalten-daten-8858" );

		namespace request {
			namespace upload {
				SERAPHCOMMON_API const tstring BEGIN = TEXT( "cf-req-23343545jdjr43434828323" );
				SERAPHCOMMON_API const tstring EXECUTE = TEXT( "cf-req-2324342323354455667" );
				SERAPHCOMMON_API const tstring END = TEXT( "cf-req-993845783947642379854375895782443456768343" );
				SERAPHCOMMON_API const tstring GET_SIZE_PER_FILE_ID = TEXT( "nmlf-req-37u9834579348573459-2894-09423-8543580357398573" );
				SERAPHCOMMON_API const tstring GET_TOTAL_SIZE = TEXT( "nmlf-req-304930-5904605860596849865546" );
			}
			namespace download {
				SERAPHCOMMON_API const tstring BEGIN = TEXT( "cf-req-85884251458858594858" );
			}
			SERAPHCOMMON_API const tstring APP_KEY = TEXT( "nmlf-req-1020" );
			SERAPHCOMMON_API const tstring CONFIRM_APP_KEY = TEXT( "cf-req-4030493054958" );
			SERAPHCOMMON_API const tstring LOGIN = TEXT( "nmlf-req-8942" );
			SERAPHCOMMON_API const tstring LOGOUT = TEXT( "nmlf-req-8583" );
			SERAPHCOMMON_API const tstring FORGET_PASSWORD = TEXT( "nmlf-req-4783" );
			SERAPHCOMMON_API const tstring GET_APP_KEY = TEXT( "nmlf-req-1020" );
			SERAPHCOMMON_API const tstring REGISTRATION = TEXT( "cf-req-8954858" );
			SERAPHCOMMON_API const tstring REGISTRATION_GET_ID = TEXT( "cf-req-4814684646" );
			SERAPHCOMMON_API const tstring GET_FILE = TEXT( "cf-req-858842514588585555" );
			SERAPHCOMMON_API const tstring DOWNLOAD_FILE_SEEN_BY_ALL = TEXT( "cf-req-8588425305949859" );
			SERAPHCOMMON_API const tstring STOP_FILE_REFRESH = TEXT( "cf-req-s" );
			SERAPHCOMMON_API const tstring DELETE_FILES = TEXT( "cf-req-29483847284888823824334" );
			SERAPHCOMMON_API const tstring MOVE_FILE = TEXT( "cf-req-47597928475049598409274" );
			SERAPHCOMMON_API const tstring CREATE_NEW_FOLDER = TEXT( "cf-req-9458490680975887589375893750284204820948205748975" );
			SERAPHCOMMON_API const tstring COPY_FILE_CLOUD = TEXT( "cf-req-0343939458345098934593405389578353945834" );
			SERAPHCOMMON_API const tstring RENAME_FILE = TEXT( "cf-req-04903849038092490234892034892304723842" );
			SERAPHCOMMON_API const tstring RESTORE_FILES = TEXT( "cf-req-827319371289378129371289361736128361831" );
			SERAPHCOMMON_API const tstring MARK_FILES_AS_PRIVILEGE_BY_ALL = TEXT( "cf-req-940832048234829842094820948290823" );
			SERAPHCOMMON_API const tstring ADD_USER_LIST_ON_FILE_PRIVILEGE = TEXT( "cf-req-2398483208423094829048234" );
			SERAPHCOMMON_API const tstring CONFIRM_REGISTRATION = TEXT( "cf-req-3483894729749827489274982742984" );
			SERAPHCOMMON_API const tstring DELETE_DELETED_FILE = TEXT( "cf-req-4342342384294723497289478234" );

			namespace get_file {
				SERAPHCOMMON_API const tstring ID = TEXT( "cf-gaftsaf-2367" );
				SERAPHCOMMON_API const tstring TYPE = L"cf-gaftsaf-666";
				SERAPHCOMMON_API const tstring ICON = L"cf-gaftsaf-897";
			}

			namespace upload_file_begin {
				SERAPHCOMMON_API const tstring VAR_BENUTZER = TEXT( "cf-ufb-a943875847589475938743984" );
				SERAPHCOMMON_API const tstring VAR_NAME = TEXT( "cf-ufb-b843857878374748347947239847664343446565" );
				SERAPHCOMMON_API const tstring VAR_SIZE = TEXT( "cf-ufb-uuru328843445748975485" );
				SERAPHCOMMON_API const tstring VAR_PARENT_ID = TEXT( "cf-ufb-834737777347374732323432323342" );
			}
			namespace upload_file_finish {
				SERAPHCOMMON_API const tstring VAR_ID = TEXT( "cf-uff-84593758927427584204830579720948029483957" );
			}
			namespace create_new_folder {
				SERAPHCOMMON_API const tstring PARENT = TEXT( "nmlf-cnf-02439085824902849375824" );
				SERAPHCOMMON_API const tstring BENUTZER = TEXT( "nmlf-cnf-439078974984728947284792" );
			}
		}
	}
}
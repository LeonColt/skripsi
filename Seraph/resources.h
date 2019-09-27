#pragma once
//main window
//key 10xx
#define MAIN_WINDOW_STATUS_BAR 1001


//login
//key 11xx
#define unit_login 1100
#define edit_login_id_username_email 1101
#define edit_login_password 1102
#define button_login_login 1103
#define button_login_forget_password 1104
#define button_login_registration 1105
#define list_view_icon_login_daten 1106
#define button_login_upload_file 1107
#define progress_bar_login_pb 1108
#define MENU_UNIT_LOGIN_LOGOUT 1109

#define request_login L"nmlf-req-8942"
#define var_id_username_email L"nmlf-l-8593"
#define var_password L"nmlf-l-8888"

//regisration
//key : 12xx
#define unit_registration 1200
#define static_registration_id 1201
#define edit_registration_id 1202
#define static_registration_username 1203
#define edit_registration_username 1204
#define static_registration_email 1205
#define edit_registration_email 1206
#define static_registration_password 1207
#define edit_registration_password 1208
#define static_registration_confirm_password 1209
#define edit_registration_confirm_password 1210
#define btt_registration_regisration 1211
#define check_box_registration_aggree 1212
#define static_registration_message 1213
#define btt_registration_login 1214
//registration_confirmation
#define EDIT_CONFIRM_REGISTRATION_SECRET_CODE 1215
#define BTT_CONFIRM_REGISTRATION 1216

//fileControl
//key 13xx
#define unit_file_control 1300
#define list_view_file_control_file_shower 1301
#define button_file_control_back 1302
#define button_file_control_refresh 1303
#define edit_file_control_url 1304
#define edit_file_control_file_rename 1305
#define progress_bar_file_control_pb 1306
#define menu_file_control_open 1307
#define menu_File_control_cut 1308
#define menu_file_control_copy 1309
#define menu_file_control_rename 1310
#define menu_file_control_paste 1311
#define menu_file_control_new_folder 1312
#define menu_file_control_delete 1313
#define MENU_FILE_CONTROL_RESTORE 1314
#define MENU_FILE_CONTROL_PROPERTY 1315

//list download file atau filedownloadlist
//key 14xx
#define FILE_DOWNLOAD_WINDOW 1400
#define LIST_VIEW_FILE_DOWNLOAD_WINDOW 1401
#define MENU_FILE_DOWNLOAD_WINDOW_START_DOWNLOAD 1402
#define MENU_FILE_DOWNLOAD_WINDOW_OPEN_FILE_LOCATION 1404
#define MENU_FILE_DOWNLOAD_WINDOW_OPEN_FILE_LOCATION_IN_EXPLORER 1405
#define MENU_FILE_DOWNLOAD_WINDOW_DELETE_FILE 1406
#define MENU_FILE_DOWNLOAD_WINDOW_DELETE 1407


//file process window
//key 15xx
#define FILE_PROCESS_WINDOW 1500
#define LABEL_FILE_PROCESS_STATUS_VAR 1501
#define LABEL_FILE_PROCESS_STATUS_VAL 1502
#define LABEL_FILE_PROCESS_SIZE_VAR 1503
#define LABEL_FILE_PROCESS_SIZE_VAL 1504
#define LABEL_FILE_PROCESS_DOWNLOADED_VAR 1505
#define LABEL_FILE_PROCESS_DOWNLOADED_VAL 1506
#define LABEL_FILE_PROCESS_SPEED_VAR 1507
#define LABEL_FILE_PROCESS_SPEED_VAL 1508
#define LABEL_FILE_PROCESS_REMAIN_TIME_VAR 1509
#define LABEL_FILE_PROCESS_REMAIN_TIME_VAL 1510
#define LABEL_FILE_PROCESS_BUFFER_SIZE 1511
#define EDIT_FILE_PROCESS_BUFFER_SIZE 1512
#define BUTTON_FILE_PROCESS_START_PAUSE 1514
#define BUTTON_FILE_PROCESS_CANCEL 1515


//fileUploadOperationWindow
//key 16xx
#define FILE_UPLOAD_OPERATION_WINDOW 1600
#define LISTVIEW_FILE_UPLOAD_OPERATION_WINDOW 1601
#define MENU_FILE_UPLOAD_OPERATION_WINDOW_START_UPLOAD 1602
#define MENU_FILE_UPLOAD_OPERATION_WINDOW_OPEN_FILE_LOCATION 1404
#define MENU_FILE_UPLOAD_OPERATION_WINDOW_DELETE_FILE 1406
#define MENU_FILE_UPLOAD_OPERATION_WINDOW_DELETE 1407

//FileUploadProcessWindow
//key 17xx
#define FILE_UPLOAD_PROCESS_WINDOW 1700
#define LABEL_FILE_UPLOAD_PROCESS_STATUS_VAR 1701
#define LABEL_FILE_UPLOAD_PROCESS_STATUS_VAL 1702
#define LABEL_FILE_UPLOAD_PROCESS_SIZE_VAR 1703
#define LABEL_FILE_UPLOAD_PROCESS_SIZE_VAL 1704
#define LABEL_FILE_UPLOAD_PROCESS_DOWNLOADED_VAR 1705
#define LABEL_FILE_UPLOAD_PROCESS_DOWNLOADED_VAL 1706
#define LABEL_FILE_UPLOAD_PROCESS_SPEED_VAR 1707
#define LABEL_FILE_UPLOAD_PROCESS_SPEED_VAL 1708
#define LABEL_FILE_UPLOAD_PROCESS_REMAIN_TIME_VAR 1709
#define LABEL_FILE_UPLOAD_PROCESS_REMAIN_TIME_VAL 1710
#define LABEL_FILE_UPLOAD_PROCESS_BUFFER_SIZE 1711
#define EDIT_FILE_UPLOAD_PROCESS_BUFFER_SIZE 1712
#define BUTTON_FILE_UPLOAD_PROCESS_START_PAUSE 1714
#define BUTTON_FILE_UPLOAD_PROCESS_CANCEL 1715

//Directory Cloud Property
//key 18xx
#define CLOUD_PROPERTY 1800
#define BUTTON_CLOUD_PROPERTY_ABOUT 1801
#define BUTTON_CLOUD_PROPERTY_SEEN_LIST 1802
#define BUTTON_CLOUD_PROPERTY_DOWNLOADER_LIST 1803
#define BUTTON_CLOUD_PROPERTY_UPLOAD_LIST 1804

//DirectoryCloudPropertyAbout
//key 19xx
#define CLOUD_PROPERTY_ABOUT 1900
#define LABEL_CLOUD_PROPERTY_ABOUT_ID_VAR 1901
#define LABEL_CLOUD_PROPERTY_ABOUT_ID_VAL 1902
#define LABEL_CLOUD_PROPERTY_ABOUT_NAME 1903
#define EDIT_CLOUD_PROPERTY_ABOUT_NAME 1904
#define LABEL_CLOUD_PROPERTY_ABOUT_SEEN_BY_ALL 1905
#define CHECK_CLOUD_PROPERTY_ABOUT_SEEN_BY_ALL 1906
#define LABEL_CLOUD_PROPERTY_ABOUT_DOWNLOAD_BY_ALL 1907
#define CHECK_CLOUD_PROPERTY_ABOUT_DOWNLOAD_BY_ALL 1908
#define LABEL_CLOUD_PROPERTY_ABOUT_MODIFY_BY_ALL 1909
#define CHECK_CLOUD_PROPERTY_ABOUT_MODIFY_BY_ALL 1910
#define LABEL_CLOUD_PROPERTY_ABOUT_OWNER_VAR 1911
#define LABEL_CLOUD_PROPERTY_ABOUT_OWNER_VAL 1912
#define LABEL_CLOUD_PROPERTY_ABOUT_PARENT_VAR 1913
#define LABEL_CLOUD_PROPERTY_ABOUT_PARENT_VAL 1914

//CloudPropertyDownloadList
//key 20xx
#define CLOUD_PROPERTY_DOWNLOAD_LIST 2000
#define LIST_VIEW_CLOUD_PROPERTY_DOWNLOAD_LIST 2001

//CloudPropertyUploaderList
//key 21xx
#define CLOUD_PROPERTY_UPLOADER_LIST 2100
#define LABEL_CLOUD_PROPERTY_UPLOADER_LIST_NAME_OR_ID_ADDED 2101
#define EDIT_CLOUD_PROPERTY_UPLOADER_LIST_NAME_OR_ID_ADDEDD 2102
#define BUTTON_CLOUD_PROPERTY_PRIVILEGE_LIST_ADD_USER 2103

//CloudPropertySeenList
//key 22xx
#define CLOUD_PROPERTY_SEEN_LIST 2200

//LocalProperty
//key 23xx
#define LOCAL_PROPERTY 2300
#define BUTTON_LOCAL_PROPERTY_ABOUT 2301

//LocalPropertyAbout
//key 24xx
#define LOCAL_PROPERTY_ABOUT 2400
#define LABEL_LOCAL_PROPERTY_ABOUT_NAME 2401
#define EDIT_LOCAL_PROPERTY_ABOUT_NAME 2402
#define LABEL_LOCAL_PROPERTY_ABOUT_OWNER_VAR 2403
#define LABEL_LOCAL_PROPERTY_ABOUT_OWNER_VAL 2404
#define LABEL_LOCAL_PROPERTY_ABOUT_PATH_VAR 2405
#define LABEL_LOCAL_PROPERTY_ABOUT_PATH_VAL 2406
#define LABEL_LOCAL_PROPERTY_ABOUT_DATE_CREATED_VAR 2407
#define LABEL_LOCAL_PROPERTY_ABOUT_DATE_CREATED_VAL 2408
#define LABEL_LOCAL_PROPERTY_ABOUT_LAST_ACCESS_TIME_VAR 2409
#define LABEL_LOCAL_PROPERTY_ABOUT_LAST_ACCESS_TIME_VAL 2410
#define LABEL_LOCAL_PROPERTY_ABOUT_LAST_WRITE_TIME_VAR 2411
#define LABEL_LOCAL_PROPERTY_ABOUT_LAST_WRITE_TIME_VAL 2412

//CloudRecycleBinProperty
//key 25xx
#define CLOUD_RECYCLE_BIN_PROPERTY 2500
#define BUTTON_CLOUD_RECYCLE_BIN_PROPERTY_ABOUT 2511

//CloudRecycleBinPropertyAbout
//key 26xx
#define CLOUD_RECYCLE_BIN_PROPERTY_ABOUT 2600
#define LABEL_CLOUD_RECYCLE_BIN_PROPERTY_NAME 2511
#define EDIT_CLOUD_RECYCLE_BIN_PROPERTY_NAME 2512
#define LABEL_CLOUD_RECYCLE_BIN_PROPERTY_OWNER_VAR 2513
#define LABEL_CLOUD_RECYCLE_BIN_PROPERTY_OWNER_VAL 2514
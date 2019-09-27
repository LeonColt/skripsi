<?php
/**
 * Created by PhpStorm.
 * User: LC
 * Date: 19/02/2016
 * Time: 15:28
 */
if(!defined('standard_path'))
    define('standart_path', $_SERVER['DOCUMENT_ROOT'].'/seraph/');

if(!defined('standart_url'))
    define('standart_url', $_SERVER['SERVER_NAME'].'/seraph/');

if(!defined('name_of_session'))
    define('name_of_session', ('erhlaten_daten_'.bin2hex(openssl_random_pseudo_bytes(7))));

if(!defined('REGISTRATION_TEMPORARY_PATH'))
    define('REGISTRATION_TEMPORARY_PATH', (standart_path.'daten/registrant'));

const SECURITY_TOKEN_MIN = 25;
const SECURITY_TOKEN_MAX = 100;

const MYSQL_BIGINT_UNSIGNED_MAX = 18446744073709552000;

const EMAIL_ADMIN_NO_REPLY = "no-reply@seraph.xyz";

const UNKNOWN_FILE_TYPE = 65535;
const UNKNOWN_FILE_FORMAT = 16777215;

const host = "mysql.idhostinger.com";
const port = 3306;
const database = "u576593533_daten";
const database_username = "u576593533_cas";
const database_password = "ANNODOMINO33131";

const BUFFER_SIZE = 4096;

const var_app_key = "erhlanten-daten-9493";
const var_app_type = "erhalten-daten-1558";
const app_type_winapi = "erhalten-daten-8858";
const app_type_linuxapi = "erhalten-daten-4234";
const app_type_android = "erhalten-daten-5895";
const app_type_mac_os = "erhalten-daten-6678";
const app_type_browser = "erhalten-daten-9980";
const var_phpsessid = "erhalten-daten-7748";

const loginid = 'nmlf-l-4564';
const userid = 'nmlf-l-8654';
const username = 'nmlf-l-7556';
const useremail = 'nmlf-l-7862';
const var_login_as_admin = "nmlf-l-8598";
const var_id_username_email = "nmlf-l-8593";
const var_password = "nmlf-l-8888";
//register
const var_id = "cf-benutzer-884753545";
const var_username = "cf-benutzer-03493849";
const var_email = "cf-benutzer-24335454";
const var_registration_password = "cf-benutzer-2394394392232";

//request
//nmlf_req_xxxx
const var_request = "nmlf-req-9959";
const var_request_type = "nmlf-req-9583";
const var_request_list = "nmlf-req-8574";
const request_forget_password = "nmlf-req-4783";
const request_type_json = "nmlf-req-8341";
const request_type_xml = "nmlf-req-8876";
const request_login = "nmlf-req-8942";
const request_logout = "nmlf-req-8583";
const request_app_key = "nmlf-req-1020";
const request_registration = "cf-req-8954858";
const request_registration_get_id = "cf-req-4814684646";
const request_confirm_app_key = "cf-req-4030493054958";
const request_get_file = "cf-req-858842514588585555";
const request_download_file_begin = "cf-req-85884251458858594858";
const request_download_file = "cf-req-8588425305949859";
const request_download_file_finish = "cf-req-858842519949598876895";
const request_delete_file = "cf-req-29483847284888823824334";
const request_update_file = "cf-req-09929392499989898232323232434554";
const request_insert_file = "cf-req-599658741123586989585585558592150255541";
const request_stop_file_refresh = "cf-req-s";
const request_copy_file = "cf-req-3457487937593879287424";
const request_move_file = "cf-req-47597928475049598409274";
const request_rename_file = "cf-req-4846130360449+84848";
//const request_delete_file = "cf-req-47841300201056163100478745415";


//request_get_all_file_seen_by_all mapper
const get_file_id = "cf-gafsaf-8789955585897959";
const get_file_nama = "cf-gafsaf-7864051513135464313";
const get_file_format_id = "cf-gafsaf-459958398393056";
const get_file_format = "cf-gafsaf-5930593725647003746";
const get_file_tipe_id = "cf-gafsaf-88989850876930";
const get_file_tipe = "cf-gafsaf-589756249587958";
const get_file_size = "cf-gafsaf-8485964928658996349252";
const get_file_owner = "cf-gafsaf-8589649621635825420032";
const get_file_exists = "cf-gafsaf-02850205954057947";
const get_file_parent = "cf-gafsaf-02850205349037403";


//session
//nmlf-ssrq-xxxxxxxx
const session_time = "nmlf-ssrq-00000001";
const session_login = "nmlf-ssrq-84594852";
const session_request_list = "nmlf-ssrq-89848593";
const session_time_login = "nmlf-ssrq-857966321";
const session_file_download = "nmlf-ssrq-2993384938";
const session_app_key = "nmlf-ssrq-9402384749378";
const session_download_key_list = "nmlf-ssrq-894185285979787";
const session_get_file_time = "nmlf-ssrq-9943482933482933333345";
const session_stop_file_refresh = "nmlf-ssrq-s";


const var_json_status = "nmlf-json-42323";
const var_json_result = "nmlf-json-24352";
const var_json_message = "nmlf-json-23432";
const json_status_success = "nmlf-json-23232";
const json_status_failed = "nmlf-json-23333";

//download
const id_download_length = 25;
const id_upload_length = 25;
const file_download_file_id = "cf-df-9950435903033959";

//file_changing_universal
const file_changing_time = "cf-fcu-897463215898";
const file_changing_file_id = "cf-fcu-9879898585858";
const file_changing_file_operation = "cf-fcu-99696585772589547";
const file_changing_file_insert = "cf-fcu-9314878756321";
const file_changing_file_update = "cf-fcu-998878785210";
const file_changing_file_delete = "cf-fcu-6312056970365891325";

//xml
if(!defined("xml_session")) define("xml_session", standart_path."core/XML/session.xml");
if(!defined("xml_common")) define("xml_common", standart_path."core/XML/common.xml");
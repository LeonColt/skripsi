<?php

/**
 * Created by PhpStorm.
 * User: LeonColt
 * Date: 11/4/2016
 * Time: 11:49 AM
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
class JSONRequest
{
    const var_request = "nmlf-req-9959";
    //begin akey
    const akey = "nmlf-req-1020";
    const confirm_akey = "cf-req-4030493054958";
    //end akey

    //begin download
    const download_file_begin = "cf-req-85884251458858594858";
    const download_file = "cf-req-8588425305949859";
    const download_file_finish = "cf-req-858842519949598876895";
    //end download

    //begin upload
    const upload_file_begin = "cf-req-23343545jdjr43434828323";
    const upload_file = "cf-req-2324342323354455667";
    const upload_file_finish = "cf-req-993845783947642379854375895782443456768343";
    const upload_file_upload_file_size = "nmlf-req-37u9834579348573459-2894-09423-8543580357398573";
    const upload_file_upload_total_file_size = "nmlf-req-304930-5904605860596849865546";
    //end upload

    const var_request_type = "nmlf-req-9583";
    const var_request_list = "nmlf-req-8574";
    const FORGET_PASSWORD = "nmlf-req-4783";
    const request_type_json = "nmlf-req-8341";
    const request_type_xml = "nmlf-req-8876";
    const request_login = "nmlf-req-8942";
    const request_logout = "nmlf-req-8583";
    const request_registration = "cf-req-8954858";
    const request_registration_get_id = "cf-req-4814684646";
    const get_file = "cf-req-858842514588585555";
    const GET_FILE_INFO = "cf-req-858842514588585555-ex";
    const PATH_PARSER = "cf-req-3472847249827498274927498274298";
    const GET_RECYCLE_BIN_LIST = "cf-req-34994058305435035395";
    const request_delete_file = "cf-req-29483847284888823824334";
    const request_update_file = "cf-req-09929392499989898232323232434554";
    const request_insert_file = "cf-req-599658741123586989585585558592150255541";
    const request_stop_file_refresh = "cf-req-s";
    const request_copy_file = "cf-req-3457487937593879287424";
    const move_file = "cf-req-47597928475049598409274";
    const COPY_FILE = "cf-req-0343939458345098934593405389578353945834";
    const request_rename_file = "cf-req-4846130360449+84848";
    const CREATE_NEW_FOLDER = "cf-req-9458490680975887589375893750284204820948205748975";
    const SEARCH_FILE = "cf-req-980948320948039849302809238034980923842378";
    const RENAME_FILE = "cf-req-04903849038092490234892034892304723842";
    const RESTORE_FILE = "cf-req-827319371289378129371289361736128361831";
    //const request_delete_file = "cf-req-47841300201056163100478745415";
    const GRANT_DOWNLOAD_ACCESS = "cf-req-340930593495304583490850398593084";
    const GET_DOWNLOAD_ACCESS_ON_FILE = "cf-req-343905043054345384590835340539405835";
    const MARK_FILE_AS_PRIVILEGE_BY_ALL = "cf-req-940832048234829842094820948290823";
    const GET_USER_LIST = "cf-req-34939849574389573497345";
    const GET_USER_LIST_ON_FILE_PRIVILEGE = "cf-req-3490389547348975395834";
    const ADD_USER_LIST_ON_FILE_PRIVILEGE = "cf-req-2398483208423094829048234";
    const CONFIRM_REGISTRATION = "cf-req-3483894729749827489274982742984";
    const DELETE_DELETED_FILE = "cf-req-4342342384294723497289478234";
    const PARSER_PATHS = "cf-req-483924892482472897428947298";
    const DIRECTORY_CHILDREN_COUNT = "cf-req-3874827428742472874284782478247284";
}
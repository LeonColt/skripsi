<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 29/02/2016
 * Time: 17:02
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
require_once standart_path.'core/Session/Session.php';
require_once standart_path.'core/Session/AkeySession.php';
require_once standart_path.'core/Session/Session_login.php';
require_once standart_path.'core/UniversalMethode.php';
require_once standart_path.'core/file/FileHandler.php';
require_once standart_path.'core/database/Runner.php';
require_once standart_path.'server_script/JSON/json_encoder.php';
require_once standart_path.'server_script/JSON/JSONRequest.php';
require_once standart_path.'core/User.php';
class JSON_Runner
{
    private $app_type, $akey;
    public function __construct(string $app_type)
    {
        $this->app_type = $app_type;
        $this->akey = null;
    }
    public function execute($request)
    {
        if(strcmp($request, JSONRequest::akey) === 0) $this->getAppKey();
        else if(strcmp($request, JSONRequest::confirm_akey) === 0) $this->confirmAppKey();
        else if($this->isAkeyValid()) {
            switch($request)
            {
                case request_registration_get_id: $this->registration_get_benutzer_id(); break;
                case request_login: $this->request_login(); break;
                case request_logout: $this->request_logout(); break;
                case request_registration: $this->request_registration(); break;
                case JSONRequest::get_file: $this->getFile(); break;
                case JSONRequest::GET_RECYCLE_BIN_LIST: $this->getRecycleBin(); break;
                case JSONRequest::request_delete_file: $this->requestDeleteFile(); break;
                case request_delete_file: $this->requestDeleteFile(); break;
                case JSONRequest::COPY_FILE: $this->requestCopyFile(); break;
                case JSONRequest::move_file: $this->requestMoveFile(); break;
                case JSONRequest::CREATE_NEW_FOLDER: $this->requestCreateNewFolder(); break;
                case JSONRequest::SEARCH_FILE: $this->searchFile(); break;
                case JSONRequest::RENAME_FILE : $this->requestRenameFile(); break;
                case JSONRequest::RESTORE_FILE : $this->requestRestoreFile(); break;

                //begin registration
                case JSONRequest::CONFIRM_REGISTRATION : $this->requestConfirmRegistration(); break;
                //end registration

                case JSONRequest::FORGET_PASSWORD : $this->requestForgetPassword(); break;

                case JSONRequest::PATH_PARSER : $this->requestPathParser(); break;

                //begin file upload
                case JSONRequest::upload_file_begin: $this->requestUploadFileBegin(); break;
                case JSONRequest::upload_file: $this->requestUploadFile(); break;
                case JSONRequest::upload_file_finish: $this->requestUploadFileFinish(); break;
                case JSONRequest::upload_file_upload_file_size: $this->requestUploadFileUploadFileSize(); break;
                case JSONRequest::upload_file_upload_total_file_size: $this->requestUploadFileUploadTotalFileSize(); break;
                //end file upload

                //begin file download
                case JSONRequest::download_file_begin: $this->downloadFileBegin(); break;
                case JSONRequest::download_file: $this->requestDownloadFile(); break;
                //end file download

                //begin file access
                case JSONRequest::GET_DOWNLOAD_ACCESS_ON_FILE : $this->requestGetDownloadAccesOnFile(); break;
                //end file access

                case JSONRequest::MARK_FILE_AS_PRIVILEGE_BY_ALL : $this->requestMarkFileAsPrivilegeByAll(); break;

                case JSONRequest::GET_USER_LIST: $this->requestGetUserList(); break;

                case JSONRequest::GET_USER_LIST_ON_FILE_PRIVILEGE: $this->requestGetUserListOnFilePrivilege(); break;

                case JSONRequest::ADD_USER_LIST_ON_FILE_PRIVILEGE : $this->requestAddUserListOnFilePrivilege(); break;

                case JSONRequest::DELETE_DELETED_FILE : $this->requestDeleteDeletedFile(); break;

                case JSONRequest::PARSER_PATHS : $this->requestParserPaths(); break;

                case JSONRequest::DIRECTORY_CHILDREN_COUNT: $this->requestDirectoryChildrenCount(); break;

                default:
                {
                    header("HTTP/1.0 404 Not Found - command not found", true, 404);
                    require standart_path."404.html";
                    exit();
                } break;
            }
        }
        else
        {
            header("HTTP/1.0 401 Illegal Request - command denied", true, 401);
            echo "missing Application Key";
            exit();
        }
    }

    private function requestDirectoryChildrenCount() {
        require_once standart_path.'server_script/file_controller/DirectoryChildrenCount.php';
        $encoder = new json_encoder();
        try {
            $exec = new DirectoryChildrenCount(filter_input(INPUT_POST, DirectoryChildrenCount::PATH),
                filter_input(INPUT_POST, DirectoryChildrenCount::LOGIN_ID));
            $count = $exec->execute();
            $encoder->status_berhasil();
            $encoder->set_result($count);
        } catch( Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function searchFile() {
        require_once standart_path.'server_script/file_controller/SearchFile.php';
        $encoder = new json_encoder();
        try {
            $exec = new SearchFile(filter_input(INPUT_POST, SearchFile::FILE_NAME), filter_input(INPUT_POST, SearchFile::LOGIN_ID));
            $result = $exec->execute();
            $encoder->status_berhasil();
            $encoder->set_result($result);
        } catch (Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function requestRenameFile() {
        require_once standart_path.'server_script/file_controller/FileRename.php';
        $encoder = new json_encoder();
        try {
            $exec = new FileRename(filter_input(INPUT_POST, FileRename::ID, FILTER_VALIDATE_INT), filter_input(INPUT_POST, FileRename::NEW_NAME));
            $exec->execute();
            $encoder->status_berhasil();
        } catch(Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function requestConfirmRegistration() {
        require_once standart_path.'server_script/Register/Confirmation.php';
        $encoder = new json_encoder();
        try {
            $exec = new Confirmation(filter_input(INPUT_POST, Confirmation::SECURITY_TOKEN), filter_input(INPUT_POST, Confirmation::SECRET_KEY));
            $exec->execute();
            $encoder->status_berhasil();
            $encoder->set_message("Registration Success, Now you can login");
        } catch(Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function requestPathParser() {
        require_once standart_path.'server_script/file_controller/PathParser.php';
        $encoder = new json_encoder();
        try {
            $exec = new PathParser(filter_input(INPUT_POST, PathParser::INPUT_PATH),
                filter_input(INPUT_POST, PathParser::LOGIN_ID));
            $result = $exec->execute();
            $encoder->status_berhasil();
            $encoder->set_result($result);
        } catch(Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function requestForgetPassword() {
        require_once standart_path.'server_script/ForgetPassword.php';
        $encoder = new json_encoder();
        try {
            $exec = new ForgetPassword(filter_input(INPUT_POST, ForgetPassword::ID_USERNAME_EMAIL));
            $exec->execute();
            $encoder->status_berhasil();
            $encoder->set_message("Password sudah dikirim ke email");
        } catch(Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function requestRestoreFile() {
        require_once standart_path.'server_script/file_controller/RestoreFile.php';
        $encoder = new json_encoder();
        try {
            $exec = new RestoreFile(filter_input(INPUT_POST, RestoreFile::IDS), filter_input(INPUT_POST, RestoreFile::LOGIN_ID));
            $exec->execute();
            $encoder->status_berhasil();
        } catch(Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function requestCreateNewFolder() {
        require_once standart_path.'server_script/file_controller/CreateNewFolder.php';
        $encoder = new json_encoder();
        try {
            $exec = new CreateNewFolder(filter_input(INPUT_POST, CreateNewFolder::PARENT),
                filter_input(INPUT_POST, CreateNewFolder::NAME),
                filter_input(INPUT_POST, CreateNewFolder::LOGIN_ID));
            $exec->execute();
            $encoder->status_berhasil();
        } catch(Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function requestCopyFile() {
        require_once standart_path.'server_script/file_controller/FileCopy.php';
        $encoder = new json_encoder();
        try {
            $exec = new FileCopy(filter_input(INPUT_POST, FileCopy::SOURCES),
                filter_input(INPUT_POST, FileCopy::TARGET),
                filter_input(INPUT_POST, FileCopy::LOGIN_ID));
            $exec->execute();
            $encoder->status_berhasil();
        } catch(Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function requestMoveFile() {
        require_once standart_path.'server_script/file_controller/FileMove.php';
        $encoder = new json_encoder();
        try {
            $exec = new FileMove(filter_input(INPUT_POST, FileMove::FROMS),
                filter_input(INPUT_POST, FileMove::TO),
                filter_input(INPUT_POST, FileMove::LOGIN_ID));
            $exec->execute();
            $encoder->status_berhasil();
        } catch(Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function requestDeleteFile() {
        require_once standart_path.'server_script/file_controller/FileDelete.php';
        $encoder = new json_encoder();
        try {
            $exec = new FileDelete(filter_input(INPUT_POST, FileDelete::PATHS),
                filter_input(INPUT_POST, FileDelete::LOGIN_ID));
            $exec->execute();
            $encoder->status_berhasil();
        } catch (Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function requestUploadFileFinish() {
        require_once standart_path.'server_script/file_controller/upload/FileUploadFinish.php';
        $encoder = new json_encoder();
        try {
            $exec = new FileUploadFinish(filter_input(INPUT_POST, FileUploadFinish::ID),
                filter_input(INPUT_POST, FileUploadFinish::LOGIN_ID));
            $exec->execute();
            $encoder->status_berhasil();
        } catch (Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function requestUploadFileUploadFileSize() {
        require_once standart_path.'server_script/file_controller/upload/FileUploadGetUploadedFileSize.php';
        $encoder = new json_encoder();
        try {
            $exec = new FileUploadGetUploadedFileSize(
                filter_input(INPUT_POST, FileUploadGetUploadedFileSize::ID),
                filter_input(INPUT_POST, FileUploadGetUploadedFileSize::FILE_ID),
                filter_input(INPUT_POST, FileUploadGetUploadedFileSize::LOGIN_ID)
            );
            $res = $exec->execute();
            $encoder->status_berhasil();
            $encoder->set_result($res);
        } catch( Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function requestUploadFileUploadTotalFileSize() {
        require_once standart_path.'server_script/file_controller/upload/FileUploadTotalUploadedSize.php';
        $encoder = new json_encoder();
        try {
            $exec = new FileUploadTotalUploadedSize(filter_input(INPUT_POST, FileUploadTotalUploadedSize::ID), filter_input(INPUT_POST, FileUploadTotalUploadedSize::LOGIN_ID));
            $res = $exec->execute();
            $encoder->status_berhasil();
            $encoder->set_result($res);
        } catch (Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function requestUploadFile() {
        require_once standart_path.'server_script/file_controller/upload/FileUpload.php';
        $encoder = new json_encoder();
        try {
            if( !isset($_FILES[FileUpload::FILE_BIN]) || empty($_FILES[FileUpload::FILE_BIN]))
                throw new Exception("File Upload Not Found");
            $exec = new FileUpload(
                filter_input(INPUT_POST, FileUpload::UPLOAD_ID),
                filter_input(INPUT_POST, FileUpload::FILE_ID),
                filter_input(INPUT_POST, FileUpload::FILE_SIZE),
                $_FILES[FileUpload::FILE_BIN],
                filter_input(INPUT_POST, FileUpload::LOGIN_ID)
            );
            $exec->execute();
            $encoder->status_berhasil();
        } catch(Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function requestUploadFileBegin() {
        require_once standart_path.'server_script/file_controller/upload/FileUploadBegin.php';
        $encoder = new json_encoder();
        try {
            $exec = new FileUploadBegin(filter_input(INPUT_POST, FileUploadBegin::FILES),
                filter_input(INPUT_POST, FileUploadBegin::LOGIN_ID),
                filter_input(INPUT_POST, FileUploadBegin::PARENT));
            $res = $exec->execute();
            $encoder->status_berhasil();
            $encoder->set_result($res);
        } catch (Exception $ex) {
            $encoder->status_gagal();
            $encoder->set_message($ex->getMessage());
        }
        $encoder->show();
    }

    private function requestDownloadFile()
    {
        require_once standart_path.'server_script/file_controller/download/FileDownloadPrepare.php';
        $exec = new FileDownloadPrepare(filter_input(INPUT_POST, FileDownloadPrepare::DOWNLOAD_ID),
            filter_input(INPUT_POST, FileDownloadPrepare::FILE_ID),
            filter_input(INPUT_POST, FileDownloadPrepare::BEGIN_BYTE),
            filter_input(INPUT_POST, FileDownloadPrepare::END_BYTE),
            filter_input(INPUT_POST, FileDownloadPrepare::LOGIN_ID));
        $exec->execute();
    }

    private function downloadFileBegin()
    {
        $encoder = new json_encoder();
        require_once standart_path.'server_script/file_controller/download/DownloadFileBegin.php';
        try {
            $exec = new DownloadFileBegin(filter_input(INPUT_POST, DownloadFileBegin::LOGIN_ID),
                filter_input(INPUT_POST, DownloadFileBegin::IDS),
                filter_input(INPUT_POST, DownloadFileBegin::AS_ZIP)
            );
            $result = $exec->execute();
            $encoder->status_berhasil();
            $encoder->set_result($result);
        } catch(Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }
    private function getRecycleBin() {
        require_once standart_path.'server_script/file_controller/GetRecycleBin.php';
        $encoder = new json_encoder();
        try {
            $exec = new GetRecycleBin(filter_input(INPUT_POST, GetRecycleBin::INPUT_TIME, FILTER_VALIDATE_INT),
                filter_input(INPUT_POST, GetRecycleBin::LOGIN_ID));
            $result = $exec->execute();
            $encoder->status_berhasil();
            $encoder->set_result($result);
        }
        catch(Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }
    private function getFile()
    {
        $encoder = new json_encoder();
        require_once standart_path.'server_script/file_controller/GetFile.php';
        try {
            $exec = new GetFile(filter_input(INPUT_POST, GetFile::INPUT_TIME, FILTER_VALIDATE_INT), filter_input(INPUT_POST, GetFile::INPUT_PARENT), filter_input(INPUT_POST, GetFile::LOGIN_ID));
            $result = $exec->execute();
            $encoder->status_berhasil();
            $encoder->set_result($result);
        }
        catch(Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }
    private function request_registration()
    {
        require_once standart_path.'server_script/Register/Register.php';
        $encoder = new json_encoder();
        try {
            $register = new Register(filter_input(INPUT_POST, var_id),
                filter_input(INPUT_POST, var_username),
                filter_input(INPUT_POST, var_email, FILTER_SANITIZE_EMAIL),
                filter_input(INPUT_POST, var_registration_password));
            $token = $register->execute();
            $encoder->status_berhasil();
            $encoder->set_message("Registration confirmation has been sent to your email");
            $encoder->set_result($token);
        } catch(Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }
    private function request_login()
    {
        require_once standart_path.'server_script/JSON/Login/LoginJSON.php';
        $encoder = new json_encoder();
        try {
            $exec = new LoginJSON($this->app_type,
                filter_input(INPUT_POST, LoginJSON::ID_USER_USERNAME_EMAIL),
                filter_input(INPUT_POST, LoginJSON::PASSWORD));
            $res = $exec->execute();
            $encoder->status_berhasil();
            $encoder->set_result($res);
        } catch(Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }
    private function request_logout()
    {
        require_once standart_path.'server_script/JSON/Login/Logout_json.php';
        $exec = new Logout_json();
        $exec->execute();
    }
    private function registration_get_benutzer_id()
    {
        require_once standart_path.'server_script/JSON/get_betnutzer_id_json.php';
        $exec = new get_betnutzer_id_json();
        $exec->execute();
    }
    private function confirmAppKey()
    {
        require_once standart_path.'server_script/ConfirmAppKey.php';
        $encoder = new json_encoder();
        try {
            $exec = new ConfirmAppKey($this->app_type);
            $header = getallheaders();
            if(!isset($header[var_app_key]) || empty($header[var_app_key])) throw new Exception("missing Application Key");
            $exec->execute($header[var_app_key]);
            $encoder->status_berhasil();
        } catch (Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function requestGetDownloadAccesOnFile() {
        require_once standart_path.'server_script/file_controller/access/GetDownloadAccessOnFile.php';
        $encoder = new json_encoder();
        try {
            $exec = new GetDownloadAccessOnFile(filter_input(INPUT_POST, GetDownloadAccessOnFile::LOGIN_ID),
                filter_input(INPUT_POST, GetDownloadAccessOnFile::FILE_ID),
                filter_input(INPUT_POST, GetDownloadAccessOnFile::USER_IDS));
            $exec->execute();
        } catch(Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function requestMarkFileAsPrivilegeByAll() {
        require_once standart_path.'server_script/file_controller/MarkFileAsPrivilegeByAll.php';
        $encoder = new json_encoder();
        try {
            $exec = new MarkFileAsPrivilegeByAll(filter_input(INPUT_POST, MarkFileAsPrivilegeByAll::ID),
                filter_input(INPUT_POST, MarkFileAsPrivilegeByAll::FLAG),
                filter_input(INPUT_POST, MarkFileAsPrivilegeByAll::TYPE),
                filter_input(INPUT_POST, MarkFileAsPrivilegeByAll::LOGIN_ID));
            $exec->execute();
        } catch(Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function requestGetUserList() {
        require_once standart_path.'server_script/GetUserList.php';
        $encoder = new json_encoder();
        try {
            $exec = new GetUserList();
            $res = $exec->execute();
            $encoder->set_result($res);
        } catch(Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function requestGetUserListOnFilePrivilege() {
        require_once standart_path.'server_script/file_controller/GetUserListOnFilePrivilege.php';
        $encoder = new json_encoder();
        try {
            $exec = new GetUserListOnFilePrivilege(filter_input(INPUT_POST, GetUserListOnFilePrivilege::INPUT_FILE_ID),
                filter_input(INPUT_POST, GetUserListOnFilePrivilege::INPUT_PRIVILEGE_TYPE),
                filter_input(INPUT_POST, GetUserListOnFilePrivilege::LOGIN_ID));
            $res = $exec->execute();
            $encoder->status_berhasil();
            $encoder->set_result($res);
        } catch(Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function requestAddUserListOnFilePrivilege() {
        require_once standart_path.'server_script/file_controller/AddUserListOnFilePrivilege.php';
        $encoder = new json_encoder();
        try {
            $exec = new AddUserListOnFilePrivilege(filter_input(INPUT_POST, AddUserListOnFilePrivilege::FILE_ID),
                filter_input(INPUT_POST, AddUserListOnFilePrivilege::USER_ID_USERNAME_EMAIL),
                filter_input(INPUT_POST, AddUserListOnFilePrivilege::TYPE),
                filter_input(INPUT_POST, AddUserListOnFilePrivilege::LOGIN_ID));
            $exec->execute();
            $encoder->status_berhasil();
        } catch(Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function requestDeleteDeletedFile() {
        require_once standart_path.'server_script/file_controller/DeleteDeletedFile.php';
        $encoder = new json_encoder();
        try {
            $exec = new DeleteDeletedFile(filter_input(INPUT_POST, DeleteDeletedFile::PATHS),
                filter_input(INPUT_POST, DeleteDeletedFile::LOGIN_ID));
            $exec->execute();
            $encoder->status_berhasil();
        } catch(Exception $e) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function getAppKey()
    {
        require_once standart_path.'server_script/GetAppKey.php';
        $encoder = new json_encoder();
        try {
            $exec = new GetAppKey($this->app_type);
            $res = $exec->execute();
            $encoder->status_berhasil();
            $encoder->set_result($res);
        } catch( Exception $e ) {
            $encoder->status_gagal();
            $encoder->set_message($e->getMessage());
        }
        $encoder->show();
    }

    private function isAkeyValid() : bool {
        $headers = getallheaders();
        if(!isset($headers[var_app_key]) || empty($headers[var_app_key])) return false;
        else {
            $this->akey = $headers[var_app_key];
            $session = new Session();
            $session->startSession();
            if($session->isContainByKey(session_app_key)) {
                $sakey = unserialize($session->getSessionByKey(session_app_key));
                if($sakey instanceof AkeySession ) {
                    if(strcmp($sakey->getAkey(), $this->akey) === 0) return true;
                    else return false;
                } else return false;
            }
            else {
                $select = new Select('app_list_winapi');
                $select->append_column('app_id');
                $select->append_column('owner');
                $select->append_where('app_key = ?');
                $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->akey));
                $runner = new Runner();
                $runner->connect(host, port, database, database_username, database_password);
                $runner->append_query($select);
                $runner->execute();
                $result = $select->result();
                if(count($result) > 0)
                {
                    $sakey = new AkeySession($result[0][0], $this->akey, $result[0][1]);
                    $session->addSessionByKey(serialize($sakey), session_app_key);
                    return true;
                }
                else return false;
            }
        }
    }

    public function requestParserPaths() {
        require_once standart_path.'server_script/CloudPathParserRequest.php';
    }
}
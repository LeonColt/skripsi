<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 13/02/2017
 * Time: 13:22
 */
require_once '../../core/global_variable.php';
require_once '../../core/database/Runner.php';
require_once '../../core/UniversalMethode.php';
require_once '../../core/Session/Session.php';
require_once standart_path.'server_script/file_controller/CloudPathParser.php';
class CloudPathParserRequest
{
    const GET_FILE_ID = "cf-cppr-8789955585897959"; //string then cast to UINT64
    const GET_FILE_NAME = "cf-cppr-7864051513135464313"; //string
    const GET_FILE_SIZE = "cf-cppr-8485964928658996349252"; //string then cast to UINT64
    const GET_FILE_OWNER = "cf-cppr-8589649621635825420032"; //string
    const GET_FILE_EXISTS = "cf-cppr-02850205954057947"; //bool
    const GET_FILE_PARENT = "cf-cppr-02850205349037403"; //string then cast to UINT64
    const GET_FILE_SEEN_BY_ALL = "cf-cppr-3874982742936427423"; //bool
    const GET_FILE_DOWNLOAD_BY_ALL = "cf-cppr-995-0350937853953953905834";//bool
    const GET_FILE_MODIFY_BY_ALL = "cf-cppr-328585874892374289472";//bool
    const GET_FILE_DOWNLOAD_ALLOWED_CURRENT_USER = "cf-cppr-439048028490238402489204";//bool
    const GET_FILE_UPLOAD_ON_DIRECTORY_ALLOWED_CURRENT_USER = "cf-cppr-349823904823849203482904";//bool
    const GET_FILE_DELETE_ALLOWED_CURRENT_USER = "cf-cppr-39420948238420482084294";//bool
    const GET_FILE_IS_DIRECTORY = "cf-cppr-4983209482394723894723742"; //bool
    const GET_FILE_DATE_CREATED = "cf-cppr-3942948248242949204"; //string

    const PATHS = "cf-cppr-4394728498274829742897428";
    const LOGIN_ID ="cf-cppr-342384728947289479474";
    private $paths, $benutzer;
    public function __construct(string $paths, $login_id)
    {
        $this->paths = $paths;
        $this->benutzer = getUser($login_id);
    }
    public function execute() : array {
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $ccp = new CloudPathParser();
        $files = $ccp->parse($this->paths, $runner);
        return $this->prepareFile($files);
    }
    private function prepareFile(array $files) : array {
        $result = array();
        foreach( $files as $file) {
            if( $file instanceof SDirectory) {
                $temp = array();
                $temp[CloudPathParserRequest::GET_FILE_ID] = (string)$file->getId();
                $temp[CloudPathParserRequest::GET_FILE_NAME] = $file->getName();
                $temp[CloudPathParserRequest::GET_FILE_IS_DIRECTORY] = true;
                $temp[CloudPathParserRequest::GET_FILE_EXISTS] = true;
                $temp[CloudPathParserRequest::GET_FILE_SIZE] = (string)$file->getSize();
                $temp[CloudPathParserRequest::GET_FILE_OWNER] = ( $file->getOwner() === null) ? "indenpendence" : $file->getOwner()->getUsername();
                //$temp[CloudPathParserRequest::GET_FILE_PARENT] = (string)( $file["username"] == null) ? null : ($file->getParent()->getId());
                $temp[CloudPathParserRequest::GET_FILE_SEEN_BY_ALL] = $file->getSecurityFileInfo()->isSeenByAll();
                $temp[CloudPathParserRequest::GET_FILE_DOWNLOAD_BY_ALL] = $file->getSecurityFileInfo()->isDownloadByAll();
                $temp[CloudPathParserRequest::GET_FILE_MODIFY_BY_ALL] = $file->getSecurityFileInfo()->isModifyByAll();
                $temp[CloudPathParserRequest::GET_FILE_DOWNLOAD_ALLOWED_CURRENT_USER] = $file->getSecurityFileInfo()->isAllowedToDownload($this->benutzer);
                $temp[CloudPathParserRequest::GET_FILE_DELETE_ALLOWED_CURRENT_USER] = $file->getSecurityFileInfo()->isAllowedToDelete($this->benutzer);
                $temp[CloudPathParserRequest::GET_FILE_DATE_CREATED] = $file->getDateCreated();
                array_push($result, $temp);
            }
            else if($file instanceof File) {
                $temp = array();
                $temp[CloudPathParserRequest::GET_FILE_ID] = (string)$file->getId();
                $temp[CloudPathParserRequest::GET_FILE_NAME] = $file->getName();
                $temp[CloudPathParserRequest::GET_FILE_IS_DIRECTORY] = false;
                $temp[CloudPathParserRequest::GET_FILE_EXISTS] = true;
                $temp[CloudPathParserRequest::GET_FILE_SIZE] = (string)$file->getSize();
                $temp[CloudPathParserRequest::GET_FILE_OWNER] = ( $file->getOwner() === null) ? "indenpendence" : $file->getOwner()->getUsername();
                //$temp[GetFile::GET_FILE_PARENT] = (string)( $file["username"] == null) ? null : ($file->getParent()->getId());
                $temp[CloudPathParserRequest::GET_FILE_SEEN_BY_ALL] = $file->getSecurityFileInfo()->isSeenByAll();
                $temp[CloudPathParserRequest::GET_FILE_DOWNLOAD_BY_ALL] = $file->getSecurityFileInfo()->isDownloadByAll();
                $temp[CloudPathParserRequest::GET_FILE_MODIFY_BY_ALL] = $file->getSecurityFileInfo()->isModifyByAll();
                $temp[CloudPathParserRequest::GET_FILE_DOWNLOAD_ALLOWED_CURRENT_USER] = $file->getSecurityFileInfo()->isAllowedToDownload($this->benutzer);
                $temp[CloudPathParserRequest::GET_FILE_DELETE_ALLOWED_CURRENT_USER] = $file->getSecurityFileInfo()->isAllowedToDelete($this->benutzer);
                $temp[CloudPathParserRequest::GET_FILE_DATE_CREATED] = $file->getDateCreated();
                array_push($result, $temp);
            }
            else throw new Exception("Forbidden file template");
        }
        return $result;
    }
}

$exec = new CloudPathParserRequest("Cloud:\\Document\\PDF\\ | Cloud::\\test.zip", null);
var_dump($exec->execute());
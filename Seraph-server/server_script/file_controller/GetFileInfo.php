<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 09/02/2017
 * Time: 13:25
 */
class GetFileInfo
{
    const GET_FILE_ID = "cf-gafsaf-8789955585897959"; //string then cast to UINT64
    const GET_FILE_NAME = "cf-gafsaf-7864051513135464313"; //string
    const GET_FILE_SIZE = "cf-gafsaf-8485964928658996349252"; //string then cast to UINT64
    const GET_FILE_OWNER = "cf-gafsaf-8589649621635825420032"; //string
    const GET_FILE_EXISTS = "cf-gafsaf-02850205954057947"; //bool
    const GET_FILE_PARENT = "cf-gafsaf-02850205349037403"; //string then cast to UINT64
    const GET_FILE_SEEN_BY_ALL = "cf-gafsaf-3874982742936427423"; //bool
    const GET_FILE_DOWNLOAD_BY_ALL = "cf-gafsaf-995-0350937853953953905834";//bool
    const GET_FILE_MODIFY_BY_ALL = "cf-gafsaf-328585874892374289472";//bool
    const GET_FILE_DOWNLOAD_ALLOWED_CURRENT_USER = "cf-gafsaf-439048028490238402489204";//bool
    const GET_FILE_UPLOAD_ON_DIRECTORY_ALLOWED_CURRENT_USER = "cf-gafsaf-349823904823849203482904";//bool
    const GET_FILE_DELETE_ALLOWED_CURRENT_USER = "cf-gafsaf-39420948238420482084294";//bool
    const GET_FILE_IS_DIRECTORY = "cf-gafsaf-4983209482394723894723742"; //bool
    const GET_FILE_DATE_CREATED = "cf-gafsaf-3942948248242949204"; //string
    private $time, $id, $benutzer;
    public function __construct(int $time, int $id, $login_id)
    {
        $this->time = $this;
        $this->id = $id;
        $this->benutzer = getUser($login_id);
    }
    public function execute() {

    }
    private function prepareFile(array $files) : array {
        $result = array();
        foreach( $files as $file) {
            if( $file instanceof SDirectory) {
                $temp = array();
                $temp[GetFile::GET_FILE_ID] = (string)$file->getId();
                $temp[GetFile::GET_FILE_NAME] = $file->getName();
                $temp[GetFile::GET_FILE_IS_DIRECTORY] = true;
                $temp[GetFile::GET_FILE_EXISTS] = true;
                $temp[GetFile::GET_FILE_SIZE] = (string)$file->getSize();
                $temp[GetFile::GET_FILE_OWNER] = ( $file->getOwner() === null) ? "indenpendence" : $file->getOwner()->getUsername();
                //$temp[GetFile::GET_FILE_PARENT] = (string)( $file["username"] == null) ? null : ($file->getParent()->getId());
                $temp[GetFile::GET_FILE_SEEN_BY_ALL] = $file->getSecurityFileInfo()->isSeenByAll();
                $temp[GetFile::GET_FILE_DOWNLOAD_BY_ALL] = $file->getSecurityFileInfo()->isDownloadByAll();
                $temp[GetFile::GET_FILE_MODIFY_BY_ALL] = $file->getSecurityFileInfo()->isModifyByAll();
                $temp[GetFile::GET_FILE_DOWNLOAD_ALLOWED_CURRENT_USER] = $file->getSecurityFileInfo()->isAllowedToDownload($this->benutzer);
                $temp[GetFile::GET_FILE_DELETE_ALLOWED_CURRENT_USER] = $file->getSecurityFileInfo()->isAllowedToDelete($this->benutzer);
                $temp[GetFile::GET_FILE_DATE_CREATED] = $file->getDateCreated();
                array_push($result, $temp);
            }
            else if($file instanceof File) {
                $temp = array();
                $temp[GetFile::GET_FILE_ID] = (string)$file->getId();
                $temp[GetFile::GET_FILE_NAME] = $file->getName();
                $temp[GetFile::GET_FILE_IS_DIRECTORY] = false;
                $temp[GetFile::GET_FILE_EXISTS] = true;
                $temp[GetFile::GET_FILE_SIZE] = (string)$file->getSize();
                $temp[GetFile::GET_FILE_OWNER] = ( $file->getOwner() === null) ? "indenpendence" : $file->getOwner()->getUsername();
                //$temp[GetFile::GET_FILE_PARENT] = (string)( $file["username"] == null) ? null : ($file->getParent()->getId());
                $temp[GetFile::GET_FILE_SEEN_BY_ALL] = $file->getSecurityFileInfo()->isSeenByAll();
                $temp[GetFile::GET_FILE_DOWNLOAD_BY_ALL] = $file->getSecurityFileInfo()->isDownloadByAll();
                $temp[GetFile::GET_FILE_MODIFY_BY_ALL] = $file->getSecurityFileInfo()->isModifyByAll();
                $temp[GetFile::GET_FILE_DOWNLOAD_ALLOWED_CURRENT_USER] = $file->getSecurityFileInfo()->isAllowedToDownload($this->benutzer);
                $temp[GetFile::GET_FILE_DELETE_ALLOWED_CURRENT_USER] = $file->getSecurityFileInfo()->isAllowedToDelete($this->benutzer);
                $temp[GetFile::GET_FILE_DATE_CREATED] = $file->getDateCreated();
                array_push($result, $temp);
            }
            else throw new Exception("Forbidden file template");
        }
        return $result;
    }
}
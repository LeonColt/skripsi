<?php

/**
 * Created by PhpStorm.
 * User: LeonColt
 * Date: 11/14/2016
 * Time: 11:44 AM
 */
require_once standart_path.'server_script/file_controller/file_object/FileLoader.php';
require_once standart_path.'server_script/file_controller/GetFile.php';
class SearchFile
{
    const FILE_NAME = "nmlf-sf-2409308908409274832978138213";
    const LOGIN_ID = "nmlf-sf-385904890284092750248209405980437580354";
    private $name, $benutzer;
    public function __construct(string $name, $login_id)
    {
        $this->name = '%';
        $this->name .= $name;
        $this->name .= '%';
        $this->benutzer = getUser($login_id);
    }

    public function execute() : array {
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        return $this->prepareFile($this->getFile($runner));
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
    private function getFile(Runner &$runner) : array {
        $temp = array();
        $select_uncredible = new Select('file');
        $select_uncredible->append_column('id');
        $select_uncredible->append_where('name LIKE ?');
        $select_uncredible->append_parameter(new Parameter($select_uncredible->getParameterVariableIntegerOrder(), $this->name));
        $select_uncredible->append_where('(seen_by_all = 1', 'AND');
        $select_uncredible->append_where('owner IS NULL)', 'OR');
        $runner->append_query($select_uncredible);
        $runner->execute();
        $runner->clear_query();
        foreach($select_uncredible->result() as $item) array_push($temp, $item["id"]);

        if($this->benutzer !== null) {
            $select_credible = new Select('file');
            $select_credible->append_column('id');
            $select_credible->append_where('name LIKE ?');
            $select_credible->append_parameter(new Parameter($select_uncredible->getParameterVariableIntegerOrder(), $this->name));
            $select_credible->append_where('owner = ?', 'AND');
            $select_credible->append_parameter(new Parameter($select_credible->getParameterVariableIntegerOrder(), $this->benutzer->getId()));
            $runner->clear_query();
            $runner->append_query($select_credible);
            $runner->execute();
            $runner->clear_query();
            foreach($select_credible->result() as $item) array_push($temp, $item["id"]);
        }
        $loader = new FileLoader();
        return $loader->load($temp, $runner, true);
    }
}
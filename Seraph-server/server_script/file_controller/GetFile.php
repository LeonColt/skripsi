<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 13/04/2016
 * Time: 16:31
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
require_once standart_path.'server_script/file_controller/file_object/SDirectory.php';
require_once standart_path.'server_script/file_controller/file_object/File.php';
require_once standart_path.'server_script/file_controller/file_object/FileLoader.php';
require_once standart_path.'server_script/file_controller/FilePrivilegeConstant.php';
class GetFile
{
    const INPUT_PARENT = "cf-gf-23433434";
    const INPUT_TIME = "cf-gf-897531590";
    const LOGIN_ID = "cf-gf-93048903284203";

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
    private $parent = null, $login_id, $benutzer, $time;
    public function __construct(int $time, $parent = null, $login_id = null) {
        $this->time = $time;
        $this->parent = $parent;
        $this->login_id = $login_id;
        $this->benutzer = getUser($login_id);
    }

    public function execute() : array {
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        if((int)$this->time === -1) return $this->prepareFile($this->getFile($runner));
        else
        {
            set_time_limit(0); // set limit waktu tak terbatas
            $sleep_time = 1; //tick every seconds
            $total_sleep_max = 5; //15 menit
            //$total_sleep_max = 900; //15 menit
            $sleep_time_total = 0;
            for(;;)
            {
                //echo "\0";
                //if( connection_aborted()) break;
                if($this->isThereAnyChange() || $sleep_time_total > ($total_sleep_max - 1))
                    return $this->prepareFile($this->getFile($runner));
                sleep($sleep_time);
                $sleep_time_total += $sleep_time;
            }
        }
        return array();
    }

    public function isThereAnyChange() : bool
    {
        $select = new Select('file_change_recorder');
        $select->append_column('1');
        $select->append_where('id_dir = ?');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->parent));
        $select->append_where(' UNIX_TIMESTAMP(waktu) > ?', 'AND');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->time));

        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $runner->append_query($select);
        $runner->execute();
        return count($select->result()) > 0;
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
        if($this->checkVarParentExists())
        {
            $select_uncredible->append_where('parent = ?');
            $select_uncredible->append_parameter(new Parameter($select_uncredible->getParameterVariableIntegerOrder(), $this->parent));
        }
        else $select_uncredible->append_where('parent IS NULL');
        $select_uncredible->append_where('(seen_by_all = 1', 'AND');
        $select_uncredible->append_where('owner IS NULL)', 'OR');
        $runner->append_query($select_uncredible);
        $runner->execute();
        $runner->clear_query();
        foreach($select_uncredible->result() as $item) array_push($temp, $item["id"]);

        if($this->benutzer !== null) {
            $select_credible = new Select('file');
            $select_credible->append_column('id');
            if($this->checkVarParentExists())
            {
                $select_credible->append_where('parent = ?');
                $select_credible->append_parameter(new Parameter($select_credible->getParameterVariableIntegerOrder(), $this->parent));
            }
            else $select_credible->append_where('parent IS NULL');
            $select_credible->append_where('owner = ?', 'AND');
            $select_credible->append_parameter(new Parameter($select_credible->getParameterVariableIntegerOrder(), $this->benutzer->getId()));
            $runner->clear_query();
            $runner->append_query($select_credible);
            $runner->execute();
            $runner->clear_query();
            foreach($select_credible->result() as $item) array_push($temp, $item["id"]);

            $select_seen = new Select('privilege_benutzer_on_object');
            $select_seen->append_column('id', 'file');
            $select_seen->append_join('JOIN', 'file');
            $select_seen->append_on('privilege_benutzer_on_object', 'file_id', '=', 'file', 'id');
            if($this->checkVarParentExists())
            {
                $select_credible->append_where('file.parent = ?');
                $select_credible->append_parameter(new Parameter($select_credible->getParameterVariableIntegerOrder(), $this->parent));
            }
            else $select_credible->append_where('file.parent IS NULL');
            $select_seen->append_where('privilege_id = ?');
            $select_seen->append_parameter(new Parameter($select_seen->getParameterVariableIntegerOrder(), FilePrivilegeConstant::SEEN));
            $runner->clear_query();
            $runner->append_query($select_seen);
            $runner->execute();
            $runner->clear_query();
            foreach($select_seen->result() as $item)
                array_push($temp, $item["id"]);
        }
        $loader = new FileLoader();
        return $loader->load($temp, $runner, true);
    }
    private function checkVarParentExists() : bool
    {
        if( $this->parent === null ) return false;
        else {
            if(!is_numeric($this->parent)) throw new Exception("Parent is not correctly in format");
            if((int)$this->parent !== 0) return true;
        }
        return false;
    }
}
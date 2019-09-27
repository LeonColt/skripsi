<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 25/04/2016
 * Time: 14:46
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
require_once standart_path.'server_script/file_controller/download/FileDownload.php';
final class FileDownloadPrepare
{
    const DOWNLOAD_ID = "cf-df-0432434675567";
    const FILE_ID = "cf-df-39859759837509274892589325";
    const BEGIN_BYTE = "cf-df-22948938402";
    const END_BYTE = "cf-df-235984035847";
    const LOGIN_ID = "cf-df-59408509802480234";
    private $download_id, $file_id, $benutzer, $file, $name, $begin, $end;
    public function __construct(string $download_id, string $file_id, $begin, $end, $login_id)
    {
        $this->benutzer = null;
        $this->download_id = $download_id;
        $this->file_id = $file_id;
        $this->begin = $begin;
        $this->end = $end;
        $this->benutzer = getUser($login_id);
    }

    public function execute()
    {
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $this->validateRequest($runner);
        $this->getBenutzerandFile($runner);

        if($this->file === null || $this->file === 0) {
            header("File Not Found", true, 404);
            echo "File Not Found";
            exit(0);
        }
        $key = $this->getKeyFile($this->file);
        if(strcmp($key, "") === 0) {
            header("File is missing or forbidden", true, 520);
            echo "Missing or forbidden Download Request";
            exit(0);
        }
        $begin = ($this->begin !== null) ? $this->begin : 0;
        $end = ($this->end !== null) ? $this->end : -1;
        $fd = new FileDownload($key, $this->name, $begin, $end);
        $fd->execute();
        /*
        if($update->getResult())
        {

        }
        else
        {
            header("request state cannot be changed", true, 520);
            echo "Request state cannot be changed";
            exit(0);
        }
        */
    }

    public function getBenutzerandFile(Runner &$runner)
    {
        $select = new Select('download_record');
        $select->append_column('id', 'download_record');
        $select->append_column('benutzer', 'download_record');
        $select->append_column('id', 'file');
        $select->append_column('name', 'file');
        $select->append_join('JOIN', 'download_record_files');
        $select->append_on('download_record', 'id', '=', 'download_record', 'id');
        $select->append_join('JOIN', 'file');
        $select->append_on('download_record_files', 'file_id', '=', 'file', 'id');
        $select->append_where('download_record.id = ?');
        $select->append_where('file.id = ?', 'AND');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->download_id));
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->file_id));
        //$select->distinct();
        $runner->clear_query();
        $runner->append_query($select);
        $runner->execute();
        $result = $select->result();
        if(count($result) > 0)
        {
            $this->download_id = $result[0][0];
            $this->benutzer = $result[0][1];
            $this->file = $result[0][2];
            $this->name = $result[0][3];
        }
        else $this->download_id = null;
    }

    public function getKeyFile(string $id_file) : string
    {
        return $id_file;
        /*
        $xml = simplexml_load_file(standart_path.'daten/daten_representator.xml');
        foreach($xml as $daten)
        {
            if((int)$daten->id === (int)$id_file) return $daten->key;
        }
        return "";
        */
    }
    public function validateRequest(Runner &$runner) {
        $select = new Select('download_record');
        $select->append_column('id', 'download_record', 'download_id');
        $select->append_column('id', 'file', 'file_id');
        $select->append_column('download_by_all', 'file');
        $select->append_column('owner', 'file');
        $select->append_join('JOIN', 'download_record_files');
        $select->append_on('download_record', 'id', '=', 'download_record', 'id');
        $select->append_join('JOIN', 'file');
        $select->append_on('download_record_files', 'file_id', '=', 'file', 'id');
        $select->append_where('download_record.id = ?');
        $select->append_where('file.id = ?', 'AND');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->download_id));
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->file_id));
        if($this->benutzer === null)
            $select->append_where('download_record.benutzer IS NULL', 'AND');
        else {
            $select->append_where('download_record.benutzer = ?', 'AND');
            $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->benutzer->getId()));
        }
        $select->distinct();
        $runner->clear_query();
        $runner->append_query($select);
        $runner->execute();
        if(count($select->result()) > 0)
        {
            if((int)$select->result()[0][2] === 0)
            {
                $session = new Session();
                $session->startSession();
                if($session->isContainByKey(session_login))
                {
                    $session_login = unserialize($session->getSessionByKey(session_login));
                    if($session_login->get_user_id() !== $runner->result($runner->get_default_key())[0][0][2])
                    {
                        header("file download not allowed", true, 520);
                        echo "Forbidden Download Request";
                        exit(0);
                    }
                    else $this->benutzer = $session_login->get_user_id();
                }
                else {
                    header("file download not allowed", true, 520);
                    echo "Forbidden Download Request";
                    exit(0);
                }
            }
            else
            {
                $this->download_id = $select->result()[0][0];
                $this->file_id = $select->result()[0][1];
            }
        }
        else {
            header("Download Request Not Found", true, 404);
            echo "Illegal Download Request";
            exit(0);
        }
    }
    private function tipeFile(string $ekstensi)
    {
        switch($ekstensi)
        {
            case 'swf' : return 'application/x-shockwave-flash';
            case 'pdf' : return 'application/pdf';
            case 'exe' : return 'application/octet-stream';
            case 'zip' : return 'application/zip';
            case 'doc' : return 'application/msword';
            case 'xls' : return 'application/vnd.ms-excel';
            case 'ppt' : return 'application/vnd.ms-powerpoint';
            case 'gif' : return 'image/gif';
            case 'png' : return 'image/png';
            case 'jpeg' : return 'image/jpg';
            case 'jpg' : return 'image/jpg';
            case 'rar' : return 'application/rar';

            case 'ra' : return 'audio/x-pn-realaudio';
            case 'ram' : return 'audio/x-pn-realaudio';
            case 'ogg' : return 'audio/x-pn-realaudio';

            case 'wav' : return 'video/x-msvideo';
            case 'wmv' : return 'video/x-msvideo';
            case 'avi' : return 'video/x-msvideo';
            case 'asf' : return 'video/x-msvideo';
            case 'divx' : return 'video/x-msvideo';

            case 'mp3' : return 'audio/mpeg';
            case 'mp4' : return 'audio/mpeg';
            case 'mpeg' : return 'video/mpeg';
            case 'mpg' : return 'video/mpeg';
            case 'mpe' : return 'video/mpeg';
            case 'mov' : return 'video/quicktime';
            case '3gp' : return 'video/quicktime';
            case 'm4a' : return 'video/quicktime';
            case 'aac' : return 'video/quicktime';
            case 'm3u' : return 'video/quicktime';
            default : return "";
        }
    }
}
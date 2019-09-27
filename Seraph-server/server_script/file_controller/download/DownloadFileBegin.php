<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 20/04/2016
 * Time: 14:35
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
require_once standart_path.'server_script/file_controller/file_object/SDirectory.php';
require_once standart_path.'server_script/file_controller//file_object/File.php';
require_once standart_path.'core/User.php';
require_once standart_path.'core/Random.php';
require_once standart_path.'server_script/file_controller/CloudPathParser.php';
class DownloadedDirectory extends SDirectory {
    private $original;
    public function __construct()
    {
        parent::__construct();
        $this->original = false;
    }
    /**
     * @return bool
     */
    public function isOriginal() : bool{return $this->original;}
    /**
     * @param bool $original
     */
    public function setOriginal( bool $original){$this->original = $original;}
}
class DownloadedFile extends File {
    private $original;
    public function __construct()
    {
        parent::__construct();
        $this->original = false;
    }
    /**
     * @return bool
     */
    public function isOriginal() : bool{return $this->original;}
    /**
     * @param bool $original
     */
    public function setOriginal( bool $original){$this->original = $original;}
}
class DownloadFileBegin
{
    const LOGIN_ID = "cf-dfb-8987585585";
    const IDS = "cf-dfb-23433434";
    const AS_ZIP = "cf-dfb-93937589473598";

    const KEMBALIAN_ID = "cf-fdbk-3989375894765894"; //string
    const KEMBALIAN_FILES = "cf-fdbk-39589407948758937534985"; //array
    const KEMBALIAN_FILES_ID = "cf-fdbk-34948957489679837598423"; //string then cast to uint64
    const KEMBALIAN_FILES_NAME = "cf-fdbk-name"; //string
    const KEMBALIAN_FILES_SIZE = "cf-fdbk-size"; //string then cast to uint64
    const KEMBALIAN_FILES_PARENT = "cf-fdbk-parent"; //string then cast to uint64
    const KEMBALIAN_FILES_ORIGINAL = "cf-fdbk-original"; //bool
    const KEMBALIAN_FILES_DIRECTORY = "cf-fdbk-directory"; //bool
    private $benutzer, $paths;
    public function __construct($login_id, $paths, bool $as_zip)
    {
        $this->benutzer = getUser($login_id);
        $this->paths = $paths;
    }
    public function execute() : array
    {
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $files = $this->getFiles($runner);
        if(count($files) == 0) throw new Exception("Files do not exist.");
        $this->checkPrivilege($files);
        $id = $this->prepareIdDownload();
        $insert_record = new Insert('download_record');
        $insert_record->append_column_value('id', new Parameter($insert_record->getParameterVariableIntegerOrder(), $id));
        $insert_record->append_column_value('benutzer', new Parameter($insert_record->getParameterVariableIntegerOrder(), ( ($this->benutzer === null) ? null : $this->benutzer->getId() ) ));
        $insert_record->append_column_value('download_start', new Parameter($insert_record->getParameterVariableIntegerOrder(), time()));
        $insert_files = new Insert('download_record_files');
        foreach($files as $file) {
            /* @var $file BaseFile */
            $insert_files->append_column_value('download_id', new Parameter($insert_files->getParameterVariableIntegerOrder(), $id));
            $insert_files->append_column_value('file_id', new Parameter($insert_files->getParameterVariableIntegerOrder(), $file->getId()));
        }
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $runner->append_query($insert_record, 'download_recorder');
        $runner->append_query($insert_files, 'download_recorder');
        $runner->execute();
        //struktur meta download
        /*
         * 1. download id
         * 2. ->
         * {
         * file id, size, format_id, format,  parent, original
         * }
         * original = true if selected by user, false is derivied download
         */
        $output = array();
        $output[DownloadFileBegin::KEMBALIAN_ID] = $id;
        $output[DownloadFileBegin::KEMBALIAN_FILES] = array();
        foreach ($files as $file) {
            $temp = array();
            if($file instanceof DownloadedDirectory) {
                /** @var $file DownloadedDirectory */
                $temp[DownloadFileBegin::KEMBALIAN_FILES_ID] = (string)$file->getId();
                $temp[DownloadFileBegin::KEMBALIAN_FILES_NAME] = (string)$file->getName();
                $temp[DownloadFileBegin::KEMBALIAN_FILES_SIZE] = (string)$file->getSize();
                $temp[DownloadFileBegin::KEMBALIAN_FILES_DIRECTORY] = true;
                $temp[DownloadFileBegin::KEMBALIAN_FILES_PARENT] = (string)(($file->getParent() == null) ? "0" : $file->getParent()->getId());
                $temp[DownloadFileBegin::KEMBALIAN_FILES_ORIGINAL] = $file->isOriginal();
            }
            else if($file instanceof DownloadedFile) {
                /** @var $file DownloadedFile */
                $temp[DownloadFileBegin::KEMBALIAN_FILES_ID] = (string)$file->getId();
                $temp[DownloadFileBegin::KEMBALIAN_FILES_NAME] = (string)$file->getName();
                $temp[DownloadFileBegin::KEMBALIAN_FILES_SIZE] = (string)$file->getSize();
                $temp[DownloadFileBegin::KEMBALIAN_FILES_DIRECTORY] = false;
                $temp[DownloadFileBegin::KEMBALIAN_FILES_PARENT] = (string)(($file->getParent() == null) ? "0" : $file->getParent()->getId());
                $temp[DownloadFileBegin::KEMBALIAN_FILES_ORIGINAL] = $file->isOriginal();
            }
            array_push($output[DownloadFileBegin::KEMBALIAN_FILES], $temp);
        }
        return $output;
    }
    private function prepareIdDownload() : string {
        $random = new Random();
        $select = new Select('download_record');
        $select->append_column('1');
        $select->append_where('id = ?');
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        do {
            $id = $random->alphaNumeric(id_download_length);
            $select->clear_parameter();
            $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $id));
            $runner->clear_query();
            $runner->append_query($select);
            $runner->execute();
        } while(count($select->result()) > 0);
        return $id;
    }

    /**
     * @param BaseFile[] $files
     * @throws Exception
     */
    private function checkPrivilege(array $files) {
        foreach($files as $file) {
            if( !$file->getSecurityFileInfo()->isAllowedToDownload($this->benutzer))
                throw new Exception("File Not Allowed to Download");
        }
    }
    private function getExtendedFiles(Runner &$runner, array $ids, array &$arr) {
        $loader = new FileLoader();
        $files = $loader->load($ids, $runner);
        foreach($files as $file) {
            if( $file instanceof SDirectory ){
                $dir = new DownloadedDirectory();
                $dir->copyMeta($file);
                array_push($arr, $dir);
                $dir->readChildren();
                $child_ids = array();
                foreach($dir->getChildren() as $child){
                    /** @var BaseFile $child */
                    array_push($child_ids, $child->getId());
                }
                $this->getExtendedFiles($runner, $child_ids, $arr);
            }
            else if( $file instanceof File) {
                $file = new DownloadedFile();
                $file->copyMeta($file);
                array_push($arr, $file);
            }
        }
    }
    private function getFiles(Runner &$runner) : array {
        $cpp = new CloudPathParser();
        $files = $cpp->parse($this->paths, $runner);
        $res = array();
        foreach($files as $file) {
            if( $file instanceof SDirectory ){
                $dir = new DownloadedDirectory();
                $dir->copyMeta($file);
                $dir->setOriginal(true);
                array_push($res, $dir);
                $dir->readChildren();
                $child_ids = array();
                foreach($dir->getChildren() as $child){
                    /** @var BaseFile $child */
                    array_push($child_ids, $child->getId());
                }
                $this->getExtendedFiles($runner, $child_ids, $res);
            }
            else if( $file instanceof File) {
                $df = new DownloadedFile();
                $df->copyMeta($file);
                $df->setOriginal(true);
                array_push($res, $df);
            }
        }
        return $res;
    }
}
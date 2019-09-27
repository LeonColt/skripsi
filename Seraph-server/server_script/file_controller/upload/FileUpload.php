<?php

/**
 * Created by PhpStorm.
 * User: LeonColt
 * Date: 9/24/2016
 * Time: 2:26 PM
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
require_once standart_path.'core/UniversalMethode.php';
class FileUpload
{
    const UPLOAD_ID = "cf-uf-3445693958694505465789487888796546484";
    const FILE_ID = "cf-uf-983094928403297585794035809348039485092292";
    CONST FILE_SIZE = "cf-uf-8798465447846416512136048048484848";
    const LOGIN_ID = "cf-uf-879343757729479247950820482797498";
    const FILE_BIN = "cf-uf-858898584995455968375897598793857924131";
    private $id, $file_id, $size, $login_id, $benutzer, $file;
    public function __construct(string $id, $file_id, int $size, $file, $login_id) {
        $this->id = $id;
        $this->file_id = $file_id;
        $this->size = $size;
        $this->login_id = $login_id;
        $this->file = $file;
        $this->benutzer = getUser($this->login_id);
    }
    public function execute() {
        $this->checkUpload();
        $path = standart_path.'daten/upload/'.$this->id."-".$this->file_id;
        if(file_exists($path)) {
            if (file_put_contents($path, file_get_contents($this->file["tmp_name"]), FILE_APPEND) === FALSE)
                throw new Exception("Unable to move file -a");
        }
        else if( move_uploaded_file($this->file["tmp_name"], $path) === FALSE )
            throw new Exception("Unable to move file; -n; ".$this->file["error"]);
    }
    private function checkUpload() {
        $select = new Select('upload_record');
        $select->append_column('1');
        $select->append_join('INNER JOIN', 'upload_record_files');
        $select->append_on('upload_record', 'upload_id', '=', 'upload_record_files', 'upload_id');
        $select->append_where('upload_record.upload_id = ?');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->id));
        $select->append_where('upload_record_files.file_id = ?', 'AND');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->file_id));
        if( $this->benutzer === null) $select->append_where('benutzer IS NULL', 'AND');
        else {
            $select->append_where('benutzer = ?', 'AND');
            $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->benutzer->getId()));
        }
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $runner->append_query($select);
        $runner->execute();
        if(count($select->result()) < 1)
            throw new Exception('Upload Operation Not Found');
    }
}
<?php

/**
 * Created by PhpStorm.
 * User: LeonColt
 * Date: 11/22/2016
 * Time: 12:26 PM
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
class FileUploadGetUploadedFileSize
{
    const ID = "nmlf-fugufs-489375893479843759375935";
    const FILE_ID = "nmlf-fugufs-48608503480749274923792";
    const LOGIN_ID = "nmlf-fugufs-938508509485092840923842094";
    private $id, $file_id, $benutzer;
    public function __construct(string $id, $file_id, $login_id )
    {
        $this->id = $id;
        $this->file_id = $file_id;
        $this->benutzer = getUser($login_id);
    }
    public function execute() : string {
        $this->checkUpload();
        $path = standart_path.'daten/upload/'.$this->id."-".$this->file_id;
        if( file_exists($path))
            return (string)filesize($path);
        return (string)0;
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
            throw new Exception('File Upload Not Found or Forbidden');
    }
}
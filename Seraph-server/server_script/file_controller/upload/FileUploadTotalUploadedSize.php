<?php

/**
 * Created by PhpStorm.
 * User: LeonColt
 * Date: 11/27/2016
 * Time: 5:52 PM
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
class FileUploadTotalUploadedSize
{
    const ID = "nmlf-futus-348938594087357398753985";
    const LOGIN_ID = "nmlf-futus-87489375934583530957982764821365135176214";
    private $id, $login_id, $benutzer, $files;
    public function __construct($id, $login_id)
    {
        $this->id = $id;
        $this->login_id = $login_id;
        $this->files = null;
        $this->benutzer = getUser($login_id);
    }
    public function execute() : string {
        $this->checkUpload();

        $size = 0;
        foreach( $this->files as $file) {
            $path = standart_path.'daten/upload/'.$this->id."-".$file[0];
            if( file_exists($path)) $size += filesize($path);
        }
        return $size;
    }
    private function checkUpload() {
        $select = new Select('upload_record');
        $select->append_column('file_id', 'upload_record_files');
        $select->append_join('INNER JOIN', 'upload_record_files');
        $select->append_on('upload_record', 'upload_id', '=', 'upload_record_files', 'upload_id');
        $select->append_where('upload_record.upload_id = ?');
        $select->append_parameter(new Parameter($select->getBinderVariableIntegerOrder(), $this->id));
        if( $this->benutzer === null) $select->append_where('benutzer IS NULL', 'AND');
        else {
            $select->append_where('upload_record.benutzer = ?', 'AND');
            $select->append_parameter(new Parameter($select->getBinderVariableIntegerOrder(), $this->benutzer->getId()));
        }
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $runner->append_query($select);
        $runner->execute();
        if(count($select->result()) === 0)
            throw new Exception('File Upload Not Found or Forbidden');
        $this->files = $select->result();
    }
}
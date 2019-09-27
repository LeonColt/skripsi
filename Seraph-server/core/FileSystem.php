<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 20/12/2016
 * Time: 15:15
 */
class FileSystem
{
    const RENAME = 1;
    const MOVE = 2;
    const COPY = 3;
    const DELETE = 4;
    const MOVE_UPLOAD = 5;
    private $operations, $execution_pointer;
    public function __construct(){$this->operations = array();}
    public function rename(string $old, string $new) {array_push($this->operations, array(FileSystem::RENAME, $old, $new));}
    public function move(string $old, string $new) {array_push($this->operations, array(FileSystem::MOVE, $old, $new));}
    public function copy(string $from, string $to) {array_push($this->operations, array(FileSystem::COPY, $from, $to));}
    public function delete(string $path) {array_push($this->operations, array(FileSystem::DELETE, $path));}
    public function moveUpload(string $old, string $new){array_push($this->operations, array(FileSystem::MOVE_UPLOAD, $old, $new));}
    public function roolBack() {
        for($i = 0; $i < $this->execution_pointer + 1; $i++) {
            switch($this->operations[$i][0]) {
                case FileSystem::RENAME:{
                    rename($this->operations[$i][2], $this->operations[$i][1]);
                } break;

                case FileSystem::MOVE:{
                    rename($this->operations[$i][2], $this->operations[$i][1]);
                } break;

                case FileSystem::COPY:{
                    unlink($this->operations[$i][2]);
                } break;

                case FileSystem::MOVE_UPLOAD:{
                    unlink($this->operations[$i][2]);
                } break;
            }
        }
    }
    public function commit() : bool {
        $this->checkFileExists();
        $flag = true;
        for($this->execution_pointer = 0; $flag && $this->execution_pointer < count($this->operations); $this->execution_pointer++) {
            switch($this->operations[$this->execution_pointer][0]) {
                case FileSystem::RENAME:{
                    if(!rename($this->operations[$this->execution_pointer][1],
                        $this->operations[$this->execution_pointer][2])) $flag = false;
                } break;

                case FileSystem::MOVE:{
                    if(!rename($this->operations[$this->execution_pointer][1],
                        $this->operations[$this->execution_pointer][2])) $flag = false;
                } break;

                case FileSystem::COPY:{
                    if(!copy($this->operations[$this->execution_pointer][1],
                        $this->operations[$this->execution_pointer][2])) $flag = false;
                } break;

                case FileSystem::DELETE: {
                    if(file_exists($this->operations[$this->execution_pointer][1])) {
                        if( unlink($this->operations[$this->execution_pointer][1])) $flag = false;
                    }
                } break;

                case FileSystem::MOVE_UPLOAD : {
                    if(!move_uploaded_file($this->operations[$this->execution_pointer][1],
                        $this->operations[$this->execution_pointer][2])) $flag = false;
                } break;
            }
        }
        if(!$flag) {
            $this->roolBack();
            return false;
        }
        return true;
    }
    private final function checkFileExists() {
        foreach($this->operations as $operation) {
            if(!file_exists($operation[1])) throw new Exception("File does not exists");
        }
    }
}
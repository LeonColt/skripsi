<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 18/01/2017
 * Time: 17:09
 */
class FileHandler
{
    private $path, $file_handler;
    private $buffer_size;
    private $pointer, $reversed;
    public final function __construct( string $path, string $mode )
    {
        $this->path = $path;
        $this->file_handler = fopen($path, $mode);
        if($this->file_handler === FALSE) throw new Exception("File Does Not Exist");
    }

    public function getFileHandler(){return $this->file_handler;}

    public final function begin(int $buffer_size) {
        rewind($this->file_handler);
        if(feof($this->file_handler)) return FALSE;
        $this->buffer_size = $buffer_size;
        return fread($this->file_handler, $buffer_size);
    }
    public final function end() {return FALSE;}

    public final function next(&$iterator) {
        if(feof($this->file_handler)) $iterator = FALSE;
        $iterator = fread($this->file_handler, $this->buffer_size);
    }

    public function rbegin(int $buffer_size) {

    }
    public function rend() {return FALSE;}
    public function rnext() {

    }
    public function beginLine() {
        rewind($this->file_handler);
        return fgets($this->file_handler);
    }
    public function endLine() {return FALSE;}
    public function nextLine(&$iterator) {$iterator = fgets($this->file_handler);}
    public function rbeginLine(int $begin_index = 0, bool $reversed = false) {
        $this->reversed = $reversed;
        $line = "";
        for($this->pointer = $begin_index, $c = -1; fseek($this->file_handler, $this->pointer, SEEK_END) !== -1; $this->pointer--) {
            $before_c = $c;
            $c = fgetc($this->file_handler);
            if(ord($c) === 13) //\n
            {
                if(ord($before_c) === 10) {
                    if($reversed) return strrev($line); else return $line;
                }
                else $line.= $c;
            }
            else if(ord($c) !== 10 && ord($c) > -1 && $c !== -1 ) $line.= $c;
        }
        return FALSE;
    }
    public function rendLine() {return FALSE;}
    public function rnextLine(&$iterator) {
        $line = "";
        for($c = -1; fseek($this->file_handler, $this->pointer, SEEK_END) !== -1; $this->pointer--) {
            $before_character = $c;
            $c = fgetc($this->file_handler);
            if(ord($c) === 13) //\n
            {
                if(ord($before_character) === 10) {
                    if($this->reversed) {
                        $iterator = strrev($line);
                        return;
                    }
                    else {
                        $iterator =  $line;
                        return;
                    }
                }
                else $line.= $c;
            }
            else if(ord($c) !== 10 && ord($c) > -1 && $c !== -1 ) $line.= $c;
        }
        $iterator = FALSE;
    }
    public function pushLine(string $data) {
        fwrite($this->file_handler, ($data.PHP_EOL));
    }
    public function push(string $data) {
        fwrite($this->file_handler, $data);
    }
    public final function delete() {
        unlink($this->path);
    }
    public final function __destruct()
    {
        // TODO: Implement __destruct() method.
        if( $this->file_handler !== FALSE ) fclose($this->file_handler);
    }
}
<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 23/04/2016
 * Time: 20:54
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
class FileDownload
{
    private $key, $file_path, $file_name, $file_size;
    private $begin, $end;
    private $buffer_length =2048;
    public function __construct(string $key, string $file_name, int $begin, int $end)
    {
        $this->key = $key;
        $this->file_name = $file_name;
        $this->file_path = standart_path.'daten/'.((int)$key);
        $this->file_size = filesize($this->file_path);
        $this->delay = 0;
        $this->begin = $begin;
        $this->end = ($end !== -1) ? $end : $this->file_size - 1;
        //$this->end = $end;
    }
    public function execute()
    {
        $file_handler = fopen($this->file_path, 'rb');
        $length = $this->begin - $this->end + 1;
        set_time_limit(0);
        //header("Accept-Ranges: bytes");
        //header('Content-Length:'.$length);
        //header('Content-Range: bytes '.$this->start.'-'.$this->end.'/'.$this->file_size);
        //header("Content-Type: application/octet-stream");
        //header("Content-Transfer-Encoding: binary");
        //header(sprintf('Content-Disposition: attachment; filename="%s"', ($this->file_name.'.'.$this->file_format)));
        fseek($file_handler, $this->begin);
        ob_clean();
        while(!feof($file_handler))
        {
            print fread($file_handler, $this->buffer_length);
            ob_flush();
            flush();
            $this->begin += $this->buffer_length;
            if($this->begin == $this->end) break;
        }
        fclose($file_handler);
    }
}
<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 30/03/2016
 * Time: 15:41
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
class Returner
{
    protected $status, $message;
    public function __construct()
    {
        $this->status = array();
        $this->message = array();
    }

    public function is_there_any_error() : bool
    {
        return (count($this->status) > 0);
    }
    public function get_messages() : array
    {
        return $this->message;
    }
    public function getMessageinString(string $seperator) : string
    {
        return implode($seperator, $this->message);
    }
}
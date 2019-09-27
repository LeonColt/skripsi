<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 17/03/2016
 * Time: 18:52
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
class ConfirmAppKey
{
    private $type;
    public function __construct($type)
    {
        $this->type = $type;
    }
    public function execute($key) {
        switch($this->type) {
            case app_type_winapi: $this->confirmWinapi($key); break;
            default: throw new Exception("key and the product do not match");
        }
    }
    public function confirmWinapi($key) {
        $insert = new Insert('app_list_winapi');
        $insert->append_column_value('app_key', new Parameter($insert->getParameterVariableIntegerOrder(), $key));
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $runner->append_query($insert);
        $runner->execute();
    }
}
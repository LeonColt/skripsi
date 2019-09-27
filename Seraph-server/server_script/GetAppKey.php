<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 05/03/2016
 * Time: 15:32
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
require_once standart_path.'core/Random.php';
require_once standart_path.'core/database/Runner.php';
require_once standart_path.'core/database/Select.php';
class GetAppKey
{
    private $type;
    public function __construct($type)
    {
        $this->type = $type;
    }
    public function execute() : string {
        switch($this->type) {
            case app_type_winapi: return $this->getWinApi();
        }
        return "";
    }
    private function getWinApi() : string {
        $random = new Random();
        $select = new Select('app_list_winapi');
        $select->append_column('app_key');
        $select->append_where("app_key = ?");
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        do
        {
            $select->clear_parameter();
            $key = strtoupper("64win".$random->alphaNumeric(20));
            $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $key));
            $runner->clear_query();
            $runner->append_query($select);
            $runner->execute();
        } while((count($select->result()) > 0));
        return $key;
    }
}
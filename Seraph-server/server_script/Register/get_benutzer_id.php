<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 14/03/2016
 * Time: 13:23
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
include_once standart_path.'core/Random.php';
include_once standart_path.'core/database/Runner.php';
class get_benutzer_id
{
    public function execute() : string
    {
        $select = new Select('benutzer');
        $select->append_column('id');
        $select->append_where('id = ?');
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        do
        {
            $select->clear_parameter();
            $runner->clear_query();
            $random = new Random();
            $id =  $random->random_number_int(1, PHP_INT_MAX);
            $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $id));
            $runner->append_query($select);
            $runner->execute();
        } while(count($select->result()) > 0);
        return $id;
    }
}
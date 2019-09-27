<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 31/01/2017
 * Time: 14:17
 */
class GetUserList
{
    const ID = "id";
    const USERNAME = "username";
    const EMAIL = "email";
    public function execute() : array {
        $temp = array();
        $select = new Select('benutzer');
        $select->append_column('id');
        $select->append_column('username');
        $select->append_column('email');
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $runner->append_query($select);
        $runner->execute();
        $res = $select->result();
        foreach($res as $re) {
            $val = array();
            $val[GetUserList::ID] = $re["id"];
            $val[GetUserList::USERNAME] = $re["username"];
            $val[GetUserList::EMAIL] = $re['email'];
            array_push($temp, $re);
        }
        return $temp;
    }
}
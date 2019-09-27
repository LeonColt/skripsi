<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 31/01/2017
 * Time: 14:25
 */
require_once standart_path.'server_script/file_controller/FilePrivilegeConstant.php';
class GetUserListOnFilePrivilege
{
    const INPUT_FILE_ID = "cf-gudlof-394834839089348324";
    const INPUT_PRIVILEGE_TYPE = "cf-gudlof-3420894028492048920482904";
    const TYPE_SEEN = "cf-gudlof-34949284982942842748274234";
    const TYPE_DOWNLOAD = "cf-gudlof-394829048290348204820482";
    const TYPE_UPLOAD_ON_DIRECTORY = "cf-gudlof-93849028402984028402948390";
    const LOGIN_ID = "cf-gudlof-394834839089348323904230489035495793";

    const ID = "id";
    const USERNAME = "username";
    private $file_id, $type, $benutzer;
    public function __construct(int $file_id, string $type, $login_id)
    {
        $this->file_id = $file_id;
        $this->type = $type;
        $this->benutzer = getUser($login_id);
    }
    public function execute() : array {
        $select = new Select('privilege_benutzer_on_object');
        $select->append_column('id', 'benutzer', 'id');
        $select->append_column('username', 'benutzer', 'username');
        $select->append_join('JOIN', 'benutzer');
        $select->append_on('benutzer', 'id', '=', 'privilege_benutzer_on_object', 'benutzer');
        $select->append_where('privilege_benutzer_on_object.file_id = ?');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->file_id));
        $select->append_where('privilege_benutzer_on_object.privilege_id = ?', 'AND');
        switch( $this->type ) {
            case GetUserListOnFilePrivilege::TYPE_SEEN : {
                $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), FilePrivilegeConstant::SEEN));
            } break;

            case GetUserListOnFilePrivilege::TYPE_DOWNLOAD : {
                $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), FilePrivilegeConstant::DOWNLOAD));
            } break;

            case GetUserListOnFilePrivilege::TYPE_UPLOAD_ON_DIRECTORY : {
                $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), FilePrivilegeConstant::UPLOAD_ON_FILE));
            } break;
        }
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $runner->append_query($select);
        $runner->execute();
        $res = array();
        foreach($select->result() as $item) {
            $temp = array();
            $temp[GetUserListOnFilePrivilege::ID] = $item['id'];
            $temp[GetUserListOnFilePrivilege::USERNAME] = $item["username"];
            array_push($res, $temp);
        }
        return $res;
    }
}
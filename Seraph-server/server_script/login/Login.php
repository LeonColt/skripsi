<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 19/02/2016
 * Time: 13:57
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
require_once standart_path.'core/database/Runner.php';
require_once standart_path.'core/Session/Session_login.php';
require_once standart_path.'core/Random.php';
abstract class Login
{
    const RETURN_LOGIN_ID = "nmlf-l-i-483598472348927492742";
    const RETURN_BENUTZER_ID = "nmlf-l-i-0485-0495-03854583058";
    const RETURN_BENUTZERNAME = "nmlf-l-i-9340938492849028490284902485738924";
    const RETURN_EMAIL = "nmlf-l-i-939489204820348203480234";
    const RETURN_REQUEST = "nmlf-l-i-483908402742830478234728";
    const RETURN_GROUP_PRIVILEGE = "nmlf-l-i-3847928749284792472398";
    protected $id_username_email, $password, $password_admin, $admin;
    public function __construct($id_username_email, $password, $admin = false)
    {
        $this->id_username_email = $id_username_email;
        $this->password = $password;
        $this->admin = $admin;
    }

    public function execute() : array
    {
        if($this->admin) return $this->login_as_admin();
        else return $this->login();
    }

    public abstract function login() : array;
    public abstract function login_as_admin() : array;
    protected final function extractBenutzer(Runner &$runner, &$password) : User {
        $select = new Select('benutzer');
        $select->append_column('id', 'benutzer');
        $select->append_column('username', 'benutzer');
        $select->append_column('email', 'benutzer');
        $select->append_column('pass', 'benutzer');
        $select->append_column('privilege_group_id', 'privilege_group', 'privilege_group_id');
        $select->append_column('group_name', 'privilege_group', 'privilege_group_name');
        $select->append_join('LEFT JOIN', 'privilege_group_member');
        $select->append_on('privilege_group_member', 'benutzer', '=', 'benutzer', 'id');
        $select->append_join('LEFT JOIN', 'privilege_group');
        $select->append_on('privilege_group_member', 'group_id', '=', 'privilege_group', 'privilege_group_id');
        $select->append_where('benutzer.id = ?');
        $select->append_where('benutzer.username = ?', 'OR');
        $select->append_where('benutzer.email = ?', 'OR');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->id_username_email));
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->id_username_email));
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->id_username_email));

        $runner->clear_query();
        $runner->append_query($select);
        $runner->execute();

        $runner->clear_query();

        if(count($select->result()) < 1 ) throw new Exception("User Does not exists");

        $user = new User($select->result()[0][0], $select->result()[0][1], $select->result()[0][2], $select->result()[0][4], false);
        $password = $select->result()[0][3];
        return $user;
    }
}
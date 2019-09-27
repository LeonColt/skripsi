<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 22/01/2017
 * Time: 13:43
 */
class Confirmation
{
    const SECURITY_TOKEN = "cf-c-st-3482934829342942789274";
    const SECRET_KEY = "cf-c-sk-394829347283947294789472894";
    private $id, $username, $email, $password, $admin;
    public function __construct(string $security_token, string $secret_key)
    {
        $this->map = md5($security_token);
        try {
            $fh = new FileHandler(REGISTRATION_TEMPORARY_PATH."/".$this->map, "r");
            $data = "";
            for($i = $fh->begin(4096); $i != $fh->end(); $fh->next($i)) $data .= $i;
            $data = json_decode($data, true);
        } catch (Exception $e) {
            throw new Exception("Registration Not Found");
        }
        $this->id = $data["id"];
        $this->username = $data["username"];
        $this->email = $data["email"];
        $this->password = $data["password"];
        if( !password_verify($secret_key, $data["secret_key"])) throw new Exception("Please Input Correct Secret Key");
        $this->admin = $data["admin"];
    }
    public function register()
    {
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $key = "user_Reg";
        $insert = new Insert('benutzer');
        $insert->append_column_value('id', new Parameter($insert->getParameterVariableIntegerOrder(), $this->id));
        $insert->append_column_value('username', new Parameter($insert->getParameterVariableIntegerOrder(), $this->username));
        $insert->append_column_value('email', new Parameter($insert->getParameterVariableIntegerOrder(), $this->email));
        $insert->append_column_value('pass', new Parameter($insert->getParameterVariableIntegerOrder(), $this->password));
        $runner->append_query($insert, $key);

        $insert_group = new Insert('privilege_group_member');
        if($this->admin)
            $insert_group->append_column_value('group_id', new Parameter($insert_group->getParameterVariableIntegerOrder(), 2));
        else $insert_group->append_column_value('group_id', new Parameter($insert_group->getParameterVariableIntegerOrder(), 3));
        $insert_group->append_column_value('benutzer', new Parameter($insert_group->getParameterVariableIntegerOrder(), $this->id));

        $runner->append_query($insert_group, $key);


        $runner->execute();
    }
    public function register_admin()
    {
        throw new Exception("Not Allowed");
    }
    public function execute()
    {
        $this->checkEmpty();
        if($this->admin) $this->register_admin(); else $this->register();
    }
    private final function checkEmpty() {
        if(strcmp($this->id, "") === 0) throw new Exception("Id cannot be empty");
        if(strcmp($this->username, "") === 0) throw new Exception("username cannot be empty");
        if(strcmp($this->email, "") === 0) throw new Exception("Email cannot be empty");
        if(strcmp($this->password, "") === 0) throw new Exception("Password cannot be empty");
    }
}
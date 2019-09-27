<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 21/02/2016
 * Time: 13:34
 */
require_once standart_path.'server_script/login/Login.php';
require_once standart_path."server_script/login/LoginWinapi.php";
require_once standart_path.'core/Session/Session_login.php';
require_once standart_path.'core/Session/Request.php';
class LoginJSON
{
    const ID_USER_USERNAME_EMAIL = "nmlf-l-8593";
    const PASSWORD = "nmlf-l-8888";
    private $app_type, $id_username_email, $password;
    public function __construct(string $app_type, string $id_username_email, string $password)
    {
        $this->app_type = $app_type;
        $this->id_username_email = $id_username_email;
        $this->password = $password;
    }

    public function execute() : array
    {
        switch($this->app_type)
        {
            case app_type_winapi: $login = new LoginWinapi($this->id_username_email, $this->password); break;
            default: $login = null;
        }
        if($login == null) throw new Exception("app type is not found");
        return $login->execute();
    }
}
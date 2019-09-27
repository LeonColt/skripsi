<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 09/03/2016
 * Time: 19:03
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
class Register
{
    const SECURITY_TOKEN = "st";
    private $id, $username, $email, $password, $admin;
    public function __construct(string $id = null, string $username = null, string $email = null, string $password = null, bool $admin = false)
    {
        $this->id = $id;
        $this->username = $username;
        $this->email = $email;
        $this->password = $password;
        $this->admin = $admin;
    }
    public function register(): string
    {
        $security_token = getSecurityToken();
        $random = new Random();
        //$code = $random->alphaNumeric(5);
        $code = "12345";
        $fh = new FileHandler(REGISTRATION_TEMPORARY_PATH.'/'.md5($security_token), 'wb');
        $fh->push(json_encode(array(
            "id"=>$this->id,
            "username"=>$this->username,
            "email"=>$this->email,
            "password"=>password_hash($this->password, PASSWORD_DEFAULT),
            "secret_key"=>password_hash($code, PASSWORD_DEFAULT),
            "admin"=>false
        )));
        $message = "Your Confirmation Code : ".$code;
        $headers = "MIME-Version: 1.0\r\n";
        $headers .= "Content-type: text/html; charset=iso-8859-1\r\n";
        $headers .= "To: ".$this->username." <".$this->email.">\r\n";
        $headers .= "From: SeraphAdmin <".EMAIL_ADMIN_NO_REPLY.">\r\n";
        mail($this->email, "Registration Confirmation", $message, $headers);
        return $security_token;
    }
    public function register_admin() : string
    {
        throw new Exception("Not Allowed");
    }
    public function execute() : string
    {
        $this->checkEmpty();
        if($this->admin)
            return $this->register_admin();
        else return $this->register();
    }
    private final function checkEmpty() {
        if(strcmp($this->id, "") === 0) throw new Exception("Id cannot be empty");
        if(strcmp($this->username, "") === 0) throw new Exception("username cannot be empty");
        if(strcmp($this->email, "") === 0) throw new Exception("Email cannot be empty");
        if(strcmp($this->password, "") === 0) throw new Exception("Password cannot be empty");
    }
}
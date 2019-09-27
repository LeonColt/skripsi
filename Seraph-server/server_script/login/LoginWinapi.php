<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 09/03/2016
 * Time: 12:50
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
require_once standart_path."server_script/login/Login.php";
require_once standart_path.'core/Session/Session.php';
require_once standart_path.'core/Session/Request.php';
require_once standart_path.'core/Random.php';
require_once standart_path.'core/Session/AkeySession.php';
class LoginWinapi extends Login
{
    public function login() : array
    {
        // TODO: Implement login() method.
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $user = $this->extractBenutzer($runner, $db_pass);
        if(password_verify($this->password, $db_pass) === true)
        {
            $random = new Random();
            $login_id = $random->alphaNumeric(28);

            $this->successLog($runner, $login_id, $user->getId());


            $session = new Session();
            $session_login = new Session_login($session, $login_id, $user, new Request());
            $session->startSession();
            $session->addSessionByKey(serialize($session_login), session_login);
            if(getUser($login_id) == null) throw new Exception("unable to store login data");

            $temp = array();
            $temp[Login::RETURN_LOGIN_ID] = $login_id;
            $temp[Login::RETURN_BENUTZER_ID] = (string)$user->getId();
            $temp[Login::RETURN_BENUTZERNAME] = $user->getUsername();
            $temp[Login::RETURN_EMAIL] = $user->getEmail();
            $temp[Login::RETURN_REQUEST] = $session_login->get_requests()->extractRequest();
            $temp[Login::RETURN_GROUP_PRIVILEGE] = $user->getGroupPrivilege();
            return $temp;
        }
        else {
            $this->failureLog($runner, $user->getId());
            throw new Exception("Wrong Password");
        }
    }

    public function login_as_admin() : array
    {
        // TODO: Implement login_as_admin() method.
        throw new Exception("Not Implemented Yet");
    }
    private final function successLog(Runner &$runner, $login_id, $benutzer_id){
        $insert = new Insert('login_log_winapi');
        $insert->append_column_value('login_id', new Parameter($insert->getParameterVariableIntegerOrder(), $login_id));
        $insert->append_column_value('benutzer', new Parameter($insert->getParameterVariableIntegerOrder(), $benutzer_id));
        $session = new Session();
        $session->startSession();
        $akey = unserialize($session->getSessionByKey(session_app_key));
        $insert->append_column_value('app', new Parameter($insert->getParameterVariableIntegerOrder(), $akey->getId()));
        $runner->clear_query();
        $runner->append_query($insert);
        $runner->execute();
    }
    private final function failureLog(Runner &$runner, $benutzer_id) {
        $insert = new Insert('login_log_winapi_fail');
        $insert->append_column_value('benutzer', new Parameter($insert->getParameterVariableIntegerOrder(), $benutzer_id));
        $session = new Session();
        $session->startSession();
        $akey = unserialize($session->getSessionByKey(session_app_key));
        $insert->append_column_value('app', new Parameter($insert->getParameterVariableIntegerOrder(), $akey->getId()));
        $runner->clear_query();
        $runner->append_query($insert);
        $runner->execute();
    }
}
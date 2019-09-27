<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 27/02/2016
 * Time: 17:46
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
require_once standart_path.'core/global_variable.php';
require_once standart_path.'core/session_starter.php';
require_once standart_path.'core/database/Insert.php';
require_once standart_path.'core/database/Runner.php';
require_once standart_path.'core/Session/Session.php';
require_once standart_path.'core/Session/Session_login.php';
class Logout
{
    public function execute() : bool
    {
        $session = new Session();
        if(!$session->isStarted()) $session->startSession();
        $session_login = $session->get_benutzer_credibility();
        if($session_login !== null)
        {
            $insert = new Insert('logout_log_winapi');
            $insert->append_column_value('Login_id', $session_login->get_login_id());
            $insert->append_column_value('waktu', time());
            $runner = new Runner();
            $runner->append_query($insert);
            $runner->execute();
        }
        session_unset();
        return session_destroy();
    }
}
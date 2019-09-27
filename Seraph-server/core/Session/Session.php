<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 27/02/2016
 * Time: 17:57
 */
require_once standart_path.'core/global_variable.php';
require_once standart_path.'core/Session/Session_login.php';
ini_set('session.cookie_httponly', 0);
ini_set('session.entropy_file', '/dev/urandom');
ini_set('session.hash_function', 'whirlpool');
ini_set("session.use_cookies", 0);
ini_set('session.use_only_cookie', 0);
ini_set('session.use_trans_sid', 1);
//ini_set('session.cookie_secure', 1);
class Session
{
    public function __construct()
    {
    }

    public function startSession() : bool
    {
        if(session_status() === PHP_SESSION_NONE || session_status() === PHP_SESSION_DISABLED)
        {
            //session_name(name_of_session);
            //session_regenerate_id(true);
            if(session_start(['use_only_cookies'=>0, 'use_trans_sid'=>1]))
            {
                //session_regenerate_id(true);
                return true;
            }
        }
        return false;
    }
    public function isStarted()
    {
         return session_status() === PHP_SESSION_ACTIVE;
    }
    public function addSession($object)
    {
        if($this->isStarted())
        {
            array_push($_SESSION, $object);
            session_write_close();
        }
        else throw new Exception("Session has not been started, please call startSession method before add any object");
    }
    public function getSession(int $index)
    {
        if($this->isStarted())
        {
            if(array_key_exists($index, $_SESSION))
            {
                return $_SESSION[$index];
            }
            else throw new Exception("index does not exist");

        }
        else throw new Exception("no object can be retrieve if session has not been started");
    }
    public function addSessionByKey($object, string $key)
    {
        if($this->isStarted())
        {
            $_SESSION[$key] = $object;
            session_write_close();
        }
        else throw new Exception("Session has not been started, please call startSession method before add any object");
    }
    public function isContain(int $index)
    {
        return array_key_exists($index, $_SESSION);
    }
    public function isContainByKey(string $key)
    {
        return array_key_exists($key, $_SESSION);
    }
    public function getSessionByKey(string $key)
    {
        if($this->isStarted())
        {
            if(array_key_exists($key, $_SESSION))
            {
                return $_SESSION[$key];
            }
            else throw new Exception("index does not exist");
        }
        else throw new Exception("no object can be retrieve if session has not been started");
    }
    public function __destruct()
    {
        // TODO: Implement __destruct() method.
        session_write_close();
    }
}
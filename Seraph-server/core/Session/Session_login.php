<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 27/02/2016
 * Time: 18:57
 */
require_once standart_path.'core/global_variable.php';
require_once standart_path.'core/Session/Request.php';
require_once standart_path.'core/User.php';
class Session_login
{
    private $login_id, $expire_time, $requests;
    private $session_parent;
    private $user;
    public function __construct(Session $session_parent, string $login_id, User $user, Request $requests)
    {
        $this->login_id = $login_id;
        $this->user = $user;
        $xml = simplexml_load_file(xml_session);
        $this->expire_time = ((int)$xml->session_login_time_additional === -1) ? ((int)$xml->session_login_time_additional) : (time() + ((int)$xml->session_login_time_additional));
        $this->requests = $requests;
        $this->session_parent = $session_parent;
    }
    public function get_login_id() : string{return $this->login_id;}
    public function getUser() : User {return $this->user;}
    public function get_expire_time() : int{return $this->expire_time;}
    public function isExpired() : bool
    {
        return ($this->expire_time !== -1) ? ((time() > $this->expire_time)) : (false);
    }
    public function get_requests() : Request
    {
        return $this->requests;
    }
    public function isSessionCredebilityCorrect($login_id = null, $user_id = null, $user_email = null) : bool
    {
        if($this->session_parent->isStarted())
        {
            if( $this->login_id === null || strcmp($this->login_id, "" ) === 0) return false;
            if($this->getUser()->getId() === 0) return false;
            if( $this->getUser()->getUsername() === null || strcmp($this->getUser()->getUsername(), "") === 0) return false;
            if( $this->isExpired() ) return false;
            if( $login_id !== null) {
                if( strcmp($login_id, $this->login_id) !== 0 ) return false;
            }
            if( $user_id !== null ) {
                if( strcmp($user_id, $this->getUser()->getId()) !== 0 ) return false;
            }
            if( $user_email !== null ) {
                if( strcmp($user_email, $this->getUser()->getEmail()) !== 0 ) return false;
            }
            return true;
        }
        else return false;
    }
}
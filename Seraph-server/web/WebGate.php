<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 22/01/2017
 * Time: 15:02
 */
require_once 'core/Request.php';
class WebGate
{
    public function execute() {
        switch(filter_input(INPUT_GET, var_request)) {
            case Request::HOME : {
                $this->requestHome();
            } break;

            case Request::LOGIN_PAGE : {
                $this->requestLoginPage();
            } break;

            case Request::REGISTRATION_CONFIRMATION_PAGE : {
                $this->requestRegistrationConfirmationPage();
            } break;

            default : {
                $this->requestHome();
            }
        }
    }

    private function requestHome() {
        require_once 'Home.php';
        $exec = new Home();
        $exec->show();
    }
    private function requestRegistrationConfirmationPage() {
        require_once 'registration/ConfirmationPage.php';
        $exec = new ConfirmationPage();
        $exec->show();
    }
    private function requestLoginPage() {
        require_once 'loginlogout/LoginPage.php';
        $exec = new LoginPage();
        $exec->show();
    }
}
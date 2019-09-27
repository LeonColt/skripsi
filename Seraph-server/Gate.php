<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 19/02/2016
 * Time: 15:56
 */
require_once 'core/global_variable.php';
class Gate
{
    public function execute()
    {
        $headers = getallheaders();
error_log(getallheaders());
        if(!isset($headers[var_request_type]) || empty($headers[var_request_type])) {
            require_once 'web/WebGate.php';
            $exec = new WebGate();
            $exec->execute();
            exit(0);
        }
        if(!isset($headers[var_request]) || empty($headers[var_request])) {
            header("HTTP/1.0 404 Not Found - command not found", true, 404);
            //require standart_path."404.html";
            exit(0);
        }
        switch($headers[var_request_type]) {
            case request_type_json:
            {
                if(!isset($headers[var_app_type]) || empty($headers[var_app_type]) )
                    header("HTTP/1.0 404 Not Found - command not found", true, 404);
                require_once standart_path."server_script/JSON/JSON_Runner.php";
                $exec = new JSON_Runner($headers[var_app_type]);
                $exec->execute($headers[var_request]);

            } break;

            default:
            {
                header("HTTP/1.0 404 Not Found - command not found", true, 404);
            } break;
        }
    }
}
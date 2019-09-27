<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 09/03/2016
 * Time: 19:49
 */
require_once standart_path.'server_script/Register/Register.php';
require_once standart_path.'server_script/JSON/json_encoder.php';
class Register_json
{
    public function execute()
    {
        $encoder = new json_encoder();
        $exec = new Register(filter_input(INPUT_POST, var_id),
            filter_input(INPUT_POST, var_username),
            filter_input(INPUT_POST, var_email),
            filter_input(INPUT_POST, var_password));
        $result = $exec->execute();
        if(!$result->is_there_any_error())
        {
            $encoder->status_berhasil();
            $encoder->set_message("registration is success");
        }
        else
        {
            $encoder->status_gagal();
            $encoder->set_message(implode(" \n ", $result->get_messages()));
        }
        $encoder->show();
    }
}
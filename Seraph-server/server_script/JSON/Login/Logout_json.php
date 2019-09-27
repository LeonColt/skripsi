<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 04/03/2016
 * Time: 18:51
 */
require_once standart_path.'server_script/login/Logout.php';
class Logout_json
{
    public function execute()
    {
        $logout = new Logout();
        $encoder = new json_encoder();
        if($logout->execute())
        {
            $encoder->status_berhasil();
            $encoder->set_message("Logout is success");
        }
        else
        {
            $encoder->status_gagal();
            $encoder->set_message("Logout is not recorded");
        }
        $encoder->show();
    }
}
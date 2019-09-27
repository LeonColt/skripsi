<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 14/03/2016
 * Time: 13:24
 */
include_once standart_path.'server_script/Register/get_benutzer_id.php';
class get_betnutzer_id_json
{
    public function execute()
    {
        $encoder = new json_encoder();
        $id = new get_benutzer_id();
        $id = $id->execute();
        $encoder->status_berhasil();
        $encoder->set_result($id);
        $encoder->show();
    }
}
<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 25/02/2016
 * Time: 03:52
 */
class JSON_req_gate
{
    private $json;
    public function __construct()
    {
        $this->json = array();
    }

    public function execute()
    {
    }

    public function print_result()
    {
        echo json_encode($this->json);
    }
}
<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 19/02/2016
 * Time: 15:56
 */
//error_reporting(E_ERROR | E_WARNING | E_PARSE | E_NOTICE);
//error_reporting(E_ERROR | E_PARSE | E_NOTICE);
require_once 'core/global_variable.php';
require_once 'Gate.php';
class Index
{
    public function execute()
    {
        $gate = new Gate();
        $gate->execute();
    }
}
$index = new Index();
$index->execute();
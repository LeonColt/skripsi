<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 03/07/2016
 * Time: 16:15
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
class AkeySession
{
    private $id, $akey, $owner_id;
    public function __construct(string $id, string $akey, $owner_id)
    {
        $this->id = $id;
        $this->akey = $akey;
        $this->owner_id = $owner_id;
    }
    public function getId() : string
    {
        return $this->id;
    }
    public function getAkey() : string
    {
        return $this->akey;
    }
    public function getOwnerId() : string
    {
        return $this->owner_id;
    }
    public function isOwned() : bool
    {
        return $this->owner_id != null;
    }
}
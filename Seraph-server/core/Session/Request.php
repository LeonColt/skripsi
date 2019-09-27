<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 27/02/2016
 * Time: 19:03
 */
include_once standart_path."core/Random.php";
class Request
{
    const REQUEST_GET_FILES = "nmlf-rec-req-9388509345843905893753985";
    const REQUEST_DOWNLOAD_FILE = "nmlf-rec-req-3859375934737530948284";
    private $request_get_files;
    private $reqeust_download_files;
    public function __construct()
    {
        $random = new Random();
        do
        {
            $this->request_get_files = $random->alphaNumeric(28);
            $this->reqeust_download_files = $random->alphaNumeric(28);
        } while(strcasecmp($this->request_get_files, $this->reqeust_download_files) == 0);
    }

    public function getRequestGetFiles() : string
    {
        return $this->request_get_files;
    }
    public function getRequestDownloadFiles() : string
    {
        return $this->reqeust_download_files;
    }
    public function extractRequest() : array {
        $temp = array();
        $temp[Request::REQUEST_GET_FILES] = $this->getRequestGetFiles();
        $temp[Request::REQUEST_DOWNLOAD_FILE] = $this->getRequestDownloadFiles();
        return $temp;
    }
}
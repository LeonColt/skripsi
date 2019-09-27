<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 04/03/2016
 * Time: 18:56
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
class Random
{
    public function random_number(int $length) : string
    {
        $str = "";
        for($i = 0; $i < $length; $i++)
        {
            $str .= random_int(0, 9);
        }
        return $str;
    }
    public function random_number_int(int $min = PHP_INT_MIN, int $max = PHP_INT_MAX) : int
    {
        return random_int($min, $max);
    }
    public function alphaNumeric(int $length)
    {
        $str = "";
        for($i = 0; $i < $length; $i++)
        {
            $res = random_int(0, 35);
            if($res < 10) $str .= $res;
            else $str .= chr(($res+55));
        }
        return $str;
    }
    public function random_hexadecimal(int $length) : string
    {
        $hlength = ceil(($length/2));
        $str = true;
        $str = bin2hex(openssl_random_pseudo_bytes($hlength, $str));
        while(strlen($str) > $length)
        {
            $str = substr($str, 0, strlen($str)-1);
        }
        return $str;
    }
}
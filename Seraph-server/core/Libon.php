<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 03/04/2016
 * Time: 11:30
 */
class Libon
{
    private $kontainer;
    public function __construct()
    {
        $this->kontainer = array();
    }
    public function push($value)
    {
        array_push($this->kontainer, $value);
    }
    public function encode($var)
    {
        echo "&A";
        for($i = 0; $i < count($var); $i++)
        {
            if(is_array($var[$i]))
            {
                $this->encode($var[$i]);
            }
            else if(is_bool($var[$i])) echo ($var[$i]) ? "true," : "false,";
            else if(is_string($var[$i]))
            {
                echo "&S";
                echo $var[$i];
                echo "&E,";
            }
            else if(is_null($var[$i])) echo "null,";
        }
        echo "&E";
    }
    public function show()
    {
        $this->encode($this->kontainer);
    }
}
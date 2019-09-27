<?php

/**
 * Created by PhpStorm.
 * User: LeonColt
 * Date: 11/29/2016
 * Time: 11:11 PM
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
class ParameterRef
{
    private $var, $val, $type, $length;
    public function __construct($variable, $value, int $type = null, int $length = null)
    {
        $this->var = $variable;
        $this->val = $value;
        $this->type = $type;
        $this->length = $length;
    }
    public function setVariable($variable){$this->var = $variable;}
    public function setValue(&$value){$this->val =& $value;}
    public function setType(int $type){$this->type = $type;}
    public function setLength(int $length) {$this->length = $length;}
    public function getVariable() {return $this->var;}
    public function &getValue() {return $this->val;}
    public function getType() {return $this->type;}
    public function getLength() {return $this->length;}
}
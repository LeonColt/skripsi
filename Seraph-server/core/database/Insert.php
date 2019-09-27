<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 19/01/2016
 * Time: 18:29
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
class Insert
{
    private $main_table, $columns, $update_on_duplicate, $primary_key;
    private $query;
    private $parameter;
    private $result;
    public function __construct($table, $update_on_duplicate = false)
    {
        $this->main_table = $table;
        $this->columns = array();
        $this->parameter = array();
        $this->update_on_duplicate = $update_on_duplicate;
        $this->primary_key = array();
    }
    public function set_table($table)
    {
        $this->main_table = $table;
    }
    public function clear_parameter()
    {
        $this->parameter = array();
    }
    public function clear_column()
    {
        $this->columns = array();
    }
    public function append_column_value($column, Parameter $value)
    {
        array_push($this->columns, $column);
        array_push($this->parameter, $value);
    }
    public function append_column($column)
    {
        array_push($this->columns, $column);
    }
    public function append_value($value)
    {
        array_push($this->parameter, $value);
    }
    public function append_primary_key($column)
    {
        array_push($this->primary_key, $column);
    }
    public function append_parameter(Parameter $value)
    {
        array_push($this->parameter, $value);
    }
    public function parameter() : array
    {
        return $this->parameter;
    }
    public function query($query = null, $old = true)
    {
        if($query === null)
        {
            $this->process_query();
            return $this->query;
        }
        else
        {
            if($old)
            {
                $this->process_query();
                $temp = $this->query;
                $this->query = $query;
                return $temp;
            }
            else
            {
                $this->query = $query;
                return $query;
            }
        }
    }
    private function process_query()
    {
        $this->query = "INSERT INTO ".$this->main_table;
        if(count($this->columns) > 0)
        {
            if(($number_of_columns = $this->searchSameColumnValue() ) > 1) {
                $this->query .= '(';
                for($i = 0; $i < $number_of_columns; $i++) {
                    if($i == 0) $this->query .= $this->columns[$i];
                    else $this->query .= ', '.$this->columns[$i];
                }
                $this->query .= ')';
                $this->query .= ' VALUES ';

                $n_colums = 0;
                for ($i = 0; $i < count($this->parameter); $i++) {
                    if($n_colums == 0) {
                        if($i == 0) $this->query .= '( ?';
                        else $this->query .= ', ( ?';
                    }
                    else if($n_colums == $number_of_columns - 1){
                        $this->query .= ', ? )';
                        $n_colums = -1;
                    }
                    else $this->query .= ', ?';
                    $n_colums++;
                }
            }
            else {
                $this->query .= '(';
                for($i = 0; $i<count($this->columns); $i++)
                {
                    $this->query .= $this->columns[$i];
                    if($i < count($this->columns) - 1) $this->query .= ', ';
                }
                $this->query .= ') ';
                $this->query .= 'VALUES ';
                $this->query .= '(';
                $temp = array();
                for($i = 0; $i < count($this->parameter); $i++)
                {
                    if($this->parameter[$i] instanceof Select)
                    {
                        $this->query .= " (";
                        $this->query .= $this->parameter[$i]->query();
                        $this->query .= ") ";
                        for($j = 0; $j < count($this->parameter[$i]->parameter()); $j++)
                            array_push($temp, $this->parameter[$i]->parameter()[$j]);
                    }
                    else
                    {
                        $this->query .= '?';
                        array_push($temp, $this->parameter[$i]);
                    }
                    if($i < count($this->parameter) - 1) $this->query .= ', ';
                }
                $this->parameter = $temp;
                $this->query .= ')';
            }
        }
        if($this->update_on_duplicate)
        {
            $this->query .= ' ON DUPLICATE KEY UPDATE ';
            for($i = 0; $i < count($this->columns); $i++)
            {
                $is_primary_key = false;
                for($j = 0; $j < count($this->primary_key); $j++)
                {
                    if(strcmp($this->columns[$i], $this->primary_key[$j]) === 0)
                    {
                        $is_primary_key = true; break;
                    }
                }
                if(!$is_primary_key)
                {
                    $this->query .= $this->columns[$i].' = VALUES(';
                    $this->query .= $this->columns[$i].')';
                    if($i < count($this->columns) - 1)
                    {
                        $this->query .= ',';
                    }
                }
            }
            $this->query .= ';';
        }
        else $this->query .= ';';
    }
    private function searchSameColumnValue() : int {
        if(count($this->columns) < 1) return 0;
        $founded_columns = array();
        foreach($this->columns as $column) {
            if(count($founded_columns) == 0) array_push($founded_columns, array($column, 1));
            else {
                $exists = false;
                $index = 0;
                foreach($founded_columns as $founded_column) {
                    if(strcmp($founded_column[0], $column) == 0){
                        $exists = true; break;
                    }
                    $index++;
                }
                if($exists) $founded_columns[$index][1]++;
                else array_push($founded_columns, array($column, 1));
            }
        }
        $res = 0;
        $stabil = true;
        $first = true;
        foreach($founded_columns as $founded_column){
            if($first) {
                $res = $founded_column[1];
                $first = false;
            }
            else {
                if($res != $founded_column[1]) {
                    $stabil = false;
                    break;
                }
            }
        }
        if($stabil ) return count($founded_columns);
        else return 0;
    }
    public final function setResult(bool $result) {
        $this->result = $result;
    }
    public final function getResult() : bool {
        return $this->result;
    }
    public function getParameterVariableIntegerOrder() {
        return count($this->parameter()) + 1;
    }
}
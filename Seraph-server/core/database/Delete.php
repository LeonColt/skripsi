<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 19/01/2016
 * Time: 19:09
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
class Delete
{
    private $main_table;
    private $tables, $joins, $wheres, $order_by, $limit, $offset;
    private $query;
    private $parameter;
    private $result;
    public function __construct($table)
    {
        $this->main_table = $table;
        $this->tables = array();
        $this->joins = array();
        $this->wheres = array();
        $this->order_by = array();
        $this->parameter = array();
        $this->limit = null;
        $this->offset = null;
    }
    public function clear_table()
    {
        $this->tables = array();
    }
    public function clear_join()
    {
        $this->joins = array();
    }
    public function clear_where()
    {
        $this->wheres = array();
    }
    public function clear_order_by()
    {
        $this->order_by = array();
    }
    public function clear_limit()
    {
        $this->limit = null;
    }
    public function clear_offset()
    {
        $this->offset = null;
    }
    public function clear_parameter()
    {
        $this->parameter = array();
    }
    public function append_on($table_left, $left, $operator, $table_right, $right, $seperator_operator = null)
    {
        if($left instanceof Select)
        {
            $temp = $left;
            $left = "(";
            $left .= $temp->query();
            $left .= ") ";
            for($i = 0; $i<count($temp->parameter()); $i++)
                array_push($this->parameter, $temp->parameter()[$i]);
        }
        if($right instanceof Select)
        {
            $temp = $right;
            $right = "(";
            $right .= $temp->query();
            $right .= ") ";
            for($i = 0; $i<count($temp->parameter()); $i++)
                array_push($this->parameter, $temp->parameter()[$i]);
        }
        if($this->joins[count($this->joins)-1][2] === null) $this->joins[count($this->joins)-1][2] = array();
        array_push($this->joins[count($this->joins)-1][2], array(array($left, $table_left), $operator, array($right, $table_right), $seperator_operator));
    }
    public function append_where($expression, $seperator_operator = null)
    {
        array_push($this->wheres, array($expression, $seperator_operator));
    }
    public function append_order_by($column, $asc_desc = ascending)
    {
        array_push($this->order_by, array($column, $asc_desc));
    }
    public function append_table($table)
    {
        if($table instanceof Select)
        {
            $temp = $table;
            $table = "(";
            $table .= $temp->query();
            $table .= ") ";
            for($i = 0; $i<count($temp->parameter()); $i++)
                array_push($this->parameter, $temp->parameter()[$i]);
        }
        array_push($this->tables, $table);
    }
    public function append_join($type, $table)
    {
        if($table instanceof Select)
        {
            $temp = $table;
            $table = "(";
            $table .= $temp->query();
            $table .= ") ";
            for($i = 0; $i<count($temp->parameter()); $i++)
                array_push($this->parameter, $temp->parameter()[$i]);
        }
        array_push($this->joins, array($type, $table, null, null, null, null));
    }
    public function append_parameter($value)
    {
        array_push($this->parameter, $value);
    }
    public function parameter() : array {
        return $this->parameter;
    }
    public function query($query = null, $old = true) : string
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
        $this->query = "DELETE ";
        if(count($this->tables) > 1)
        {
            for($i = 0; $i<count($this->tables); $i++)
            {
                $this->query .= $this->tables[$i];
                if($i < count($this->tables) - 1) $this->query .= ',';
            }
            $this->query .= " FROM ";
            $this->query .= $this->main_table;
            $this->query .=" ";
            foreach($this->joins as $join)
            {
                $this->query .= $join[0]." ".$join[1].' ';
                if(isset($join[2]))
                {
                    $this->query .= "ON ";
                    foreach($join[2] as $on)
                    {
                        if($on[3] !== null) $this->query .= $on[3]." ";
                        $this->query .= $on[0][1].'.'.$on[0][0].' ';
                        $this->query .= $on[1].' ';
                        $this->query .= $on[2][1].'.'.$on[2][0].' ';
                    }
                }
            }
        }
        else
        {
            $this->query .= "FROM ".$this->main_table.' ';
        }
        if(count($this->wheres) > 0)
        {
            $this->query .= "WHERE ";
            foreach($this->wheres as $where)
            {
                if($where[1] !== null) $this->query .= $where[1].' ';
                $this->query .= $where[0].' ';
            }
        }
        if(true)//if(count($this->tables) < 2)
        {
            if(count($this->order_by) > 0)
            {
                $this->query .= 'ORDER BY ';
                for($i = 0; $i < count($this->order_by); $i++)
                {
                    $this->query .= $this->order_by[$i][0].' '.$this->order_by[$i][1].' ';
                    if($i < count($this->order_by) - 1) $this->query .= ',';
                }
            }
        }
        if(true)//if(count($this->tables) < 2)
        {
            if($this->limit !== null) $this->query .= 'LIMIT '.$this->limit.' ';
        }
        if(true)//if(count($this->tables) < 2)
        {
            if($this->offset !== null) $this->query .= 'OFFSET '.$this->offset.' ';
        }
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
<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 19/01/2016
 * Time: 21:40
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
class Update
{
    private $table, $joins, $sets, $wheres, $order_by, $limit, $offset;
    private $query, $parameter;
    private $result;
    public function __construct($table)
    {
        $this->table = $table;
        $this->joins = array();
        $this->sets = array();
        $this->wheres = array();
        $this->order_by = array();
        $this->parameter = array();
        $this->limit = null;
        $this->offset = null;
    }
    public function set_table($table)
    {
        $this->table = $table;
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
    public function append_join($type, $table)
    {
        array_push($this->joins, array($type, $table, null, null, null, null));
    }
    public function append_on($table_left, $left, $operator, $table_right, $right, $seperator_operator = null)
    {
        if($this->joins[count($this->joins)-1][2] === null) $this->joins[count($this->joins)-1][2] = array();
        array_push($this->joins[count($this->joins)-1][2], array(array($left, $table_left), $operator, array($right, $table_right), $seperator_operator));
    }
    public function apppend_set($column, $expression)
    {
        array_push($this->sets, array($column, $expression));
    }
    public function append_where($expression, $seperator_operator = null)
    {
        array_push($this->wheres, array($expression, $seperator_operator));
    }
    public function append_parameter($param)
    {
        array_push($this->parameter, $param);
    }
    public function parameter() : array
    {
        return $this->parameter;
    }
    private function process_query()
    {
        $this->query = "UPDATE ";
        $this->query .= $this->table.' ';
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
        if(count($this->sets) > 0)
        {
            $this->query .= 'SET ';
            for($i = 0; $i<count($this->sets); $i++)
            {
                $this->query .= $this->sets[$i][0].' ';
                $this->query .= '= ';
                $this->query .= $this->sets[$i][1].' ';
                if( $i < count($this->sets) - 1) $this->query .= ",";
            }
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
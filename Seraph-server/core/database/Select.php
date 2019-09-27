<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 19/01/2016
 * Time: 10:34
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
class Select
{
    private $main_table, $columns, $joins, $wheres, $order_by, $group_by, $havings, $distinct, $limit, $offset;
    private $with_rollup;
    private $query;
    private $parameter;
    private $binder;
    private $result;
    private $fetch_mode;
    public function __construct($table)
    {
        $this->main_table = $table;
        $this->columns = array();
        $this->joins = array();
        $this->wheres = array();
        $this->group_by = array();
        $this->havings = array();
        $this->order_by = array();

        $this->distinct = false;
        $this->limit = null;
        $this->offset = null;

        $this->with_rollup = false;

        $this->parameter = array();
        $this->binder = array();

        $this->result = null;
        $this->fetch_mode = -1;
    }
    public function clear_limit(){$this->limit = null;}
    public function clear_offset(){$this->offset = null;}
    public function clear_parameter(){$this->parameter = array();}
    public function clearBinder() {$this->binder = array();}
    public function append_column($column, $table = null, $as = null)
    {
        if($column instanceof Select)
        {
            $temp = $column;
            $column = "(";
            $column .= $temp->query();
            $column .= ")";
            $table = null;
        }
        array_push($this->columns, array($column, $table, $as));
    }
    public function append_column_expression($expression, $as = null)
    {
        if($expression instanceof Select)
        {
            $temp = $expression;
            $expression = "(";
            $expression .= $temp->query();
            $expression .= ")";
        }
        array_push($this->columns, array($expression, null, $as));
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
    public function append_where(string $expression, $seperator_operator = null){array_push($this->wheres, array($expression, $seperator_operator));}
    public function append_having(string $expression, $seperator_operator = null){array_push($this->havings, array($expression, $seperator_operator));}
    public function order_by(string $column, $asc_desc = "ASC"){array_push($this->order_by, array($column, $asc_desc));}
    public function group_by(string $expression){array_push($this->group_by, $expression);}
    public function with_rollup($with_rollup = true){$this->with_rollup = $with_rollup;}
    public function distinct($distinct = true){$this->distinct = $distinct;}
    public function limit(int $limit){$this->limit = $limit;}
    public function offset(int $offset){$this->offset = $offset;}
    public function append_parameter(Parameter $value){array_push($this->parameter, $value);}
    public function appendBinder($binder) {array_push($this->binder, $binder);}
    public final function getFetchMode() : int {return $this->fetch_mode;}
    public final function fetchAssoc() {$this->fetch_mode = PDO::FETCH_ASSOC;}
    public final function fetchBoth() {$this->fetch_mode = PDO::FETCH_BOTH;}
    //public final function fetchBound() {$this->fetch_mode = PDO::FETCH_BOUND;}
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
    public function parameter() : array
    {
        return $this->parameter;
    }
    public function binder() : array {
        return $this->binder;
    }
    public function setResult( $result ) {
        $this->result = $result;
    }
    public function result() {
        return $this->result;
    }
    private function process_query()
    {
        $this->query = "SELECT ";
        if($this->distinct) $this->query .= "DISTINCT ";
        if(count($this->columns) > 0)
        {
            for($i = 0; $i<count($this->columns); $i++)
            {
                if($this->columns[$i][0] instanceof Select)
                {
                    $this->query .= '(';
                    $this->query .= $this->columns[$i][0]->query();
                    $this->query .= ')';
                    if($this->columns[$i][2] !== null)
                    {
                        $this->query .= ' AS ';
                        $this->query .= $this->columns[$i][2];
                    }
                }
                else
                {
                    if($this->columns[$i][1] !== null)
                    {
                        $this->query .= $this->columns[$i][1];
                        $this->query .= '.';
                    }
                    $this->query .= $this->columns[$i][0];
                    if($this->columns[$i][2] !== null)
                    {
                        $this->query .= ' AS ';
                        $this->query .= $this->columns[$i][2];
                    }
                }
                if($i < count($this->columns)-1)
                    $this->query .= ', ';
                else $this->query .= ' ';
            }
        }
        else $this->query .= "* ";
        $this->query .= "FROM ";
        if($this->main_table instanceof Select)
        {
            $this->query .= "(";
            $this->query .= $this->main_table->query();
            $this->query .= ") ";
        }
        else $this->query .= $this->main_table.' ';
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
        if(count($this->wheres) > 0)
        {
            $this->query .= "WHERE ";
            foreach($this->wheres as $where)
            {
                if($where[1] !== null) $this->query .= $where[1].' ';
                $this->query .= $where[0].' ';
            }
        }
        if(count($this->group_by) > 0)
        {
            $this->query .= "GROUP BY ";
            for($i = 0; $i<count($this->group_by); $i++)
            {
                $this->query .= $this->group_by[0].'';
                if($i < count($this->group_by) - 1) $this->query .= ', '; else $this->query .= ' ';
            }
            if($this->with_rollup) $this->query .= 'WITH ROLLUP ';
        }
        if(count($this->havings) > 0)
        {
            $this->query .= "HAVING ";
            foreach($this->havings as $having)
            {
                if($having[1] !== null) $this->query .= $having[1].' ';
                $this->query .= $having[0].' ';
            }
        }
        if(count($this->order_by) > 0)
        {
            $this->query .= 'ORDER BY ';
            for($i = 0; $i < count($this->order_by); $i++)
            {
                $this->query .= $this->order_by[$i][0].' '.$this->order_by[$i][1].' ';
                if($i < count($this->order_by) - 1) $this->query .= ',';
            }
        }
        if($this->limit !== null) $this->query .= 'LIMIT '.$this->limit.' ';
        if($this->offset !== null) $this->query .= 'OFFSET '.$this->offset.' ';
    }
    public function getParameterVariableIntegerOrder() {
        return count($this->parameter()) + 1;
    }
    public function getBinderVariableIntegerOrder() {
        return count($this->parameter()) + 1;
    }
}
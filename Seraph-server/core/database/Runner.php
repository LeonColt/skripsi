<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 07/02/2016
 * Time: 21:56
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
require_once 'Select.php';
require_once 'Insert.php';
require_once 'Update.php';
require_once 'Delete.php';
require_once 'Parameter.php';
require_once 'ParameterRef.php';
class Runner
{
    const QUERY_SELECT = 1;
    const QUERY_INSERT = 2;
    const QUERY_UPDATE = 3;
    const QUERY_DELETE = 4;
    private $connection, $queries, $result, $keys;
    public function __construct(PDO $connection = null, $default_key_length = 7)
    {
        $this->queries = array();
        $this->result = array();
        $this->keys = array(bin2hex(openssl_random_pseudo_bytes($default_key_length)));
        if($connection !== null)
        {
            $this->connection = $connection;
            $this->connection->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        } else $this->connection = null;
        $this->queries[$this->keys[0]] = array();
    }
    public function connect(string $host = 'localhost', int $port = 3306, string $database = null, string $username = 'root', string $password = '')
    {
        try{
            if((int)$port === 3306)
                $this->connection = new PDO("mysql:host=".$host.";dbname=".$database, $username, $password);
            else $this->connection = new PDO("mysql:host=".$host.":".$port.";dbname=".$database, $username, $password);
            $this->connection->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        } catch(PDOException $ex) {
            throw $ex;
        } catch (Exception $ex) {
            throw $ex;
        }
    }
    public function execute()
    {
        if($this->connection === null) throw new Exception("there is no connection");
        if(!$this->connection instanceof PDO) throw new Exception("invalid connection object");
        if( $this->connection instanceof PDO) {
            try {
                $one_by_ones = $this->prepareQueryByKey($this->keys[0]);
                foreach($one_by_ones as $by_one) {
                    if( $by_one[1] instanceof Select) {
                        if( $by_one[0]->execute()) {
                            foreach($by_one[1]->binder() as $item) {
                                if($item instanceof Parameter)
                                    $by_one[0]->bindColumn($item->getVariable(), $item->getValue(), $item->getType(), $item->getLength());
                                else if($item instanceof ParameterRef)
                                    $by_one[0]->bindColumn($item->getVariable(), $item->getValue(), $item->getType(), $item->getLength());
                            }
                            if( count($by_one[1]->binder()) > 0 )
                                $by_one[1]->setResult($by_one[0]->fetch(PDO::FETCH_BOUND));
                            else {
                                if($by_one[1]->getFetchMode() === -1)
                                    $by_one[1]->setResult($by_one[0]->fetchAll());
                                else if($by_one[1]->getFetchMode() === PDO::FETCH_ASSOC) {
                                    $temp = array();
                                    while( $row = $by_one[0]->fetch($by_one[1]->getFetchMode()) )
                                        array_push($temp, $row);
                                    $by_one[1]->setResult($temp);
                                }

                            }
                        }
                    } else if($by_one[1] instanceof Insert) {
                        $by_one[1]->setResult($by_one[0]->execute());

                    } else if($by_one[1] instanceof Update ) {
                        $by_one[1]->setResult($by_one[0]->execute());

                    } else if($by_one[1] instanceof Delete ) {
                        $by_one[1]->setResult($by_one[0]->execute());
                    }
                    else {
                        if(!isset($this->result[$by_one[1]]))
                            $this->result[$by_one[1]] = array();
                        if( (int)$by_one[2] === Runner::QUERY_SELECT ) {
                            if ($by_one[0]->execute() )
                                array_push($this->result[$by_one[1]], $by_one[0]->fetchAll());
                            else array_push($this->result[$by_one[1]], null);
                        }
                        else {
                            if ($by_one[0]->execute() )
                                array_push($this->result[$by_one[1]], true);
                            else array_push($this->result[$by_one[1]], false);
                        }
                    }
                }
                $query_of_queries = array();
                for($i = 0; $i < count($this->keys); $i++) {
                    if( $i == 0) continue;
                    array_push($query_of_queries, $this->prepareQueryByKey($this->keys[$i]));
                }
                foreach($query_of_queries as $queries) {
                    $this->connection->beginTransaction();
                    $success = true;
                    foreach($queries as $query) {
                        if( $query[1] instanceof Select) {
                            if( $query[0]->execute()) {
                                $query[1]->setResult($query[0]->fetchAll());
                            } else {$success = false; break;}
                        } else if($query[1] instanceof Insert) {
                            if( $query[0]->execute() ) {
                                $query[1]->setResult(true);
                            }else {$success = false; break;}

                        } else if($query[1] instanceof Update ) {
                            if( $query[0]->execute() ) {
                                $query[1]->setResult(true);
                            }else {$success = false; break;}

                        } else if($query[1] instanceof Delete ) {
                            if( $query[0]->execute() ) {
                                $query[1]->setResult(true);
                            }else {$success = false; break;}
                        }
                        else {
                            if(!isset($this->result[$by_one[1]]))
                                $this->result[$by_one[1]] = array();
                            if( (int)$by_one[2] === Runner::QUERY_SELECT ) {
                                if ($by_one[0]->execute() )
                                    array_push($this->result[$by_one[1]], $by_one[0]->fetchAll());
                                else {array_push($this->result[$by_one[1]], null); $success=false; break;}
                            }
                            else {
                                if ($by_one[0]->execute() )
                                    array_push($this->result[$by_one[1]], true);
                                else {array_push($this->result[$by_one[1]], false);$success=false; break;}
                            }
                        }
                    }
                    if($success) $this->connection->commit(); else $this->connection->rollBack();
                }

            } catch(PDOException $ex) {
                throw $ex;
            } catch (Exception $ex) {
                throw $ex;
            }
        }
    }
    private final function prepareQueryByKey($key) {
        $temp = array();
        $queries = $this->queries[$key];
        if( count($queries) < 1) return $temp;
        if( $this->connection instanceof PDO ) {
            foreach($queries as $query) {
                if( $query[0] instanceof Select ) {
                    $stmt = $this->connection->prepare( $query[0]->query());
                    foreach( $query[0]->parameter() as $item ) {
                        if($item instanceof Parameter)
                            $stmt->bindParam($item->getVariable(), $item->getValue(), $item->getType(), $item->getLength());
                        else if($item instanceof ParameterRef) {
                            $stmt->bindParam($item->getVariable(), $item->getValue(), $item->getType(), $item->getLength());
                        }
                    }
                    array_push($temp, array($stmt, $query[0]));
                }
                else if( $query[0] instanceof Insert ) {
                    $stmt = $this->connection->prepare( $query[0]->query());
                    foreach( $query[0]->parameter() as $item ) {
                        if($item instanceof Parameter)
                            $stmt->bindParam($item->getVariable(), $item->getValue(), $item->getType(), $item->getLength());
                    }
                    array_push($temp, array($stmt, $query[0]));
                }
                else if( $query[0] instanceof Update ) {
                    $stmt = $this->connection->prepare( $query[0]->query());
                    foreach( $query[0]->parameter() as $item ) {
                        if($item instanceof Parameter)
                            $stmt->bindParam($item->getVariable(), $item->getValue(), $item->getType(), $item->getLength());
                    }
                    array_push($temp, array($stmt, $query[0]));
                }
                else if( $query[0] instanceof Delete ) {
                    $stmt = $this->connection->prepare( $query[0]->query());
                    foreach( $query[0]->parameter() as $item ) {
                        if($item instanceof Parameter) $stmt->bindParam($item->getVariable(), $item->getValue(), $item->getType(), $item->getLength());
                    }
                    array_push($temp, array($stmt, $query[0]));
                } else {
                    $stmt = $this->connection->prepare($query[0]);
                    if( isset($query[1]) && !empty($query[1])) {
                        foreach( $query[1] as $item) {
                            if($item instanceof Parameter) $stmt->bindParam($item->getVariable(), $item->getValue(), $item->getType(), $item->getLength());
                        }
                    }
                    array_push($temp, array($stmt, $key, $query[2]));
                }
            }
        }
        else throw new Exception("No Connection to Database");
        return $temp;
    }
    public function append_query($query, $key = null, array $parameter = null, int $type = Runner::QUERY_SELECT)
    {
        if($key === null) array_push($this->queries[$this->keys[0]], array($query, $parameter, $type));
        else
        {
            if(in_array($key, $this->keys))
            {
                if(!isset($this->queries[$key]))
                    $this->queries[$key] = array(array($query, $parameter, $type));
                else array_push($this->queries[$key], array($query, $parameter, $type));
            }
            else
            {
                array_push($this->keys, $key);
                $this->queries[$key] = array(array($query, $parameter, $type));
            }
        }
    }
    public function clear_query()
    {
        $this->queries = array();
        $this->queries[$this->keys[0]] = array();
    }
    public function result($key = null)
    {
        if($key !== null) {
            if (isset($this->result[$key])) return $this->result[$key];
            else return null;
        }
        else return $this->result;
    }
    public function get_connection(){return $this->connection;}
    public function get_default_key() : string{return $this->keys[0];}
    public final function isAllQuerySuccess($key = null) : bool {
        $queries = $this->queries[$key];
        if( count($queries) < 1) return true;
        foreach($queries as $query) {
            if( $query[0] instanceof Select ) {
                if(count($query[0]->result()) < 1 ) return false;
            }
            else if( $query[0] instanceof Insert ) {
                if($query[0]->getResult() == false) return false;
            }
            else if( $query[0] instanceof Update ) {
                if($query[0]->getResult() == false) return false;
            }
            else if( $query[0] instanceof Delete ) {
                if($query[0]->getResult() == false) return false;
            }
        }
        return true;
    }
}
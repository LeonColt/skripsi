<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 26/01/2017
 * Time: 15:07
 */
require_once 'SDirectory.php';
require_once 'File.php';
class FileLoader
{
    public function load(array $file_ids, Runner $runner = null, bool $reveal = true) : array {
        $res = array();
        if( count($file_ids) === 0) return $res;
        if($runner !== null) $runner->clear_query();
        else {
            $runner = new Runner();
            $runner->connect(host, port, database, database_username, database_password);
        }
        $select = new Select('file');
        $select->append_column('id');
        $select->append_column('is_directory');
        $select->append_where('id IN (');
        for($i = 0; $i < count($file_ids); $i++) {
            if( $i > 0) $select->append_where(',');
            $select->append_where("?");
            $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), (int)$file_ids[$i]));
        }
        $select->append_where(')');
        $runner->append_query($select);
        $runner->execute();
        if( count($select->result()) === 0) return $res;
        foreach($select->result() as $item) {
            if((int)$item['is_directory'] === 1) {
                $file = new SDirectory();
                $file->setId((int)$item['id']);
                if($reveal) $file->reveal();
                array_push($res, $file);
            }
            else {
                $file = new File();
                $file->setId((int)$item['id']);
                if($reveal) $file->reveal();
                array_push($res, $file);
            }
        }
        return $res;
    }
}
<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 28/01/2017
 * Time: 18:00
 */
class RecycleBinLoader
{
    public function load(array $file_ids, Runner $runner = null) : array {
        $res = array();
        if( count($file_ids) === 0) return $res;
        $select = new Select('recyle_bin');
        $select->append_column('id', 'recyle_bin', 'id');
        $select->append_column('file_name');
        $select->append_column('is_directory');
        $select->append_column('id', 'benutzer', 'owner_id');
        $select->append_column('username', 'benutzer', 'owner_username');
        $select->append_column('email', 'benutzer', 'owner_email');
        $select->append_column('parent');
        $select->append_column('parent_undeleted');
        $select->append_column('original');
        $select->append_column('date_deleted');
        $select->append_join('JOIN', 'benutzer');
        $select->append_on('recyle_bin', 'file_owner', '=', 'benutzer', 'id');
        $select->append_where('recyle_bin.id IN (');
        for($i = 0; $i < count($file_ids); $i++) {
            if($i > 0) $select->append_where(',');
            $select->append_where('?');
            $select->append_parameter(new Parameter($select->getBinderVariableIntegerOrder(), $file_ids[$i]));
        }
        $select->append_where(')');
        if($runner === null) {
            $runner = new Runner();
            $runner->connect(host, port, database, database_username, database_password);
        }
        $runner->clear_query();
        $runner->append_query($select);
        $runner->execute();
        if(count($select->result()) === 0) return $res;
        foreach($select->result() as $item) {
            $bin = new RecycleBinObject();
            $bin->setId((int)$item['id']);
            $bin->setName($item["file_name"]);
            $bin->setAsDirectory((((int)$item['is_directory'] === 1) ? true : false));
            if($item['parent'] !== null) {
                $parent = new SDirectory();
                $parent->setId($item["parent"]);
                $parent->reveal();
                $bin->setParent($parent);
            }
            else if($item["parent_undeleted"] !== null) {
                $parent = new RecycleBinObject();
                $parent->setId((int)$item["parent_undeleted"]);
                $parent->reveal();
                $bin->setParentUndeleted($parent);
            }
            $bin->setOriginal($item["original"]);
            $bin->setDateDeleted($item["date_deleted"]);
            $bin->setOwner(new User($item["owner_id"], $item["owner_username"], $item["owner_email"], NULL, false));
            if($bin->isDirectory()) $bin->readChildren($runner);
            array_push($res, $bin);
        }
        return $res;
    }
}
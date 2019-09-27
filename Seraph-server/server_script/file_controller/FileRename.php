<?php

/**
 * Created by PhpStorm.
 * User: LeonColt
 * Date: 10/3/2016
 * Time: 5:03 PM
 */
class FileRename
{
    const ID = "nmlf-fr-9349380248932089024890238424";
    const NEW_NAME = "nmlf-fr-9439089085947890389038459034";
    private $id, $new_name;
    public function __construct(int $id, string $new_name)
    {
        $this->id = $id;
        $this->new_name = $new_name;
    }
    public function execute() {
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $new_name_ori = $this->new_name;
        $indexer = 1;
        while( $this->checkIfNewNameAlreadyExists($runner) ) {
            $this->new_name = $new_name_ori.'('.$indexer.')';
            $indexer++;
        }
        $update = new Update('file');
        $update->apppend_set('name', '?');
        $update->append_parameter(new Parameter($update->getParameterVariableIntegerOrder(), $this->new_name));
        $update->append_where('id = ?');
        $update->append_parameter(new Parameter($update->getParameterVariableIntegerOrder(), $this->id));

        $runner->clear_query();
        $runner->append_query($update);
        $runner->execute();
    }
    private function checkIfNewNameAlreadyExists(Runner &$runner) : bool {
        $runner->clear_query();
        $select_sub_query = new Select('file');
        $select_sub_query->append_column('parent');
        $select_sub_query->append_where('id = ?');
        $select = new Select('file');
        $select->append_column('name');
        $select->append_where('parent = ('.$select_sub_query->query().")");
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->id));
        $select->append_where("name = ?", 'AND');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->new_name));
        $select->append_where('id != ?', 'AND');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->id));
        $runner->append_query($select);
        $runner->execute();
        return count($select->result()) > 0;
    }
}
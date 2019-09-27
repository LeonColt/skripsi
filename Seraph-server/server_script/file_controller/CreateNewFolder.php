<?php

/**
 * Created by PhpStorm.
 * User: LeonColt
 * Date: 11/6/2016
 * Time: 4:08 PM
 */
require_once standart_path.'core/Random.php';
require_once standart_path.'core/Session/Session.php';
class CreateNewFolder
{
    const PARENT = "nmlf-cnf-02439085824902849375824";
    const NAME = "nmlf-cnf-304830957898403504909729849545";
    const LOGIN_ID = "nmlf-cnf-439078974984728947284792";
    private $parent, $benutzer, $name;
    public function __construct($parent, $name = "New Folder", $login_id)
    {
        $this->parent = $parent;
        $this->name = $name;
        if( $name === null ) $this->name = "New Folder";
        $this->benutzer = getUser($login_id);
    }

    public function execute() {
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);

        $this->loadParent($runner, $this->parent);

        if($this->parent !== null) $this->checkPrivilege($this->parent);

        $select = new Select('file');
        $select->append_column('1');
        $select->append_where('id = ?');
        $random = new Random();

        $id = $this->generateId($random, $runner);

        $name = $this->generateName($runner);

        $insert = new Insert('file');
        $insert->append_column_value('id', new Parameter($insert->getParameterVariableIntegerOrder(), $id));
        $insert->append_column_value('name', new Parameter($insert->getParameterVariableIntegerOrder(), $name));
        $insert->append_column_value('is_directory', new Parameter($insert->getParameterVariableIntegerOrder(), 1));
        if( $this->benutzer !== null )
            $insert->append_column_value('owner', new Parameter($insert->getParameterVariableIntegerOrder(), $this->benutzer->getId()));
        if( $this->benutzer === null ) {
            $insert->append_column_value('seen_by_all', new Parameter($insert->getParameterVariableIntegerOrder(), 1));
            $insert->append_column_value('download_by_all', new Parameter($insert->getParameterVariableIntegerOrder(), 1));
            $insert->append_column_value('modify_by_all', new Parameter($insert->getParameterVariableIntegerOrder(), 1));
        }
        if( $this->parent !== null ) {
            $parent = $this->parent;
            /** @var SDirectory $parent */
            $insert->append_column_value('parent', new Parameter($insert->getParameterVariableIntegerOrder(), $parent->getId()));
        }

        $runner->clear_query();
        $runner->append_query($insert);
        $runner->execute();
    }
    private function generateName(Runner &$runner) {
        $name_indexer = 0;
        $name = $this->name;
        $select = new Select('file');
        $select->append_column('1');
        $select->append_where('name = ?');
        $select->append_where('parent = ?', 'AND');

        do {
            if($name_indexer > 0) {
                $name = $this->name."(";
                $name .= $name_indexer;
                $name .= ")";
            }
            $select->clear_parameter();
            $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $name));
            $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->parent));
            $runner->clear_query();
            $runner->append_query($select);
            $runner->execute();

        } while(count($select->result()) > 0);
        return $name;
    }
    private function generateId(Random &$random, Runner &$runner) : int {
        do
        {
            $id = $random->random_number_int(1, PHP_INT_MAX);
        }
        while(isPrimaryKeyDuplicated($runner, $id, 'file', 'id'));
        return $id;
    }
    private function checkPrivilege(SDirectory $directory) {
        if(!$directory->getSecurityFileInfo()->isAllowedToUploadOnDirectory($this->benutzer))
            throw new Exception("Access Denied");
    }
    private function loadParent(Runner &$runner, &$parent) {
        if((int)$parent === 0) {
            $parent = null;
            return;
        }
        $loader = new FileLoader();
        $files = $loader->load(array($parent), $runner);
        if(count($files) === 0) throw new Exception("Parent folder does not exist");
        $parent = $files[0];
        if(!($parent instanceof SDirectory)) throw new Exception("Parent is not a folder");
    }
}
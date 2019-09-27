<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 20/12/2016
 * Time: 14:06
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
require_once standart_path.'core/Random.php';
require_once standart_path.'core/FileSystem.php';
require_once standart_path.'server_script/file_controller/file_object/File.php';
require_once standart_path.'server_script/file_controller/file_object/SDirectory.php';
require_once standart_path.'server_script/file_controller/file_object/RecycleBinObject.php';
require_once standart_path.'server_script/file_controller/file_object/RecycleBinLoader.php';
class RestoredFile extends File {
    private $old_id, $parent_deleted;

    /**
     * @return RecycleBinObject
     */
    public function getParentDeleted()
    {
        return $this->parent_deleted;
    }

    /**
     * @param RecycleBinObject $parent_deleted
     */
    public function setParentDeleted($parent_deleted)
    {
        $this->parent_deleted = $parent_deleted;
    }

    /**
     * @return int
     */
    public function getOldId() : int
    {
        return $this->old_id;
    }

    /**
     * @param int $old_id
     */
    public function setOldId(int $old_id)
    {
        $this->old_id = $old_id;
    }
}
class RestoredDirectory extends SDirectory {
    private $old_id, $parent_deleted;

    /**
     * @return RecycleBinObject
     */
    public function getParentDeleted()
    {
        return $this->parent_deleted;
    }

    /**
     * @param RecycleBinObject $parent_deleted
     */
    public function setParentDeleted($parent_deleted)
    {
        $this->parent_deleted = $parent_deleted;
    }
    /**
     * @return int
     */
    public function getOldId() : int
    {
        return $this->old_id;
    }

    /**
     * @param int $old_id
     */
    public function setOldId(int $old_id)
    {
        $this->old_id = $old_id;
    }
}
class RestoreFile
{
    const IDS = "cf-rf-48390842903847589375935734";
    const LOGIN_ID = "nmlf-rf-49394328478974375834534";
    private $files, $login_id, $benutzer;
    public function __construct($files, $login_id)
    {
        $this->files = explode(',', $files);
        $this->login_id = $login_id;
        $this->benutzer = getUser($login_id);
    }
    public function execute() {
        if($this->benutzer === null ) throw new Exception("Access Denied");
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);

        $loader = new RecycleBinLoader();
        $tree_bins = $loader->load($this->files, $runner);

        foreach($tree_bins as $tree_bin) $this->checkPrivilege($tree_bin);

        $bins = array();
        foreach($tree_bins as $tree_bin) $this->castBinsTreeToArray($bins, $tree_bin);

        $random = new Random();
        $files = $this->generateFile($runner, $random, $bins);

        $runner->clear_query();
        $key = $random->alphaNumeric(25);
        $this->pushFileIntoDatabase($runner, $key, $files);
        $this->deleteFromBins($runner, $key, $bins);

        $fs = new FileSystem();
        foreach($files as $file) {
            if($file instanceof RestoredDirectory) {
                $fs->move(standart_path.'daten/geloscht/'.$file->getOldId(),
                    standart_path.'daten/'.$file->getId());
            }
            else if($file instanceof RestoredFile) {
                $fs->move(standart_path.'daten/geloscht/'.$file->getOldId(),
                    standart_path.'daten/'.$file->getId());
            }
            else throw new Exception("Forbidden file template");
        }
        if($fs->commit()) {
            $runner->execute();
            if(!$runner->isAllQuerySuccess($key))
                $fs->roolBack();
        }
    }

    /**
     * @param Runner $runner
     * @param string $key
     * @param RecycleBinObject[] $bins
     */
    public function deleteFromBins(Runner &$runner, string $key, array $bins) {
        $delete = new Delete('recyle_bin');
        $delete->append_where('id IN (');
        for($i = 0; $i < count($bins); $i++) {
            if($i > 0) $delete->append_where(',');
            $delete->append_where('?');
            $delete->append_parameter(new Parameter($delete->getParameterVariableIntegerOrder(), $bins[$i]->getId()));
        }
        $delete->append_where(')');
        $runner->append_query($delete, $key);
    }

    /**
     * @param Runner $runner
     * @param string $key
     * @param BaseFile[] $files
     */
    public function pushFileIntoDatabase(Runner &$runner, string $key, array $files) {
        foreach($files as $file) {
            $insert = new Insert('file');
            $insert->append_column_value('id', new Parameter($insert->getParameterVariableIntegerOrder(), $file->getId()));
            $insert->append_column_value('name', new Parameter($insert->getParameterVariableIntegerOrder(), $file->getName()));
            if($file instanceof RestoredDirectory) $insert->append_column_value('is_directory', new Parameter($insert->getParameterVariableIntegerOrder(), 1));
            if($file->getOwner() !== null)
                $insert->append_column_value('owner', new Parameter($insert->getParameterVariableIntegerOrder(), $file->getOwner()->getId()));
            if($file->getParent() !== null)
                $insert->append_column_value('parent', new Parameter($insert->getParameterVariableIntegerOrder(), $file->getParent()->getId()));
            $runner->append_query($insert, $key);
        }
    }

    /**
     * @param Runner $runner
     * @param Random $random
     * @param RecycleBinObject[] $bins
     * @return BaseFile[]
     * @throws Exception
     */
    private function generateFile(Runner &$runner, Random $random, array $bins) : array {
        $temp = array();
        foreach($bins as $bin) {
            $file = null;
            if($bin->isDirectory()) $file = new RestoredDirectory();
            else $file = new RestoredFile();
            do {
                $exit = true;
                $id = $random->random_number_int(0, PHP_INT_MAX);
                if(isPrimaryKeyDuplicated($runner, $id, 'file', 'id')) $exit = false;
                foreach($temp as $item) {
                    if($item instanceof BaseFile) {
                        if((int)$item->getId() === $id) {
                            $exit = false;
                            break;
                        }
                    }
                    else throw new Exception("Forbidden file template");
                }
                if($exit) {
                    $file->setOldId($bin->getId());
                    $file->setName($bin->getName());
                    $file->setOwner($bin->getOwner());
                    if($bin->getParent() !== null) {
                        $dir = new RestoredDirectory();
                        $dir->setId($bin->getParentUndeleted()->getId());
                        $file->setParent($dir);
                    }
                    else if($bin->getParentUndeleted() !== null) $file->setParent($bin->getParentUndeleted());
                    $file->setId($id);
                    array_push($temp, $file);
                }
            }
            while(!$exit);
        }
        //reseting parent
        for($i = 0; $i < count($temp); $i++) {
            $file = $temp[$i];
            if($file instanceof RestoredDirectory) {
                for( $j = $i + 1; $j < count($temp); $j++) {
                    $child = $temp[$j];
                    if($child instanceof RestoredDirectory) {
                        if((int)$child->getParent()->getId() === $file->getOldId())
                            $child->setParent($file);
                    }
                    else if($child instanceof RestoredFile) {
                        if((int)$child->getParent()->getId() === $file->getOldId())
                            $child->setParent($file);
                    }
                    else throw new Exception("Forbidden file template");
                }
            }
        }
        return $temp;
    }

    /**
     * @param array $bins
     * @param RecycleBinObject $bin
     */
    private function castBinsTreeToArray(array &$bins, RecycleBinObject $bin) {
        array_push($bins, $bin);
        if($bin->isDirectory()) {
            if($bin->hasChildren()) {
                for($i = $bin->begin(); $i !== $bin->end(); $bin->next($i)) {
                    $this->castBinsTreeToArray($bins, $i);
                }
            }
        }
    }

    /**
     * @param RecycleBinObject $file
     * @throws Exception
     */
    private function checkPrivilege(RecycleBinObject $file) {
        if(!$this->benutzer->compare($file->getOwner())) throw new Exception("You have no right to restore the files");
        if($file->isDirectory()) {
            for($i = $file->begin(); $i !== $file->end(); $file->next($i)) {
                $this->checkPrivilege($i);
            }
        }
    }
}

//$exec = new RestoreFile("08775468335305478800", null);
//$exec->execute();
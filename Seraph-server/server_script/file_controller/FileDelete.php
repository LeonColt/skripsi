<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 06/07/2016
 * Time: 14:20
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
require_once standart_path.'core/Random.php';
require_once standart_path.'core/FileSystem.php';
require_once standart_path.'server_script/file_controller/CloudPathParser.php';
class FileOriginal extends File {} //do not remove, this class indicates that file is original file selected by user
class DirectoryOriginal extends SDirectory {} //do not remove, this class indicates that directory is original directory selected by user
class FileBin extends File {
    private $old_id, $original;
    public function setOldId(int $old_id){$this->old_id = $old_id;}
    public function getOldId() : int{return $this->old_id;}
    public function setOriginal(bool $original){$this->original = $original;}
    public function isOriginal() : bool{return $this->original;}
}
class DirectoryBin extends SDirectory {
    private $old_id, $original;
    public function setOldId(int $old_id){$this->old_id = $old_id;}
    public function getOldId() : int{return $this->old_id;}
    public function setOriginal(bool $original){$this->original = $original;}
    public function isOriginal() : bool{return $this->original;}
}
class FileDelete
{
    const PATHS = "cf-df-78941841841896408496419841631065841086946985048";
    const LOGIN_ID = "nmlf-fd-4948374832472947247294278429";
    private $files, $login_id, $benutzer;
    public function __construct(string $paths, $login_id){;
        $this->files = $paths;
        $this->login_id = $login_id;
        $this->benutzer = getUser($this->login_id);
    }
    public final function execute() {
        $runner = new Runner();
        $runner->connect(host, port, database, database_username. database_password);
        $temp = $this->files;
        $this->files = array();
        $this->loadFilesByPath($temp, $runner, $this->files);

        if( count($this->files) === 0) throw new Exception("at least one file must be provided");

        $this->deleteIndependenceFile($runner, $this->files);

        if(count($this->files) === 0 ) return;

        foreach($this->files as $file) $this->checkPrivilege($file); // check privilege

        $files = array();
        foreach($this->files as $file ) $this->castFileTreeToFileArray($files, $file); // change file tree into array

        $random = new Random();
        $bins = $this->generateBin($runner, $random, $files); //duplicate files, so files can be inserted into table recycle bin

        $runner->clear_query();
        $key = $random->alphaNumeric(5);
        $this->pushDeletedFileIntoBin($runner, $key, $bins);
        $this->deleteFile($runner, $key, $files);
        $fs = new FileSystem();
        foreach($bins as $bin) {
            if($bin instanceof FileBin) {
                $fs->move(standart_path.'daten/'.$bin->getOldId(),
                    standart_path.'daten/geloscht/'.$bin->getId());
            }
            else if($bin instanceof DirectoryBin) {
                $fs->move(standart_path.'daten/'.$bin->getOldId(),
                    standart_path.'daten/geloscht/'.$bin->getId());
            }
            else throw new Exception("Forbidden File Template");
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
     * @param BaseFile[] $files
     */
    private function deleteFile(Runner &$runner, string $key, array $files) {
        foreach($files as $file) {
            $delete = new Delete("file");
            $delete->append_table('file');
            $delete->append_table('download_record_files');
            $delete->append_table('download_record');
            $delete->append_table('upload_record');
            $delete->append_table('upload_record_files');
            $delete->append_table('privilege_benutzer_on_object');
            $delete->append_join('LEFT JOIN', 'download_record_files');
            $delete->append_on('file', 'id', '=', 'download_record_files', 'file_id');
            $delete->append_join('LEFT JOIN', 'download_record');
            $delete->append_on('download_record_files', 'download_id', '=', 'download_record', 'id');
            $delete->append_join('LEFT JOIN', 'upload_record');
            $delete->append_on('file', 'id', '=', 'upload_record', 'parent');
            $delete->append_join('LEFT JOIN', 'upload_record_files');
            $delete->append_on('upload_record_files', 'upload_id', '=', 'upload_record', 'upload_id');
            $delete->append_join('LEFT JOIN', 'privilege_benutzer_on_object');
            $delete->append_on('file', 'id', '=', 'privilege_benutzer_on_object', 'file_id');
            $delete->append_where('file.id = ?');
            $delete->append_parameter(new Parameter($delete->getParameterVariableIntegerOrder(), $file->getId()));
            $runner->append_query($delete, $key);
        }
    }
    private function pushDeletedFileIntoBin(Runner &$runner, string $key, array $bins) {
        foreach($bins as $bin) {
            if($bin instanceof DirectoryBin) {
                $insert = new Insert('recyle_bin');
                $insert->append_column_value('id', new Parameter($insert->getParameterVariableIntegerOrder(), $bin->getId()));
                $insert->append_column_value('file_name', new Parameter($insert->getParameterVariableIntegerOrder(), $bin->getName()));
                $insert->append_column_value('is_directory', new Parameter($insert->getParameterVariableIntegerOrder(), 1));
                if($bin->getOwner() !== null) $insert->append_column_value('file_owner', new Parameter($insert->getParameterVariableIntegerOrder(), $bin->getOwner()->getId()));

                if($bin->isOriginal()) {
                    $parent = $bin->getParent();
                    if($parent instanceof SDirectory) {
                        $insert->append_column_value('parent_undeleted', new Parameter($insert->getParameterVariableIntegerOrder(), $parent->getId()));
                    }
                    $insert->append_column_value('original', new Parameter($insert->getParameterVariableIntegerOrder(), 1));
                }
                else $insert->append_column_value('parent', new Parameter($insert->getParameterVariableIntegerOrder(), $bin->getParent()->getId()));
                $runner->append_query($insert, $key);

                $update = new Update("recyle_bin");
                $update->apppend_set('parent_undeleted', 'NULL');
                $update->apppend_set("parent", "?");
                $update->append_where("parent_undeleted = ?");
                $update->append_parameter(new Parameter($update->getParameterVariableIntegerOrder(), $bin->getOldId()));
                $runner->append_query($insert, $key);
            }
            else if($bin instanceof FileBin) {
                $insert = new Insert('recyle_bin');
                $insert->append_column_value('id', new Parameter($insert->getParameterVariableIntegerOrder(), $bin->getId()));
                $insert->append_column_value('file_name', new Parameter($insert->getParameterVariableIntegerOrder(), $bin->getName()));
                if($bin->getOwner() !== null) $insert->append_column_value('file_owner', new Parameter($insert->getParameterVariableIntegerOrder(), $bin->getOwner()->getId()));
                if($bin->isOriginal()) {
                    $parent = $bin->getParent();
                    if($parent instanceof SDirectory) {
                        $insert->append_column_value('parent_undeleted', new Parameter($insert->getParameterVariableIntegerOrder(), $parent->getId()));
                    }
                    $insert->append_column_value('original', new Parameter($insert->getParameterVariableIntegerOrder(), 1));
                }
                else $insert->append_column_value('parent', new Parameter($insert->getParameterVariableIntegerOrder(), $bin->getParent()->getId()));
                $runner->append_query($insert, $key);
            }
        }
    }
    private function generateBin(Runner &$runner, Random $random, array $files) : array {
        $temp = array();
        foreach($files as $file) {
            do {
                $exit = true;
                $id = $random->random_number_int(0, PHP_INT_MAX);
                if(isPrimaryKeyDuplicated($runner, $id, 'recyle_bin', 'id')) $exit = false;
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
                    if($file instanceof DirectoryOriginal) {
                        $bin = new DirectoryBin();
                        $bin->copyMeta($file);
                        $bin->setOldId($bin->getId());
                        $bin->setId($id);
                        $bin->revealParent();
                        $bin->setOriginal(true);
                        array_push($temp, $bin);
                    }
                    else if($file instanceof FileOriginal) {
                        $bin = new FileBin();
                        $bin->copyMeta($file);
                        $bin->setOldId($bin->getId());
                        $bin->setId($id);
                        $bin->revealParent();
                        $bin->setOriginal(true);
                        array_push($temp, $bin);
                    }
                    else if($file instanceof SDirectory) {
                        $bin = new DirectoryBin();
                        $bin->copyMeta($file);
                        $bin->setOldId($bin->getId());
                        $bin->setId($id);
                        $bin->setOriginal(false);
                        array_push($temp, $bin);
                    }
                    else if($file instanceof File) {
                        $bin = new FileBin();
                        $bin->copyMeta($file);
                        $bin->setOldId($bin->getId());
                        $bin->setId($id);
                        $bin->setOriginal(false);
                        array_push($temp, $bin);
                    }
                    else throw new Exception("Forbidden file template");
                }
            }
            while(!$exit);
        }
        //reseting parent
        for($i = 0; $i < count($temp); $i++) {
            $bin = $temp[$i];
            if($bin instanceof DirectoryBin) {
                for($j = $i + 1; $j < count($temp); $j++) {
                    $child = $temp[$j];
                    if($child instanceof DirectoryBin){
                        $parent = $child->getParent();
                        if((int)$parent->getId() === (int)$bin->getOldId()) $child->setParent($bin);
                    }
                    else if($child instanceof FileBin) {
                        $parent = $child->getParent();
                        if((int)$parent->getId() === (int)$bin->getOldId()) $child->setParent($bin);
                    }
                    else throw new Exception("Forbidden file template");
                }
            }
        }
        return $temp;
    }
    private function castFileTreeToFileArray(array &$files, $file) {
        if($file instanceof DirectoryOriginal) {
            array_push($files, $file);
            $file->readChildren();
            foreach($file->getChildren() as $child) $this->castFileTreeToFileArray($files, $child);
        }
        else if($file instanceof FileOriginal) array_push($files, $file);
        else if($file instanceof SDirectory) {
            array_push($files, $file);
            $file->readChildren();
            foreach($file->getChildren() as $child) $this->castFileTreeToFileArray($files, $child);
        }
        else if($file instanceof File) array_push($files, $file);
        else throw new Exception("Forbidden file template");
    }
    private function checkPrivilege($file) {
        if($file instanceof DirectoryOriginal) {
            if(!$file->getSecurityFileInfo()->isAllowedToDelete($this->benutzer)) throw new Exception("Some Files are not allowed to delete");
            foreach($file->getChildren() as $child) $this->checkPrivilege($child);
        }
        else if($file instanceof FileOriginal) {
            if(!$file->getSecurityFileInfo()->isAllowedToDelete($this->benutzer)) throw new Exception("Some Files are not allowed to delete");
        }
        else if($file instanceof SDirectory) {
            if(!$file->getSecurityFileInfo()->isAllowedToDelete($this->benutzer)) throw new Exception("Some Files are not allowed to delete");
            foreach($file->getChildren() as $child) $this->checkPrivilege($child);

        }
        else if($file instanceof File) {
            if(!$file->getSecurityFileInfo()->isAllowedToDelete($this->benutzer)) throw new Exception("Some Files are not allowed to delete");

        }
        else throw new Exception("Forbidden file template");
    }

    /**
     * @param Runner $runner
     * @param BaseFile[] $files
     */
    private function deleteIndependenceFile(Runner &$runner, array &$files) {
        if($this->benutzer === null) return;
        if($this->benutzer->isMember() || $this->benutzer->getGroupPrivilege() === 0) return;
        $independence_files = array();
        foreach($files as $id => $file ) {
            if( $file->getOwner() === null ) {
                array_push($independence_files, $file);
                unset($files[$id]);
            }
        }
        $runner->clear_query();
        $this->deleteFile($runner, "independence_delete", $independence_files);
        $runner->execute();
        foreach($independence_files as $file) {
            /** @var BaseFile $file */
            if(file_exists(standart_path.'daten/'.$file->getId()))
                unlink(standart_path.'daten/'.$file->getId());
        }
    }
    private function loadFiles(array $ids, Runner &$runner, array &$files) {
        $loader = new FileLoader();
        $files = $loader->load($ids, $runner, false);
        foreach($files as $file) {
            if($file instanceof File) {
                $fd = new FileOriginal();
                $fd->setId($file->getId());
                $fd->reveal();
                array_push($files, $fd);
            }
            else if( $file instanceof SDirectory ) {
                $dir = new DirectoryOriginal();
                $dir->setId($file->getId());
                $dir->reveal();
                $dir->readChildren();
                array_push($files, $dir);
                $ids = array();
                foreach($file->getChildren() as $child){
                    /** @var $child BaseFile */
                    array_push($ids, $child->getId());
                }
                $this->loadFiles($ids, $runner, $files);
            }
            else throw new Exception("Forbidden file template");
        }
    }

    /**
     * @param string $paths
     * @param Runner $runner
     * @param BaseFile[] $buffer
     * @throws Exception
     */
    private function loadFilesByPath($paths, Runner &$runner, array &$buffer ) {
        $cpp = new CloudPathParser();
        $files = $cpp->parse($paths, $runner);
        foreach($files as $file) {
            if($file instanceof File) {
                $fd = new FileOriginal();
                $fd->setId($file->getId());
                $fd->reveal();
                array_push($buffer, $fd);
            }
            else if( $file instanceof SDirectory ) {
                $dir = new DirectoryOriginal();
                $dir->setId($file->getId());
                $dir->reveal();
                $dir->readChildren();
                array_push($buffer, $dir);
                $ids = array();
                foreach($file->getChildren() as $child){
                    /** @var $child BaseFile */
                    array_push($ids, $child->getId());
                }
                $this->loadFiles($ids, $runner, $buffer);
            }
            else throw new Exception("Forbidden file template");
        }
    }
}
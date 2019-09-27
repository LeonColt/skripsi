<?php

/**
 * Created by PhpStorm.
 * User: LeonColt
 * Date: 10/24/2016
 * Time: 10:31 AM
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
require_once standart_path.'core/Random.php';
require_once standart_path.'core/FileSystem.php';
require_once standart_path.'server_script/file_controller/file_object/FileLoader.php';
class CopiedFile extends File {
    private $old_id, $original;
    public function __construct()
    {
        $this->original = false;
    }

    /**
     * @return bool
     */
    public function getOriginal()
    {
        return $this->original;
    }

    /**
     * @param bool $original
     */
    public function setOriginal(bool $original)
    {
        $this->original = $original;
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
class CopiedFolder extends SDirectory {
    private $old_id, $original;
    public function __construct(){parent::__construct();$this->original = false;}
    /**
     * @return bool
     */
    public function getOriginal(){return $this->original;}
    /**
     * @param bool $original
     */
    public function setOriginal(bool $original){$this->original = $original;}
    /**
     * @return int
     */
    public function getOldId() : int{return $this->old_id;}
    /**
     * @param int $old_id
     */
    public function setOldId(int $old_id){$this->old_id = $old_id;}
}
require_once standart_path.'server_script/file_controller/CloudPathParser.php';
class FileCopy
{
    const SOURCES = "nmlf-fcpy-934092849274720948920842904";
    const TARGET = "nmlf-fcpy-4983053984309482094728472834";
    const LOGIN_ID = "nmlf-fcpy-348032489324890842908402948924";
    private $sources, $target, $benutzer;
    public function __construct($sources, $target, $login_id)
    {
        $this->target = $target;
        $this->sources = $sources;
        $this->benutzer = getUser($login_id);
    }

    public function execute() {
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);

        $cpp = new CloudPathParser();
        $temp = $cpp->parse($this->target, $runner);
        if( count($temp) === 0) $target = null; else $target = $temp[0];
        if( $target !== null) {
            if($target instanceof SDirectory) {
                if(!$target->getSecurityFileInfo()->isAllowedToUploadOnDirectory($this->benutzer))
                    throw new Exception("Access Denied");
            }
            else throw new Exception("Forbidden File Template");
        }

        $sources = array();
        $this->loadSources($runner, $this->sources, $sources, 1);

        $random = new Random();
        $this->generateIdForSources($runner, $random, $sources);

        $this->checkDuplicateName($runner, $sources);

        $key = $random->alphaNumeric(5);
        $runner->clear_query();
        foreach($sources as $source ){
            /** @var BaseFile $source */
            $insert = new Insert('file');
            $insert->append_column_value('id', new Parameter($insert->getParameterVariableIntegerOrder(), $source->getId()));
            $insert->append_column_value('name', new Parameter($insert->getParameterVariableIntegerOrder(), $source->getName()));
            if( $source instanceof CopiedFolder )
                $insert->append_column_value('is_directory', new Parameter($insert->getParameterVariableIntegerOrder(), 1));

            if($this->benutzer !== null) {
                $insert->append_column_value('owner', new Parameter($insert->getParameterVariableIntegerOrder(), $this->benutzer->getId()));
            }
            else if($target === null) {
                if($source->getOwner() !== null )
                    $insert->append_column_value('owner', new Parameter($insert->getParameterVariableIntegerOrder(), $source->getOwner()->getId()));
                else {
                    $insert->append_column_value('seen_by_all', new Parameter($insert->getParameterVariableIntegerOrder(), 1));
                    $insert->append_column_value('download_by_all', new Parameter($insert->getParameterVariableIntegerOrder(), 1));
                    $insert->append_column_value('modify_by_all', new Parameter($insert->getParameterVariableIntegerOrder(), 1));
                }
            }
            else {
                if($target->getOwner() !== null)
                    $insert->append_column_value('owner', new Parameter($insert->getParameterVariableIntegerOrder(), $target->getOwner()->getId()));
                else if($source->getOwner() !== null ){
                    $insert->append_column_value('owner', new Parameter($insert->getParameterVariableIntegerOrder(), $source->getOwner()->getId()));
                }
                else {
                    $insert->append_column_value('seen_by_all', new Parameter($insert->getParameterVariableIntegerOrder(), 1));
                    $insert->append_column_value('download_by_all', new Parameter($insert->getParameterVariableIntegerOrder(), 1));
                    $insert->append_column_value('modify_by_all', new Parameter($insert->getParameterVariableIntegerOrder(), 1));
                }
            }
            if($source->getParent() !== null)
                $insert->append_column_value('parent', new Parameter($insert->getParameterVariableIntegerOrder(), $source->getParent()->getId()));
            $runner->append_query($insert, $key);
        }
        $fs = new FileSystem();
        foreach($sources as $source) {
            if($source instanceof CopiedFolder) continue;
            else if( $source instanceof CopiedFile)
                $fs->copy(standart_path.'daten/'.$source->getOldId(), standart_path.'daten/'.$source->getId());
        }
        if($fs->commit()){
            $runner->execute();
            if(!$runner->isAllQuerySuccess($key)) $fs->roolBack();
        }
    }

    /**
     * @param Runner $runner
     * @param BaseFile[] $sources
     * @throws Exception
     */
    private final function checkDuplicateName(Runner &$runner, array &$sources) {
        for($i = 0; $i < count($sources); $i++) {
            $source = $sources[$i];
            $select = new Select('file');
            $select->append_column('1');
            if( $source->getParent() === null ) $select->append_where('parent IS NULL');
            else {
                $select->append_where('parent = ?');
                $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $source->getParent()->getId()));
            }
            $select->append_where('name = ?', 'AND');
            $indexer = 0;
            $names = explode(".", $source->getName());
            $name = "";
            if(count($names) > 2) {
                for( $i = 0; $i < count($names) - 2; $i++)
                    $name .= $names[$i];
                $format = $names[count($names) - 1];
            }
            else if(count($names) === 2) {
                $name = $names[0];
                $format = $names[1];
            }
            else {
                $name .= $names[0];
                $format = "";
            }
            do {
                $exit = true;
                if( $indexer > 0 ) $test_name = $name."(".$indexer.")";
                else $test_name = $name;
                if(!empty($format)) $test_name.= (".".$format);
                $select->clear_parameter();
                if( $source->getParent() !== null )
                    $select->append_parameter(new Parameter($select->getBinderVariableIntegerOrder(), $source->getParent()->getId()));
                $select->append_parameter(new Parameter($select->getBinderVariableIntegerOrder(), $test_name));
                $runner->clear_query();
                $runner->append_query($select);
                $runner->execute();
                if(count($select->result()) > 0) $exit = false;
                for($j = $i + 1; $j < count($sources) && $exit; $j++) {
                    $source2 = $sources[$j];
                    if($source2->getParent() !== null && $source->getParent() !== null) {
                        if($source2->getParent()->getId() === $source->getParent()->getId()) {
                            if( strcasecmp($test_name, $source2->getName()) === 0 ) {
                                $exit = false;
                                break;
                            }
                        }
                    }
                }
                $indexer++;
            } while( !$exit );
            $sources[$i]->setName($test_name);
        }
    }

    /**
     * @param Runner $runner
     * @param Random $random
     * @param BaseFile[] $sources
     */
    private function generateIdForSources(Runner &$runner, Random &$random, array &$sources) {
        $random = new Random();
        for( $i = 0; $i < count($sources); $i++) {
            $source = $sources[$i];
            do {
                $exit = true;
                $id = $random->random_number_int(1, PHP_INT_MAX);
                if(isPrimaryKeyDuplicated($runner, $id, "file", 'id')) $exit = false;
                for( $j = $i + 1; $j < count($sources) && $exit; $j++) {
                    $source2 = $sources[$j];
                    if( $source->getId() === $source2->getId()) {
                        $exit = false; break;
                    }
                }
            } while(!$exit);
            for( $j = $i + 1; $j < count($sources); $j++) {
                $source2 = $sources[$j];
                if($source2->getParent() === null) continue;
                if($source->getId() === $source2->getParent()->getId()) {
                    $source2->setParent($source);
                }
            }
            if( $source instanceof CopiedFile ) {
                $source->setOldId($source->getId());
                $source->setId($id);
            }
            else if($source instanceof CopiedFolder){
                $source->setOldId($source->getId());
                $source->setId($id);
            }
        }
    }

    /**
     * @param Runner $runner
     * @param string[] $sources
     * @param BaseFile[] $sources_result
     * @param int $by_id_zero_path_one
     * @throws Exception
     */
    private function loadSources(Runner &$runner, $sources, array &$sources_result, int $by_id_zero_path_one = 0) {
        if( $by_id_zero_path_one === 0) {
            $loader = new FileLoader();
            $files = $loader->load($sources, $runner);
            foreach($files as $file) {
                if($file instanceof File) {
                    $cf = new CopiedFile();
                    $cf->copyMeta($file);
                    if(!$cf->getSecurityFileInfo()->isAllowedToDownload($this->benutzer)) throw new Exception("Access Denied to copy ".$cf->getId()." name : ".$cf->getName());
                    array_push($sources_result, $cf);
                }
                else if( $file instanceof SDirectory) {
                    $file->readChildren();
                    $cf = new CopiedFolder();
                    $cf->copyMeta($file);
                    if(!$cf->getSecurityFileInfo()->isAllowedToDownload($this->benutzer)) throw new Exception("Access Denied to copy ".$cf->getId()." name : ".$cf->getName());
                    array_push($sources_result, $cf);
                    $temp = array();
                    foreach($cf->getChildren() as $child) {
                        /** @var BaseFile $child */
                        array_push($temp, $child->getId());
                    }
                    if(count($temp) > 0) $this->loadSources($runner, $temp, $sources_result);
                }
            }
        }
        else {
            /*
             * file that path by CloydPathParser must be original copied, not derived
             */
            $cpp = new CloudPathParser();
            $files = $cpp->parse($sources, $runner);
            foreach($files as $file) {
                if($file instanceof File) {
                    $cf = new CopiedFile();
                    $cf->copyMeta($file);
                    if(!$cf->getSecurityFileInfo()->isAllowedToDownload($this->benutzer)) throw new Exception("Access Denied to copy ".$cf->getId()." name : ".$cf->getName());
                    $cf->setOriginal(true);
                    array_push($sources_result, $cf);
                }
                else if( $file instanceof SDirectory) {
                    $file->readChildren();
                    $cf = new CopiedFolder();
                    $cf->copyMeta($file);
                    if(!$cf->getSecurityFileInfo()->isAllowedToDownload($this->benutzer)) throw new Exception("Access Denied to copy ".$cf->getId()." name : ".$cf->getName());
                    $cf->setOriginal(true);
                    array_push($sources_result, $cf);
                    $temp = array();
                    foreach($cf->getChildren() as $child) {
                        /** @var BaseFile $child */
                        array_push($temp, $child->getId());
                    }
                    if(count($temp) > 0) $this->loadSources($runner, $temp, $sources_result);
                }
            }
        }
    }
}
<?php

/**
 * Created by PhpStorm.
 * User: LeonColt
 * Date: 10/3/2016
 * Time: 5:03 PM
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
require_once standart_path.'core/Random.php';
require_once standart_path.'server_script/file_controller/file_object/FileLoader.php';
require_once standart_path.'server_script/file_controller/CloudPathParser.php';
class FileMove
{
    const FROMS = "nmlf-fms-4387829749832749824";
    const TO = "nmlf-fms-949357028974982734982749";
    const LOGIN_ID = "nmlf-fms-5804985093284274247";
    private $froms, $to, $benutzer, $login_id;
    public function __construct($froms, $to, $login_id)
    {
        $this->froms = $froms;
        $this->to = $to;
        $this->login_id = $login_id;
        $this->benutzer = getUser($login_id);
    }
    public function execute() {
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);

        $target = $this->loadTarget($runner, $this->to);

        $sources = array();
        $this->loadSources($runner, $this->froms, $sources, $target);

        $this->checkDuplicateName($runner, $sources);

        $runner->clear_query();

        $random = new Random();
        $key = $random->alphaNumeric(10);
        foreach($sources as $source) {
            /** @var BaseFile $source */
            $update = new Update('file');
            $update->apppend_set('name', '?');
            $update->append_parameter(new Parameter($update->getParameterVariableIntegerOrder(), $source->getName()));
            if($target === null) $update->apppend_set('parent', 'NULL');
            else {
                $update->apppend_set('parent', '?');
                $update->append_parameter(new Parameter($update->getParameterVariableIntegerOrder(), $target->getId()));
            }
            $update->append_where('id = ?');
            $update->append_parameter(new Parameter($update->getParameterVariableIntegerOrder(), $source->getId()));
            $runner->append_query($update, $key);
        }
        $runner->execute();
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
     * @param string $sources
     * @param BaseFile[] $sources_result
     * @param SDirectory $target
     * @throws Exception
     */
    private function loadSources(Runner &$runner, string $sources, array &$sources_result, $target) {
        $cpp = new CloudPathParser();
        $files = $cpp->parse($sources, $runner);
        foreach($files as $file) {
            /** @var BaseFile $file */
            if(!$file->getSecurityFileInfo()->isAllowedToDownload($this->benutzer)) throw new Exception("Access Denied to move ".$file->getId()." name : ".$file->getName());
            $file->setParent($target);
            array_push($sources_result, $file);
        }
    }
    /**
     * @param Runner $runner
     * @param string $target
     * @return SDirectory
     * @throws Exception
     */
    private function loadTarget(Runner &$runner, string $target) {
        $cpp = new CloudPathParser();
        $files = $cpp->parse($target, $runner);
        if(count($files) === 0) return null;
        $file = $files[0];
        if($file instanceof SDirectory) {
            if($file->getSecurityFileInfo()->isAllowedToUploadOnDirectory($this->benutzer)) return $file;
            else throw new Exception("Access Denied");
        }
        throw new Exception("Forbidden File Template");
    }
}
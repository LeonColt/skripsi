<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 12/02/2017
 * Time: 22:25
 */
require_once standart_path.'server_script/file_controller/file_object/FileLoader.php';
require_once standart_path.'server_script/file_controller/file_object/RecycleBinLoader.php';
require_once standart_path.'server_script/file_controller/file_object/RecycleBinObject.php';
class CloudPathParser {
    const TYPE_CLOUD = 0;
    const TYPE_RECYCLE_BIN = -1;
    /*
     *  path must begin with "cloud:" as root
     *  path must begin with "CloudRecycleBin:" as recycle bin root
     *  must be feed with absolute path
     *  can take wildcard
     *  separated by vertical bar(|)
     *  0 key as cloud
     *  -1 key as recycle bin
     */
    /**
     * @param string $paths_wildcard
     * @param Runner|null $runner
     * @return BaseFile[]
     * @throws Exception
     */
    public function parse(string $paths_wildcard, Runner &$runner = null) : array {
        $res = array();
        $paths = explode("|", $paths_wildcard);
        foreach($paths as $path) {
            /** @var string $path */
            $names = explode("\\", trim($path));
            $file = null;
            $flag_type = 1;
            for ( $i = 0; $i < count($names); $i++) {
                /** @var BaseFile $file */
                $name = $names[$i];
                if(strcasecmp($name, "Cloud:") == 0) {
                    $flag_type = CloudPathParser::TYPE_CLOUD;
                    continue;
                }
                else if(strcasecmp($name, "CloudRecycleBin:") == 0) {
                    $flag_type = CloudPathParser::TYPE_RECYCLE_BIN;
                    continue;
                }
                if(strcasecmp($name, "") == 0 && $i == count($names) - 1) continue;
                if($i == 1) $file = $this->getFileByName($runner, $name, 0, $flag_type);
                else $file = $this->getFileByName($runner, $name, $file->getId(), $flag_type);
            }
            if($file !== null) {
                if( $flag_type == CloudPathParser::TYPE_CLOUD) $file->setPath($path);
                array_push($res, $file);
            }
        }
        return $res;

    }
    /**
     * @param Runner $runner
     * @param string $name
     * @param int $parent_id
     * @return BaseFile
     * @throws Exception
     */
    private function getFileByName(Runner &$runner, string $name, int $parent_id, $flag_type) {
        if( $flag_type === CloudPathParser::TYPE_CLOUD) {
            $select = new Select('file');
            $select->append_column('id', 'file', 'file_id');
            $select->append_where('name = ?');
            $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $name));
            if($parent_id === 0) $select->append_where('parent IS NULL', 'AND');
            else {
                $select->append_where('parent = ?', 'AND');
                $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $parent_id));
            }
            $runner->clear_query();
            $runner->append_query($select);
            $runner->execute();
            if(count($select->result()) === 0) throw new Exception("File Not Found");
            $loader = new FileLoader();
            $files = $loader->load(array($select->result()[0]["file_id"]), $runner);
            if(count($files) === 0) throw new Exception("File Not Found");
            return $files[0];
        }
        else if( $flag_type === CloudPathParser::TYPE_RECYCLE_BIN ) {
            $select = new Select('recyle_bin');
            $select->append_column('id', 'recyle_bin', 'file_id');
            $select->append_where('file_name = ?');
            $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $name));
            if($parent_id === 0) $select->append_where('parent IS NULL', 'AND');
            else {
                $select->append_where('parent = ?', 'AND');
                $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $parent_id));
            }
            $runner->clear_query();
            $runner->append_query($select);
            $runner->execute();
            if(count($select->result()) === 0) throw new Exception("File Not Found");

            $loader = new RecycleBinLoader();
            $files = $loader->load(array($select->result()[0]['file_id']), $runner);
            if(count($files) === 0) throw new Exception("File Not Found");
            return $files[0];
        }
        else throw new Exception("Invalid Path");
    }
}
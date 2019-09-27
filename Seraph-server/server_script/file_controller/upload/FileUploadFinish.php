<?php

/**
 * Created by PhpStorm.
 * User: LeonColt
 * Date: 10/11/2016
 * Time: 5:06 AM
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
require_once standart_path.'core/Random.php';
class FileUploadFinish
{
    const ID = "cf-uff-84593758927427584204830579720948029483957";
    const LOGIN_ID = "cf-uff-49895492374395845940964-99034729084";
    private $id, $benutzer, $login_id, $parent, $owner, $files;
    public function __construct(string $id, $login_id){
        $this->id = $id;
        $this->login_id = $login_id;
        $this->benutzer = getUser($login_id);
    }

    public function execute() {
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $this->extractParentAndOwner($runner);
        $this->extractFiles($runner);
        $this->checkFileDuplicate($runner);
        $this->setId($runner);
        $this->setParent();
        $real_files = array();
        $this->reserveParentOnTop($real_files, $this->files, $this->parent);
        $path_upload = standart_path.'daten/upload/';
        foreach($real_files as $file) {
            if((int)$file["is_directory"] === 0) {
                if(!file_exists($path_upload.$this->id."-".$file["file_id"]))
                    throw new Exception("File Upload missing");
            }
        }
        $insert = new Insert('file');
        foreach($real_files as $file ) {
            $insert->append_column_value('id', new Parameter($insert->getParameterVariableIntegerOrder(), $file["id"]));
            $insert->append_column_value('name', new Parameter($insert->getParameterVariableIntegerOrder(), $file["name"]));
            $insert->append_column_value('is_directory', new Parameter($insert->getParameterVariableIntegerOrder(), $file["is_directory"]));
            if($this->owner !== null && $this->owner !== 0)
                $insert->append_column_value('owner', new Parameter($insert->getParameterVariableIntegerOrder(), $this->owner));
            else {
                $insert->append_column_value('seen_by_all', new Parameter($insert->getParameterVariableIntegerOrder(), 1));
                $insert->append_column_value('download_by_all', new Parameter($insert->getParameterVariableIntegerOrder(), 1));
                $insert->append_column_value('modify_by_all', new Parameter($insert->getParameterVariableIntegerOrder(), 1));
            }
            $insert->append_column_value('parent', new Parameter($insert->getParameterVariableIntegerOrder(), $file['parent']));
        }
        $runner->append_query($insert);
        $runner->execute();
        //if(!$runner->isAllQuerySuccess($this->id)) throw new Exception('Finishing File Upload Fail');
        foreach($real_files as $key => $file) if((int)$file["is_directory"] === 1) unset($real_files[$key]);
        $path_file = standart_path.'daten/';
        foreach($real_files as $file)
            rename($path_upload.$this->id."-".$file["file_id"], $path_file.$file["id"]);
    }
    private final function reserveParentOnTop(&$out_file, &$in_files, $parent) {
        $second_parents = array();
        foreach( $in_files as $key => $file ) {
            if( (int)$file["parent"] == (int)$parent ) {
                array_push($second_parents, $file["id"]);
                array_push($out_file, $file);
                unset($in_files[$key]);
            }
        }
        foreach($second_parents as $second_parent) {
            $this->reserveParentOnTop($out_file, $in_files, $second_parent);
        }
    }
    private final function setParent() {
        if($this->parent === null) return;
        for( $i = 0; $i < count($this->files); $i++) {
            if( $this->files[$i]["parent"] === null )
                $this->files[$i]["parent"] = $this->parent;

        }
    }
    private final function checkFileDuplicate( Runner &$runner ) {
        $select = new Select('file');
        $select->append_column('name');
        if( $this->parent === null ) $select->append_where('parent IS NULL');
        else {
            $select->append_where('parent = ?');
            $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->parent));
        }
        $runner->clear_query();
        $runner->append_query($select);
        $runner->execute();

        $real_file = $select->result();

        for($i = 0; $i < count($this->files); $i++) {
            $indexer = 0;
            $names = explode(".", $this->files[$i]["name"]);
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
                $exist = false;
                if( $indexer > 0 ) $test_name = $name."(".$indexer.")";
                else $test_name = $name;
                if(!empty($format)) $test_name.= (".".$format);
                foreach($real_file as $rf) {
                    if( strcmp($rf["name"], $test_name) === 0 )
                        $exist = true;
                }
                $indexer++;
            } while( $exist );
            $this->files[$i]["name"] = $test_name;
        }
    }
    private final function setId( Runner &$runner) {
        $select = new Select('file');
        $select->append_column('1');
        $select->append_where('id = ?');
        $random = new Random();

        for($i = 0; $i < count($this->files); $i++ ) {
            do {
                $exists = false;
                $file_id = $random->random_number_int(1);
                $select->clear_parameter();
                $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $file_id));
                $runner->clear_query();
                $runner->append_query($select);
                $runner->execute();
                if(count($select->result()) > 0) $exists = true;
                foreach($this->files as $file) {
                    if(!isset($file["id"])) break;
                    if( (int)$file_id === (int)$file["id"] ) {
                        $exists = true;
                        break;
                    }
                }
            } while($exists);
            for($j = 0; $j < count($this->files); $j++ ) {
                if( (int)$this->files[$j]["parent"] == (int)$this->files[$i]["file_id"] )
                    $this->files[$j]["parent"] = $file_id;
            }
            $this->files[$i]["id"] = $file_id;
        }
    }
    private final function extractFiles(Runner &$runner) {
        $select = new Select('upload_record');
        $select->append_column('file_id', 'upload_record_files');
        $select->append_column('name');
        $select->append_column('is_directory');
        $select->append_column('parent', 'upload_record_files');
        $select->append_join('INNER JOIN', 'upload_record_files');
        $select->append_on('upload_record', 'upload_id', '=', 'upload_record_files', 'upload_id');
        $select->append_where('upload_record.upload_id = ?');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->id));
        if( $this->benutzer === null ) $select->append_where('benutzer IS NULL', 'AND');
        else {
            $select->append_where('benutzer = ?', 'AND');
            $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->benutzer->getId()));
        }
        $runner->clear_query();
        $runner->append_query($select);
        $runner->execute();
        $this->files = $select->result();
        if( count($this->files) < 1 ) throw new Exception("File Upload Not Found");
    }
    private final function extractParentAndOwner(Runner &$runner) {
        $select = new Select('upload_record');
        $select->append_column('parent');
        $select->append_column('benutzer');
        $select->append_where('upload_id = ?');
        $select->append_parameter(new Parameter(1, $this->id));
        $runner->clear_query();
        $runner->append_query($select);
        $runner->execute();
        if( count($select->result()) > 0) {
            $this->parent = $select->result()[0][0];
            $this->owner = $select->result()[0][1];
        } else throw new Exception("Illegal File Upload");
    }
}
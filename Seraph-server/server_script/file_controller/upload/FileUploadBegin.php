<?php

/**
 * Created by PhpStorm.
 * User: LeonColt
 * Date: 9/10/2016
 * Time: 5:08 PM
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
require_once standart_path.'server_script/file_controller/CloudPathParser.php';
class FileUploadBegin
{
    const FILES = "nmlf-fub-29893840237498354387593485374";
    const LOGIN_ID = "nmlf-fub-4945098294345820420492358047503573985";
    const PARENT = "nmlf-fub-398350948603480385093485034753957935";

    const INPUT_FILES = "nmlf-fub-i-9380395809347503495734"; // array
    const INPUT_FILES_NAME = "nmlf-fub-i-5803495808504937503947U5"; // string
    const INPUT_FILES_IS_DIRECTORY = "nmlf-fub-i-34829035809574398573495"; // boolean
    const INPUT_FILES_SIZE = "nmlf-fub-i-043905893475893579385"; //string
    const INPUT_FILES_CID = "nmlf-fub-i-495893045739874293742984723984"; //int
    const INPUT_FILES_CHILDREN = "nmlf-fub-i-34398579384759384759843759375983475938"; //array
    /*
     * receive file as json encode
     * what needed is user who is uploaded the files, the size of file, format, and its parent
     */
    const RETURN_ID = "id";
    const RETURN_FILES = "Files";
    const RETURN_FILES_ID = "File Id";
    const RETURN_FILES_NAME = "File Name";
    const RETURN_FILES_PARENT = "File Parent";
    const RETURN_FILES_CID = "File cid";
    private $files, $benutzer, $parent_root;
    public function __construct(string $files, $benutzer, $parent_path) {
        $this->files = json_decode($files, true, 512, JSON_BIGINT_AS_STRING)[FileUploadBegin::INPUT_FILES];
        $this->benutzer = getUser($benutzer);
        $this->parent_root = $parent_path;
    }
    public function execute() : array {
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $cpp = new CloudPathParser();
        $temps = $cpp->parse($this->parent_root, $runner);
        if(count($temps) === 0) $this->parent_root = null; else $this->parent_root = $temps[0];

        $select = new Select('upload_record');
        $select->append_column('upload_id');
        $select->append_where('upload_id = ?');
        $random = new Random();

        $runner->clear_query();
        do {
            $select->clear_parameter();
            $id = $random->alphaNumeric(id_upload_length);
            $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $id));
            $runner->clear_query();
            $runner->append_query($select);
            $runner->execute();
        } while(count($select->result()) > 0);

        for($i = 0; $i < count($this->files); $i++) $this->setId($runner, $this->files[$i], $this->files);

        $insert = new Insert('upload_record');
        $insert->append_column_value('upload_id', new Parameter($insert->getParameterVariableIntegerOrder(), $id));
        if($this->benutzer !== null) $insert->append_column_value('benutzer', new Parameter($insert->getParameterVariableIntegerOrder(), $this->benutzer->getId()));
        if($this->parent_root !== null)
            $insert->append_column_value('parent', new Parameter($insert->getParameterVariableIntegerOrder(),
                $this->parent_root->getId()));

        $runner->clear_query();
        $runner->append_query($insert, $id);
        for($i = 0; $i < count($this->files); $i++) $this->insertFile($runner, $this->files[$i], $id, null);
        $runner->execute();

        if($insert->getResult()) {
            $temp = array();
            $temp[FileUploadBegin::RETURN_ID] = $id;
            $files = array();
            foreach($this->files as $file) $this->prepareFileUploadReturning($files, $file);
            $temp[FileUploadBegin::RETURN_FILES] = $files;
            return $temp;
        }
        else throw new Exception("Cannot Preparing File Upload");
    }

    private final function prepareFileUploadReturning(&$buffer_file, &$file) {
        $temp = array();
        $temp[FileUploadBegin::RETURN_FILES_ID] = $file["id"];
        $temp[FileUploadBegin::RETURN_FILES_NAME] = $file[FileUploadBegin::INPUT_FILES_NAME];
        $temp[FileUploadBegin::RETURN_FILES_PARENT] = ( isset($file["parent"]) ) ? $file["parent"] : 0;
        $temp[FileUploadBegin::RETURN_FILES_CID] = $file[FileUploadBegin::INPUT_FILES_CID];
        array_push($buffer_file, $temp);
        if(isset($file[FileUploadBegin::INPUT_FILES_CHILDREN])) {
            for($i = 0; $i < count($file[FileUploadBegin::INPUT_FILES_CHILDREN]); $i++)
                $this->prepareFileUploadReturning($buffer_file, $file[FileUploadBegin::INPUT_FILES_CHILDREN][$i]);
        }
    }

    private final function insertFile(Runner &$runner, &$file, $id, $parent) {
        $insert = new Insert('upload_record_files');
        $insert->append_column_value('file_id', new Parameter($insert->getParameterVariableIntegerOrder(), $file['id']));
        $insert->append_column_value('upload_id', new Parameter($insert->getParameterVariableIntegerOrder(), $id));
        $insert->append_column_value('name', new Parameter($insert->getParameterVariableIntegerOrder(), $file[FileUploadBegin::INPUT_FILES_NAME]));
        $insert->append_column_value("is_directory", new Parameter($insert->getParameterVariableIntegerOrder(), $file[FileUploadBegin::INPUT_FILES_IS_DIRECTORY]));
        $insert->append_column_value('size', new Parameter($insert->getParameterVariableIntegerOrder(), $file[FileUploadBegin::INPUT_FILES_SIZE]));
        if($parent !== null) $insert->append_column_value('parent', new Parameter($insert->getParameterVariableIntegerOrder(), $parent["id"]));
        $runner->append_query($insert, $id);
        if(isset($file[FileUploadBegin::INPUT_FILES_CHILDREN])) {
            for($i = 0; $i < count($file[FileUploadBegin::INPUT_FILES_CHILDREN]); $i++)
                $this->insertFile($runner, $file[FileUploadBegin::INPUT_FILES_CHILDREN][$i], $id, $file);
        }
    }

    private final function setId(Runner &$runner, &$file, &$file_id_checker) {
        $select = new Select('upload_record_files');
        $select->append_column('file_id');
        $select->append_where('file_id = ?');
        $random = new Random();

        do {
            $file_id = $random->random_number_int(1);
            $random->random_number_int();
            $duplicate = false;
            for($i = 0; $i < count($file_id_checker); $i++) {
                if($this->checkIdDuplicateInFile($file_id_checker[$i], (float)$file_id)) {
                    $duplicate = true;
                    break;
                }
            }
            if($duplicate) continue;
            $select->clear_parameter();
            $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $file_id));
            $runner->clear_query();

            $runner->append_query($select);

            $runner->execute();
        } while(count($select->result()) > 0);
        $file["id"] = $file_id;
        if(isset($file[FileUploadBegin::INPUT_FILES_CHILDREN])) {
            for($i = 0; $i < count($file[FileUploadBegin::INPUT_FILES_CHILDREN]); $i++)
                $this->setId($runner, $file[FileUploadBegin::INPUT_FILES_CHILDREN][$i], $file_id_checker);
        }
    }

    private final function checkIdDuplicateInFile(&$file, float $id) : bool {
        if(isset($file["id"])) {
            if((float)$file["id"] === $id) return true;
        } else return false;
        if(isset($file[FileUploadBegin::INPUT_FILES_CHILDREN])) {
            for($i = 0; $i < count($file[FileUploadBegin::INPUT_FILES_CHILDREN]); $i++)
                return $this->checkIdDuplicateInFile($file[FileUploadBegin::INPUT_FILES_CHILDREN][$i], $id);
        }
        return false;
    }
}
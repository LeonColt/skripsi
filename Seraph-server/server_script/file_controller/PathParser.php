<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 09/02/2017
 * Time: 13:39
 */
require_once standart_path.'server_script/file_controller/file_object/FileLoader.php';
class PathParser
{
    const INPUT_PATH = "cf-fp-3494829479827428472894";
    const LOGIN_ID = "cf-fp-738917381237819371893719823";
    const MAX_PATH = 30000;

    const GET_FILE_ID = "cf-gafsaf-8789955585897959"; //string then cast to UINT64
    const GET_FILE_NAME = "cf-gafsaf-7864051513135464313"; //string
    const GET_FILE_SIZE = "cf-gafsaf-8485964928658996349252"; //string then cast to UINT64
    const GET_FILE_OWNER = "cf-gafsaf-8589649621635825420032"; //string
    const GET_FILE_EXISTS = "cf-gafsaf-02850205954057947"; //bool
    const GET_FILE_PARENT = "cf-gafsaf-02850205349037403"; //string then cast to UINT64
    const GET_FILE_SEEN_BY_ALL = "cf-gafsaf-3874982742936427423"; //bool
    const GET_FILE_DOWNLOAD_BY_ALL = "cf-gafsaf-995-0350937853953953905834";//bool
    const GET_FILE_MODIFY_BY_ALL = "cf-gafsaf-328585874892374289472";//bool
    const GET_FILE_DOWNLOAD_ALLOWED_CURRENT_USER = "cf-gafsaf-439048028490238402489204";//bool
    const GET_FILE_UPLOAD_ON_DIRECTORY_ALLOWED_CURRENT_USER = "cf-gafsaf-349823904823849203482904";//bool
    const GET_FILE_DELETE_ALLOWED_CURRENT_USER = "cf-gafsaf-39420948238420482084294";//bool
    const GET_FILE_IS_DIRECTORY = "cf-gafsaf-4983209482394723894723742"; //bool
    const GET_FILE_DATE_CREATED = "cf-gafsaf-3942948248242949204"; //string

    private $paths, $benutzer;
    public function __construct(string $path, $login_id)
    {
        $this->paths = explode("\\", $path);
        $this->benutzer = getUser($login_id);
    }
    public function execute() {
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $file = null;
        for( $i = 1; $i < count($this->paths); $i++) {
            /** @var BaseFile $file */
            $path = $this->paths[$i];
            if(strcasecmp($path, "") == 0 && $i == count($this->paths) - 1) continue;
            if($i == 1) $file = $this->getFileByName($runner, $path, 0);
            else $file = $this->getFileByName($runner, $path, $file->getId());
        }
        return $this->prepareFile($file);
    }
    /**
     * @param BaseFile $file
     * @return array
     * @throws Exception
     */
    private function prepareFile($file) : array {
        $result = array();
        if( $file instanceof SDirectory) {
            $result[PathParser::GET_FILE_ID] = (string)$file->getId();
            $result[PathParser::GET_FILE_NAME] = $file->getName();
            $result[PathParser::GET_FILE_IS_DIRECTORY] = true;
            $result[PathParser::GET_FILE_EXISTS] = true;
            $result[PathParser::GET_FILE_SIZE] = (string)$file->getSize();
            $result[PathParser::GET_FILE_OWNER] = ( $file->getOwner() === null) ? "Independence" : $file->getOwner()->getUsername();
            //$temp[GetFile::GET_FILE_PARENT] = (string)( $file["username"] == null) ? null : ($file->getParent()->getId());
            $result[PathParser::GET_FILE_SEEN_BY_ALL] = $file->getSecurityFileInfo()->isSeenByAll();
            $result[PathParser::GET_FILE_DOWNLOAD_BY_ALL] = $file->getSecurityFileInfo()->isDownloadByAll();
            $result[PathParser::GET_FILE_MODIFY_BY_ALL] = $file->getSecurityFileInfo()->isModifyByAll();
            $result[PathParser::GET_FILE_DOWNLOAD_ALLOWED_CURRENT_USER] = $file->getSecurityFileInfo()->isAllowedToDownload($this->benutzer);
            $result[PathParser::GET_FILE_DELETE_ALLOWED_CURRENT_USER] = $file->getSecurityFileInfo()->isAllowedToDelete($this->benutzer);
            $result[PathParser::GET_FILE_DATE_CREATED] = $file->getDateCreated();
        }
        else if($file instanceof File) {
            $result[PathParser::GET_FILE_ID] = (string)$file->getId();
            $result[PathParser::GET_FILE_NAME] = $file->getName();
            $result[PathParser::GET_FILE_IS_DIRECTORY] = false;
            $result[PathParser::GET_FILE_EXISTS] = true;
            $result[PathParser::GET_FILE_SIZE] = (string)$file->getSize();
            $result[PathParser::GET_FILE_OWNER] = ( $file->getOwner() === null) ? "Independence" : $file->getOwner()->getUsername();
            //$temp[GetFile::GET_FILE_PARENT] = (string)( $file["username"] == null) ? null : ($file->getParent()->getId());
            $result[PathParser::GET_FILE_SEEN_BY_ALL] = $file->getSecurityFileInfo()->isSeenByAll();
            $result[PathParser::GET_FILE_DOWNLOAD_BY_ALL] = $file->getSecurityFileInfo()->isDownloadByAll();
            $result[PathParser::GET_FILE_MODIFY_BY_ALL] = $file->getSecurityFileInfo()->isModifyByAll();
            $result[PathParser::GET_FILE_DOWNLOAD_ALLOWED_CURRENT_USER] = $file->getSecurityFileInfo()->isAllowedToDownload($this->benutzer);
            $result[PathParser::GET_FILE_DELETE_ALLOWED_CURRENT_USER] = $file->getSecurityFileInfo()->isAllowedToDelete($this->benutzer);
            $result[PathParser::GET_FILE_DATE_CREATED] = $file->getDateCreated();
        }
        else throw new Exception("Forbidden file template");
        return $result;
    }
    /**
     * @param Runner $runner
     * @param string $name
     * @param int $parent_id
     * @return BaseFile
     * @throws Exception
     */
    public function getFileByName(Runner &$runner, string $name, int $parent_id) {
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
}
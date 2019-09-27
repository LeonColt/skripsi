<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 21/02/2017
 * Time: 18:39
 */
require_once standart_path.'server_script/file_controller/CloudPathParser.php';
class DirectoryChildrenCount
{
    const PATH = "cf-dcc-3943897428947289472847284";
    const LOGIN_ID = "cf-dcc-3482947289472984728947289";
    private $path, $benutzer;
    public function __construct(string $path, $login_id)
    {
        $this->path = $path;
        $this->benutzer = getUser($login_id);
    }
    public function execute() : string {
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);

        $parser = new CloudPathParser();
        $files = $parser->parse($this->path, $runner);
        if(count($files) === 0) {
            $select = new Select('file');
            $select->append_column('COUNT(id)');
            $select->append_where('parent IS NULL');
            $runner->clear_query();
            $runner->append_query($select);
            $runner->execute();
            return $select->result()[0][0];
        }
        $file = $files[0];
        if( $file instanceof SDirectory) {
            $file->readChildren();
            return (string)count($file->getChildren());
        }
        else if( $file instanceof File ) throw new Exception("File has no children");
        else throw new Exception("Forbidden File Template");
    }
}
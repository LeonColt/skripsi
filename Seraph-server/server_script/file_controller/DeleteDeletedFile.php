<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 03/02/2017
 * Time: 20:35
 */
require_once standart_path.'server_script/file_controller/CloudPathParser.php';
class DeleteDeletedFile
{
    const PATHS = "cf-ddf-34389478294728974824782349";
    const LOGIN_ID = "cf-ddf-348294082903482094829482934";
    private $paths, $benutzer;
    public function __construct(string $paths, string $login_id)
    {
        $this->paths = $paths;
        $this->benutzer = getUser($login_id);

    }
    public function execute() {
        if($this->benutzer === null) throw new Exception("Access Denied");
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $cpp = new CloudPathParser();
        $files = $cpp->parse($this->paths, $runner);
        $delete = new Delete('recyle_bin');
        $delete->append_where('id IN (');
        for($i = 0; $i < count($files); $i++) {
            /* @var RecycleBinObject[] $files */
            if($i > 0) $delete->append_where(',');
            $delete->append_where('?');
            $delete->append_parameter(new Parameter($delete->getParameterVariableIntegerOrder(), $files[$i]->getId()));
        }
        $delete->append_where(')');
        $runner->clear_query();
        $runner->append_query($delete);
        $runner->execute();
        $path = standart_path.'daten/geloscht/';
        foreach($files as $file) {
            /** @var RecycleBinObject $file */
            if(file_exists($path.$file->getId())) unlink($path.$file->getId());
        }
    }
}
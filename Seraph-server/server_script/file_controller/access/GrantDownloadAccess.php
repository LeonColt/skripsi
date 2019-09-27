<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 24/01/2017
 * Time: 20:00
 */
require_once standart_path.'server_script/file_controller/file_object/FileLoader.php';
require_once standart_path.'core/PrivilegeObjectConstant.php';
class GrantDownloadAccess
{
    const LOGIN_ID = "nmlf-gda-430950485430580345835";
    const FILE = "nmlf-gda-3432094890324823048230423";
    const USER_IDS = "nmlf-gda-29348390482348290480294";
    private $file_id, $user_ids, $benutzer;
    public function __construct(string $login_id, int $file_id, string $user_ids)
    {
        $this->file_id = $file_id;
        $this->user_ids = explode(',', $user_ids);
        $this->benutzer = getUser($login_id);
        if( $this->benutzer === null ) throw new Exception("You need to login and must be the owner of the file to grant");
    }
    public function execute() {
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $loader = new FileLoader();
        $files = $loader->load(array($this->file_id), $runner);
        if(count($files) === 0) throw new Exception("File Not Found c: zero");
        $file = $files[0];
        //begin check if login user allowed to grant file download
        if($file instanceof BaseFile) {
            if(!$this->benutzer->compare($file->getOwner()))
                throw new Exception("You are not allowed to grant file download to this file");
        }
        else throw new Exception("File Not Found c: not match");
        //end check

        //
        $runner->clear_query();
        $insert = new Insert('privilege_benutzer_on_object');
        foreach($this->user_ids as $user_id) {
            $user = new User((int)$user_id);
            $insert->append_column_value('benutzer', new Parameter($insert->getParameterVariableIntegerOrder(), $user->getId()));
            $insert->append_column_value('file_id', new Parameter($insert->getParameterVariableIntegerOrder(), $file->getId()));
            $insert->append_column_value('privilege_id', new Parameter($insert->getParameterVariableIntegerOrder(), PrivilegeObjectConstant::DOWNLOAD));
        }
        $runner->append_query($insert);
        $runner->execute();
    }
}
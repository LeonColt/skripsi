<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 02/02/2017
 * Time: 19:11
 */
require_once standart_path.'server_script/file_controller/file_object/FileLoader.php';
require_once standart_path.'server_script/file_controller/FilePrivilegeConstant.php';
class AddUserListOnFilePrivilege
{
    const FILE_ID = "cf-aulofp-39283901809831932";
    const USER_ID_USERNAME_EMAIL = "cf-aulofp-3287489347983247928472";
    const TYPE = "cf-aulofp-38972384972894728947294824";
    const TYPE_SEEN = "cf-aulofp-348293472984782947289472894";
    const TYPE_DOWNLOAD = "cf-aulofp-4827482374274289748247824";
    const TYPE_UPLOAD_ON_DIRECTORY = "cf-aulofp-3472893472894789247284";
    const LOGIN_ID = "cf-aulofp-284237427429847247298474";
    private $file_id, $user_id_username_email, $type, $benutzer;
    public function __construct(int $file_id, $user_id_username_email, string $type, $login_id)
    {
        $this->file_id = $file_id;
        $this->user_id_username_email = $user_id_username_email;
        $this->type = $type;
        $this->benutzer = getUser($login_id);
    }
    public function execute() {
        if($this->benutzer === null) throw new Exception("Access Denied-empty");
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $file = $this->loadFile($runner);
        if(!$this->benutzer->compare($file->getOwner())) throw new Exception("Access Denied");
        $user = $this->loadUser($runner);
        $insert = new Insert('privilege_benutzer_on_object');
        $insert->append_column_value('benutzer', new Parameter($insert->getParameterVariableIntegerOrder(), $user->getId()));
        $insert->append_column_value('file_id', new Parameter($insert->getParameterVariableIntegerOrder(), $file->getId()));
        switch( $this->type ) {
            case AddUserListOnFilePrivilege::TYPE_SEEN : {
                $insert->append_column_value('privilege_id', new Parameter($insert->getParameterVariableIntegerOrder(), FilePrivilegeConstant::SEEN));
            } break;

            case AddUserListOnFilePrivilege::TYPE_DOWNLOAD : {
                $insert->append_column_value('privilege_id', new Parameter($insert->getParameterVariableIntegerOrder(), FilePrivilegeConstant::DOWNLOAD));
            } break;

            case AddUserListOnFilePrivilege::TYPE_UPLOAD_ON_DIRECTORY : {
                $insert->append_column_value('privilege_id', new Parameter($insert->getParameterVariableIntegerOrder(), FilePrivilegeConstant::UPLOAD_ON_FILE));
            } break;
        }
        $runner->clear_query();
        $runner->append_query($insert);
        $runner->execute();
    }

    /**
     * @param Runner $runner
     * @return User
     * @throws Exception
     */
    private function loadUser(Runner &$runner) {
        $select = new Select('benutzer');
        $select->append_column('id');
        $select->append_column('username');
        $select->append_column('email');
        $select->append_where('id = ?');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->user_id_username_email));
        $select->append_where('username = ?', 'OR');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->user_id_username_email));
        $select->append_where('email = ?', 'OR');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->user_id_username_email));
        $select->fetchAssoc();
        $runner->clear_query();
        $runner->append_query($select);
        $runner->execute();
        if(count($select->result()) === 0 ) throw new Exception("User Not Found");
        $result = $select->result()[0];
        $user = new User($result["id"], $result["username"], $result["email"], false);
        return $user;
    }

    /**
     * @param Runner $runner
     * @return BaseFile
     * @throws Exception
     */
    private function loadFile(Runner &$runner) {
        $loader = new FileLoader();
        $files = $loader->load(array($this->file_id), $runner);
        if(count($files) === 0) throw new Exception("File Not Found");
        return $files[0];
    }
}
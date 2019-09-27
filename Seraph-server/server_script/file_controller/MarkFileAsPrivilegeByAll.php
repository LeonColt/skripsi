<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 31/01/2017
 * Time: 00:39
 */
class MarkFileAsPrivilegeByAll
{
    const ID = "cf-mfapba-48948290384902849204";
    const FLAG = "cf-mfapba-34948290480248024924";
    const TYPE = "cf-mfapba-93489274923472947294724";
    const TYPE_SEEN = "cf-mfapba-28389173137913791371";
    const TYPE_DOWNLOAD = "cf-mfapba-823891371731937193";
    const TYPE_MODIFY = "cf-mfapba-988301830183183913";
    const LOGIN_ID = "cf-mfapba-99834934829048924824";
    private $id, $flag, $type, $benutzer;
    public function __construct(int $id, bool $flag, string $type, $login_id)
    {
        $this->id = $id;
        $this->flag = $flag;
        $this->type = $type;
        $this->benutzer = getUser($login_id);
    }
    public function execute() {
        if($this->benutzer === null) throw new Exception("Access Denied");
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $update = new Update('file');
        if(strcasecmp($this->type, MarkFileAsPrivilegeByAll::TYPE_SEEN) === 0)
            $update->apppend_set('seen_by_all', '?');
        else if(strcasecmp($this->type, MarkFileAsPrivilegeByAll::TYPE_DOWNLOAD) === 0)
            $update->apppend_set('download_by_all', '?');
        else if(strcasecmp($this->type, MarkFileAsPrivilegeByAll::TYPE_MODIFY) === 0)
            $update->apppend_set('modify_by_all', '?');
        $update->append_parameter(new Parameter($update->getParameterVariableIntegerOrder(), (($this->flag) ? 1 : 0)));
        $update->append_where("id = ?");
        $update->append_parameter(new Parameter($update->getParameterVariableIntegerOrder(), $this->id));
        $runner->append_query($update);
        $runner->execute();
    }
}
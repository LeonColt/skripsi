<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 24/01/2017
 * Time: 20:17
 */
class GetDownloadAccessOnFile
{
    const USERID = "userid";
    const USERNAME = "username";
    const LOGIN_ID = "nmlf-gdaof-94893284932482938549852394802938423";
    const FILE_ID = "nmlf-gdaof-239849234298428420492";
    const USER_IDS = "nmlf-gdaof-34893589340580345835";
    private  $benutzer, $file_id, $user_id;
    public function __construct($login_id, $file_id, $user_id)
    {
        $this->benutzer = getUser($login_id);
        $this->file_id = $file_id;
        $user_id = explode(",", $user_id);
        $this->user_id = array();
        foreach($user_id as $id) {
            array_push($this->user_id, new User((int)$id));
        }
    }
    public function execute() : array {
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $select = new Select('file');
        $select->append_column('format');
        $select->append_column('seen_by_all');
        $select->append_column('download_by_all');
        $select->append_column('modify_by_all');
        $select->append_where("id = ?");
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->file_id));
        $runner->append_query($select);
        $runner->execute();
        if(count($select->result()) === 0) throw new Exception("File Not Found");
        $ids = array();
        if((int)$select->result()['format'] === 1) {
            $file = new SDirectory();
            $file->setId($this->file_id);
            $file->reveal();
            $info = $file->getSecurityFileInfo();
            for( $i = $info->beginAllowedDownloadUser(); $i !== $info->endAllowedDownloadUser(); $info->nextAllowedDownloadUser($i))
                array_push($ids, array(GetDownloadAccessOnFile::USERID=>$i->getId(), GetDownloadAccessOnFile::USERNAME=>$i->getUsername()));
        }
        else {
            $file = new File();
            $file->setId($this->file_id);
            $file->reveal();
            $info = $file->getSecurityFileInfo();
            for( $i = $info->beginAllowedDownloadUser(); $i !== $info->endAllowedDownloadUser(); $info->nextAllowedDownloadUser($i))
                array_push($ids, array(GetDownloadAccessOnFile::USERID=>$i->getId(), GetDownloadAccessOnFile::USERNAME=>$i->getUsername()));
        }
        return $ids;
    }
}
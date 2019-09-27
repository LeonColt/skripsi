<?php

/**
 * Created by PhpStorm.
 * User: LeonColt
 * Date: 10/26/2016
 * Time: 9:34 PM
 */
require_once 'BaseFile.php';
require_once 'SecurityFileInfo.php';
class SDirectory extends BaseFile
{
    private $iterator;
    private $children;
    private $ender = null;
    public function __construct()
    {
        $this->children = array();
        $this->children_ender = new File();
    }
    public final function &begin() {
        $this->iterator = 0;
        return $this->children[$this->iterator];
    }
    public final function &end() {
        return $this->ender;
    }
    public final function &next(){
        $this->iterator++;
        if($this->iterator < count($this->children)) return $this->children[$this->iterator];
        else return $this->ender;
    }
    public function addChildren($child) {
        array_push($this->children, $child);
    }
    public function setChildren(array $children) {
        $this->children = $children;
    }
    public final function &getChildren() : array {
        return $this->children;
    }
    public final function getChildrenCount() : int {
        return count($this->children);
    }
    public final function readChildren() {
        $this->children = array();
        $select = new Select('file');
        $select->append_column('id');
        $select->append_column('is_directory');
        $select->append_where('parent = ?');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->id));
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $runner->append_query($select);
        $runner->execute();
        foreach($select->result() as $file_db) {
            if((int)$file_db['is_directory'] === 1) {
                $dir = new SDirectory();
                $dir->setId((int)$file_db['id']);
                $dir->reveal();
                $dir->parent = $this;
                array_push($this->children, $dir);
            }
            else {
                $file = new File();
                $file->setId((int)$file_db['id']);
                $file->reveal();
                $file->parent = $this;
                array_push($this->children, $file);
            }
        }
    }
    public function reveal()
    {
        // TODO: Implement reveal() method.
        if(empty($this->id)) return;
        $select = new Select('file');
        $select->append_column('id', 'file', 'file_id');
        $select->append_column('name', 'file', 'file_name');
        $select->append_column('is_directory', 'file', 'directory');
        $select->append_column('owner', 'file', 'file_owner');
        $select->append_column('parent', 'file', 'file_parent');
        $select->append_column('seen_by_all', 'file', 'seen_by_all');
        $select->append_column('download_by_all', 'file', 'download_by_all');
        $select->append_column('modify_by_all', 'file', 'modify_by_all');
        $select->append_column('date_created', 'file', 'date_created');
        $select->append_where('file.id = ?');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->id));
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $runner->append_query($select);
        $runner->execute();
        if((int)$select->result()[0]['directory'] !== 1) throw new Exception("File is not a Directory");
        $this->name = $select->result()[0]['file_name'];
        $this->size = 0;
        $this->owner = ($select->result()[0]['file_owner'] === null) ? null : new User((int)$select->result()[0]['file_owner']);
        $this->parent = $select->result()[0]['file_parent'];
        $this->revealParent();
        $this->security_file_info = new SecurityFileInfo();
        $this->security_file_info->setSeenByAll($select->result()[0]['seen_by_all'] == 1);
        $this->security_file_info->setDownloadByAll($select->result()[0]['download_by_all'] == 1);
        $this->security_file_info->setModifyByAll($select->result()[0]['modify_by_all'] == 1);
        $this->date_created = $select->result()[0]["date_created"];

        if($this->owner === 0 || $this->owner === null) {
            $this->security_file_info->setSeenByAll(true);
            $this->security_file_info->setDownloadByAll(true);
            $this->security_file_info->setModifyByAll(true);
            return;
        }

        $list_able_download = array();
        $list_able_delete = array();
        $list_able_upload = array();
        if($this->owner !== null) array_push($list_able_download, $this->owner);
        if($this->owner !== null) array_push($list_able_delete, $this->owner);
        if($this->owner !== null) array_push($list_able_upload, $this->owner);
        $select = new Select('seraph_privilege_on_object');
        $select->append_column('benutzer');
        $select->append_column('privilege_id');
        $select->append_where('file_id = ?');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->id));
        $runner->clear_query();
        $runner->append_query($select);
        $runner->execute();
        foreach($select->result() as $user) {
            if((int)$user["privilege_id"] === PrivilegeObjectConstant::DOWNLOAD)
                array_push($list_able_download, new User((int)$user["benutzer"]));
            else if((int)$user["privilege_id"] === PrivilegeObjectConstant::UPLOAD_ON_DIRECTORY)
                array_push($list_able_upload, new User((int)$user["benutzer"]));
            else if( (int)$user["privilege_id"] === PrivilegeObjectConstant::DELETE)
                array_push($list_able_delete, new User((int)$user["benutzer"]));
        }
        $this->security_file_info->setAllowedDownloadUser($list_able_download);
        $this->security_file_info->setAllowedDeleteUser($list_able_delete);
        $this->security_file_info->setAllowedUploadOnDirectoryUser($list_able_upload);
    }

    /**
     * @param SDirectory $directory
     */
    public function copyMeta($directory)
    {
        parent::copyMeta($directory); // TODO: Change the autogenerated stub
        $this->children = $directory->children;
    }
}
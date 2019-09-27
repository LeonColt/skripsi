<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 24/01/2017
 * Time: 15:27
 */
class PrivilegeChecker
{
    private $user, $runner;
    public function __construct(User $user)
    {
        $this->user = $user;
        $this->runner = new Runner();
        $this->runner->connect(host, port, database, database_username, database_password);
    }
    public function canDo(int $do, $input) : bool {
        switch($do) {
            case PrivilegeConstant::LOGIN: return $this->canLogin();
            case PrivilegeConstant::LOGIN_AS_ADMIN: return $this->canLoginAsAdmin();
            case PrivilegeConstant::DELETE_FILE: return $this->candeleteFile($input);
            case PrivilegeConstant::DOWNLOAD_FILE: return $this->canDownloadFile($input);
            case PrivilegeConstant::UPLOAD_FILE: return $this->canUploadFile($input);
        }
        return false;
    }
    private final function canUploadFile($input) : bool {

    }
    private final function canDownloadFile($input) : bool {
        if($input instanceof BaseFile) {
            if($input->getOwner()->compare($this->user)) return true;
            else return false;
        }
        else if( is_array($input)) {
            foreach($input as $item) {
                $res = $this->candeleteFile($item);
                if(!$res) return false;
            }
            return true;
        }
        return false;
    }
    /*
     * support BaseFile
     * support BaseFile Array
     */
    private final function candeleteFile($input) : bool {
        if($input instanceof BaseFile) {
            if($input->getOwner()->compare($this->user)) return true;
            else return false;
        }
        else if( is_array($input)) {
            foreach($input as $item) {
                $res = $this->candeleteFile($item);
                if(!$res) return false;
            }
            return true;
        }
        return false;
    }
    private final function canLoginAsAdmin() : bool {
        if($this->user->isSuperAdmin() || $this->user->isAdmin()) return true;
        else if($this->user->isMember()) return false;
        else {
            $select = new Select('privilege_benutzer');
            $select->append_column('1');
            $select->append_where('benutzer = ?');
            $select->append_where('privilege_id = ?', 'AND');
            $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->user->getId()));
            $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), PrivilegeConstant::LOGIN_AS_ADMIN));
            $this->runner->clear_query();
            $this->runner->append_query($select);
            $this->runner->execute();
            return count($select->result()) > 0;
        }
    }
    private final function canLogin() : bool {
        if($this->user->isSuperAdmin() || $this->user->isAdmin() || $this->user->isMember()) return true;
        else {
            $select = new Select('privilege_benutzer');
            $select->append_column('1');
            $select->append_where('benutzer = ?');
            $select->append_where('privilege_id = ?', 'AND');
            $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->user->getId()));
            $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), PrivilegeConstant::LOGIN));
            $this->runner->clear_query();
            $this->runner->append_query($select);
            $this->runner->execute();
            return count($select->result()) > 0;
        }
    }
}
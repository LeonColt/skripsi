<?php

/**
 * Created by PhpStorm.
 * User: LeonColt
 * Date: 10/27/2016
 * Time: 11:14 AM
 */
final class SecurityFileInfo
{
    private $pointer;

    private $seen_by_all, $download_by_all, $modify_by_all;
    private $allowed_download_user;
    private $allowed_upload_on_directory_user;
    private $allowed_delete_user;
    public final function setSeenByAll(bool $seen_by_all) {$this->seen_by_all = $seen_by_all;}
    public final function isSeenByAll() : bool {return $this->seen_by_all;}
    public final function setDownloadByAll(bool $download_by_all){$this->download_by_all = $download_by_all;}
    public final function isDownloadByAll() : bool {return $this->download_by_all;}
    public final function setModifyByAll(bool $modify_by_all) {$this->modify_by_all = $modify_by_all;}
    public final function isModifyByAll() : bool {return $this->modify_by_all;}
    public final function setAllowedDownloadUser(array $allowed_download_user) {$this->allowed_download_user = $allowed_download_user;}
    public final function setAllowedUploadOnDirectoryUser(array $allowed_upload_on_directory_user) {$this->allowed_upload_on_directory_user = $allowed_upload_on_directory_user;}
    public final function setAllowedDeleteUser(array $allowed_delete_user) {$this->allowed_delete_user = $allowed_delete_user;}
    public final function beginAllowedDownloadUser() {
        if( count($this->allowed_download_user) == 0) return null;
        $this->pointer = 0;
        return $this->allowed_download_user[$this->pointer];
    }
    public final function endAllowedDownloadUser() {return null;}
    public final function nextAllowedDownloadUser(User &$iterator) {
        if( $this->pointer < count($this->allowed_download_user) )
            $iterator = $this->allowed_download_user[$this->pointer++];
        else $iterator = null;
    }

    /**
     * @return User
     */
    public final function beginAllowedUploadOnDirectoryUser() {
        if( count($this->allowed_upload_on_directory_user) == 0) return null;
        $this->pointer = 0;
        return $this->allowed_upload_on_directory_user[0];
    }

    /**
     * @return User
     */
    public final function  endAllowedUploadOnDirectoryUser() {
        return null;
    }

    /**
     * @param User $iterator
     */
    public final function nextAllowedUploadOnDirectoryUser(User &$iterator) {
        if($this->pointer < count($this->allowed_upload_on_directory_user))
            $iterator = $this->allowed_upload_on_directory_user[$this->pointer++];
        else $iterator = null;
    }

    public final function beginAllowedDeleteUser() {
        if( count($this->allowed_delete_user) == 0) return null;
        $this->pointer = 0;
        return $this->allowed_delete_user[0];
    }
    public final function endAllowedDeleteUser() {
        return null;
    }
    public final function nextAllowedDeleteUser(User &$iterator) {
        if( $this->pointer < count($this->allowed_delete_user) )
            $iterator = $this->allowed_delete_user[$this->pointer++];
        else $iterator = null;
    }

    /**
     * @param User $user
     * @return bool
     */
    public final function isAllowedToUploadOnDirectory($user = null) : bool {
        if($this->isModifyByAll()) return true;
        else if($user === null) return false;
        if($user instanceof User) {
            for($i = $this->beginAllowedUploadOnDirectoryUser(); $i !== $this->endAllowedUploadOnDirectoryUser(); $this->nextAllowedUploadOnDirectoryUser($i)) {
                if($user->compare($i)) return true;
            }
        }
        return false;
    }
    /**
     * @param User $user
     * @return bool
     */
    public final function isAllowedToDownload($user = null) : bool {
        if($this->isDownloadByAll()) return true;
        else if( $user === null) return false;
        if($user instanceof User) {
            if( $user->isAdmin() || $user->isSuperAdmin() ) return true;
            for($i = $this->beginAllowedDownloadUser(); $i !== $this->endAllowedDownloadUser(); $this->nextAllowedDownloadUser($i)) {
                if($user->compare($i)) return true;
            }
        }
        return false;
    }

    /**
     * @param User $user
     * @return bool
     */
    public final function isAllowedToDelete($user = null) : bool {
        if($user instanceof User) {
            if( $user->isAdmin() || $user->isSuperAdmin() ) return true;
            for($i = $this->beginAllowedDeleteUser(); $i !== $this->endAllowedDeleteUser(); $this->nextAllowedDeleteUser($i)) {
                if($user->compare($i)) return true;
            }
        }
        return false;
    }
}
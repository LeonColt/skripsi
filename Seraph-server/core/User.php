<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 18/01/2017
 * Time: 17:39
 */
class User
{
    const SUPERADMIN = 1;
    const ADMIN = 2;
    const MEMBER = 3;
    private $id, $username, $email;
    private $privilege_group;
    public function __construct(int $id, $username = null, $email = null, int $privilege_group = NULL, bool $check = true)
    {
        $this->id = $id;
        if( $username !== null) $this->username = $username;
        if( $email !== null) $this->email = $email;
        $this->privilege_group = $privilege_group;
        if($check) {
            $select = new Select('benutzer');
            $select->append_column('username', 'benutzer', "benutzername");
            $select->append_column('email', 'benutzer', "benutzer_email");
            $select->append_column('privilege_group_id', 'privilege_group', 'privilege_group_id');
            $select->append_column('group_name', 'privilege_group', 'privilege_group_name');
            $select->append_join('LEFT JOIN', 'privilege_group_member');
            $select->append_on('privilege_group_member', 'benutzer', '=', 'benutzer', 'id');
            $select->append_join('LEFT JOIN', 'privilege_group');
            $select->append_on('privilege_group_member', 'group_id', '=', 'privilege_group', 'privilege_group_id');
            $select->append_where("benutzer.id = ?");
            $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->id));
            $select->fetchAssoc();
            $runner = new Runner();
            $runner->connect(host, port, database, database_username, database_password);
            $runner->append_query($select);
            $runner->execute();
            if(count($select->result()) != 1) throw new Exception("User Not Found with id : ".$this->id );
            $this->username = $select->result()[0]["benutzername"];
            $this->email = $select->result()[0]["benutzer_email"];
            $this->privilege_group = (int)$select->result()[0]["privilege_group_id"];
        }
    }
    public function getId() : int {return $this->id;}
    public function getUsername() : string {return $this->username;}
    public function getEmail() : string {return $this->email;}
    public final function isSuperAdmin() : bool {
        return $this->privilege_group === User::SUPERADMIN;
    }
    public final function isAdmin() : bool {
        return $this->privilege_group === User::ADMIN;
    }
    public final function isMember() : bool {
        return $this->privilege_group === User::MEMBER;
    }
    public final function getGroupPrivilege() : int {
        return $this->privilege_group;
    }

    /**
     * @param User $user
     * @return bool
     */
    public final function compare($user) : bool {
        return ($user === null) ? false : ($this->id === $user->getId());
    }
}
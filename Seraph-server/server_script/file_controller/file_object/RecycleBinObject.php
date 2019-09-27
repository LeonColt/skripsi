<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 28/01/2017
 * Time: 17:34
 */
class RecycleBinObject
{
    private $id, $name, $owner, $parent, $parent_undeleted, $original, $directory, $children, $date_deleted;

    /**
     * @return string
     */
    public function getDateDeleted() : string
    {
        return $this->date_deleted;
    }

    /**
     * @param string $date_deleted
     */
    public function setDateDeleted(string $date_deleted)
    {
        $this->date_deleted = $date_deleted;
    }

    private $pointer;

    /**
     * @return bool
     */
    public function isDirectory() : bool{return $this->directory;}

    /**
     * @param mixed $directory
     */
    public function setAsDirectory(bool $directory){$this->directory = $directory;}

    /**
     * @return int
     */
    public function getId() : int
    {
        return $this->id;
    }

    /**
     * @param int $id
     */
    public function setId(int $id)
    {
        $this->id = $id;
    }

    /**
     * @return string
     */
    public function getName() : string
    {
        return $this->name;
    }

    /**
     * @param string $name
     */
    public function setName(string $name)
    {
        $this->name = $name;
    }

    /**
     * @return User
     */
    public function getOwner()
    {
        return $this->owner;
    }

    /**
     * @param User $owner
     */
    public function setOwner( $owner)
    {
        $this->owner = $owner;
    }

    /**
     * @return RecycleBinObject
     */
    public function getParent()
    {
        return $this->parent;
    }

    /**
     * @param RecycleBinObject $parent
     */
    public function setParent($parent)
    {
        $this->parent = $parent;
    }

    /**
     * @return SDirectory
     */
    public function getParentUndeleted()
    {
        return $this->parent_undeleted;
    }

    /**
     * @param SDirectory $parent_undeleted
     */
    public function setParentUndeleted($parent_undeleted)
    {
        $this->parent_undeleted = $parent_undeleted;
    }

    /**
     * @return bool
     */
    public function getOriginal() : bool
    {
        return $this->original;
    }

    /**
     * @param bool $original
     */
    public function setOriginal(bool $original)
    {
        $this->original = $original;
    }
    /**
     * @return bool
     */
    public function hasChildren() : bool {
        if($this->children !== null) {
            return count($this->children) > 0;
        }
        return false;
    }

    /**
     * @return RecycleBinObject
     */
    public function begin() {
        if($this->hasChildren()) {
            $this->pointer = 0;
            return $this->children[$this->pointer++];
        }
        return null;
    }

    /**
     * @return RecycleBinObject
     */
    public function end() {
        return null;
    }

    /**
     * @param RecycleBinObject $iterator
     */
    public function next(&$iterator) {
        if($this->pointer < count($this->children)) $iterator = $this->children[$this->pointer++];
        else $iterator = null;
    }
    public function revealParent(Runner $runner = null) {
    }
    /**
     * @param Runner $runner
     */
    public function readChildren(Runner $runner = null) {
        $select = new Select('recyle_bin');
        $select->append_column('id', 'recyle_bin', 'id');
        $select->append_column('file_name');
        $select->append_column('is_directory');
        $select->append_column('id', 'benutzer', 'owner_id');
        $select->append_column('username', 'benutzer', 'owner_username');
        $select->append_column('email', 'benutzer', 'owner_email');
        $select->append_column('parent');
        $select->append_column('parent_undeleted');
        $select->append_column('original');
        $select->append_column('date_deleted');
        $select->append_join('JOIN', 'benutzer');
        $select->append_on('recyle_bin', 'file_owner', '=', 'benutzer', 'id');
        $select->append_where('parent = ?');
        $select->append_parameter(new Parameter($select->getBinderVariableIntegerOrder(), $this->id));
        if($runner === null) {
            $runner = new Runner();
            $runner->connect(host, port, database, database_username, database_password);
        } else $runner->clear_query();
        $runner->clear_query();
        $runner->append_query($select);
        $runner->execute();
        if(count($select->result()) === 0) return;
        foreach($select->result() as $item) {
            $bin = new RecycleBinObject();
            $bin->setId((int)$item['id']);
            $bin->setName($item["file_name"]);
            $bin->setAsDirectory((((int)$item['is_directory'] === 1) ? true : false));
            $bin->setOwner(new User($item['owner_id'], $item['owner_username'], $item['owner_email'], NULL, false));
            if($item['parent'] !== null) $bin->setParent($this);
            else if($item["parent_undeleted"] !== null) {
                $parent = new SDirectory();
                $parent->setId($item["parent"]);
                $parent->reveal();
                $bin->setParentUndeleted($parent);
            }
            $bin->setOriginal($item["original"]);
            $bin->setDateDeleted($item["date_deleted"]);
        }
    }
    public function reveal(Runner $runner = null) {
        $select = new Select('recyle_bin');
        $select->append_column('id', 'recyle_bin', 'id');
        $select->append_column('file_name');
        $select->append_column('is_directory');
        $select->append_column('id', 'benutzer', 'owner_id');
        $select->append_column('username', 'benutzer', 'owner_username');
        $select->append_column('email', 'benutzer', 'owner_email');
        $select->append_column('parent');
        $select->append_column('parent_undeleted');
        $select->append_column('original');
        $select->append_column('date_deleted');
        $select->append_join('JOIN', 'benutzer');
        $select->append_on('recyle_bin', 'file_owner', '=', 'benutzer', 'id');
        $select->append_where('recyle_bin.id = ?');
        if($runner === null) {
            $runner = new Runner();
            $runner->connect(host, port, database, database_username, database_password);
        }else $runner->clear_query();
        $runner->clear_query();
        $runner->append_query($select);
        $runner->execute();
        if(count($select->result()) === 0) throw new Exception("Bin Not Found");
        $item = $select->result()[0];
        $this->setId((int)$item['id']);
        $this->setName($item["file_name"]);
        $this->setAsDirectory((((int)$item['is_directory'] === 1) ? true : false));
        if($item['parent'] !== null) {
            $parent = new RecycleBinObject();
            $parent->setId((int)$item["parent_undeleted"]);
            $parent->reveal();
            $this->setParent($parent);
        }
        else if($item["parent_undeleted"] !== null) {
            $parent = new SDirectory();
            $parent->setId($item["parent"]);
            $parent->reveal();
            $this->setParentUndeleted($parent);
        }
        $this->setOriginal($item["original"]);
        $this->setDateDeleted($item["date_deleted"]);
        $this->setOwner(new User($item['owner_id'], $item['owner_username'], $item['owner_email'], NULL, false));
    }
}
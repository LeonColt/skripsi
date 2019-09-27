<?php

/**
 * Created by PhpStorm.
 * User: LeonColt
 * Date: 10/26/2016
 * Time: 9:35 PM
 */
require_once standart_path.'core/PrivilegeObjectConstant.php';
require_once standart_path.'core/User.php';
abstract class BaseFile
{
    protected $id, $name, $size, $parent, $owner, $security_file_info, $date_created;
    protected $path;

    /**
     * @return mixed
     */
    public function getPath() : string
    {
        return $this->path;
    }

    /**
     * @param mixed $path
     */
    public function setPath(string $path)
    {
        $this->path = $path;
    }

    /**
     * @return string
     */
    public function getDateCreated() : string {return $this->date_created;}
    /**
     * @param string $date_created
     */
    public function setDateCreated(string $date_created){$this->date_created = $date_created;}
    public final function setId(string $id){
        $this->id = $id;
    }
    public function getId() : int {
        return (int)$this->id;
    }
    public function getName() : string {
        return $this->name;
    }
    public function setName(string $name) {
        $this->name = $name;
    }
    public function getSize() {
        return $this->size;
    }
    public function setSize(int $size) {
        $this->size = $size;
    }
    /**
     * @return BaseFile
     */
    public function getParent() {
        return $this->parent;
    }

    /**
     * @param BaseFile $parent
     */
    public function setParent($parent) {
        $this->parent = $parent;
    }

    /**
     * @return User
     */
    public function getOwner() {
        return $this->owner;
    }
    public function setOwner(User $owner) {
        $this->owner = $owner;
    }

    /**
     * @return SecurityFileInfo
     */
    public final function getSecurityFileInfo() : SecurityFileInfo {
        return $this->security_file_info;
    }

    /**
     * @param SecurityFileInfo $security_file_info
     */
    public function setSecurityFileInfo($security_file_info)
    {
        $this->security_file_info = $security_file_info;
    }

    /**
     * @param BaseFile $baseFile
     */
    public function copyMeta($baseFile) {
        $this->id = $baseFile->id;
        $this->name = $baseFile->name;
        $this->size = $baseFile->size;
        $this->parent = $baseFile->parent;
        $this->owner = $baseFile->owner;
        $this->security_file_info = $baseFile->security_file_info;
    }
    abstract public function reveal();
    public final function revealParent()
    {
        // TODO: Implement revealParent() method.
        if($this->parent instanceof SDirectory || $this->parent == null) return;
        $select = new Select('file');
        $select->append_column('id');
        $select_parent = new Select('file');
        $select_parent->append_column('parent');
        $select_parent->append_where('id = ?');
        $select->append_where('id = ('.$select_parent->query().')');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->id));
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $runner->append_query($select);
        $runner->execute();

        if(count($select->result()) > 0) {
            $this->parent = new SDirectory();
            $this->parent->setId($select->result()[0][0]);
            $this->parent->reveal();
        }
    }
}
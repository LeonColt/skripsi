<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 10/01/2017
 * Time: 11:27
 */
require_once '../../core/global_variable.php';
require_once '../../core/database/Runner.php';
require_once standart_path.'server_script/User.php';
require_once standart_path.'core/file/FileHandler.php';
require_once standart_path.'server_script/chat/Message.php';
class Chat
{
    private $fh;
    private $left, $right;
    public function __construct()
    {
    }
    public function load(string $path) {
        $fh_member = new FileHandler($path."/member", "r+b");
        $this->fh = new FileHandler($path."/data", "r+b");
        $temp = array();
        for($i = $fh_member->beginLine(); $i !== $fh_member->endLine(); $fh_member->nextLine($i)) {
            $i = preg_replace("/\s+/", "", trim($i));
            if(!is_numeric($i)) throw new Exception("Invalid User ID");
            $user = new User((int)$i);
            array_push($temp, $user);
        }
        $this->left = $temp[0];
        $this->right = $temp[1];
    }
    public function getLeft() : User {return $this->left;}
    public function getRight() : User {return $this->right;}
    public function begin() {
        if($this->fh instanceof FileHandler) {
            $temp = $this->fh->beginLine();
            if( $temp === $this->fh->endLine() ) return null;
            $temp = json_decode($temp, true);
            if(count($temp) != 2) return null;
            return new Message($temp[Message::PUBLISHER], $temp[Message::PUBLISHED], $temp[Message::TIME], null, null, null);
        }
        throw new Exception("Chat has not been loaded or created");
    }
    public function end() {return null;}
    public function next(&$iterator){
        if($this->fh instanceof FileHandler) {
            $temp = FALSE;
            $this->fh->nextLine($temp);
            if( $temp === $this->fh->endLine() ) $iterator = null;
            $temp = json_decode($temp, true);
            if(count($temp) != 2) return null;
            $iterator = new Message($temp[Message::PUBLISHER], $temp[Message::PUBLISHED], $temp[Message::TIME], null, null, null);
        } else throw new Exception("Chat has not been loaded or created");
    }
    public function rbegin() {
        if($this->fh instanceof FileHandler) {
            $temp = $this->fh->rbeginLine(-2, true);
            if( $temp == $this->fh->rendLine() ) return null;
            $temp = json_decode($temp, true);
            if(count($temp) != 2) return null;
            return new Message($temp[Message::PUBLISHER], $temp[Message::PUBLISHED], $temp[Message::TIME], null, null, null);
        }
        throw new Exception("Chat has not been loaded or created");
    }
    public function rend() {return null;}
    public function rnext(&$iterator){
        if($this->fh instanceof FileHandler) {
            $temp = FALSE;
            $this->fh->rnextLine($temp);
            if( $temp == $this->fh->rendLine() ) $iterator = null;
            $temp = json_decode($temp, true);
            if(count($temp) != 2) return null;
            $iterator = new Message($temp[Message::PUBLISHER], $temp[Message::PUBLISHED], $temp[Message::TIME], null, null, null);
        } else throw new Exception("Chat has not been loaded or created");
    }
    public function push($data) {
        if($this->fh instanceof FileHandler) {
            $this->fh->pushLine($data);
        } else throw new Exception("Chat has not been loaded or created");
    }
}
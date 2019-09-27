<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 10/01/2017
 * Time: 11:28
 */
require_once '../../core/global_variable.php';
require_once 'Chat.php';
class ChatSeeker
{
    const PATH = standart_path."daten/chat";
    public function seekByMemberId( int $member_id ) : array {
        $is = new DirectoryIterator(ChatSeeker::PATH);
        $temp = array();
        foreach($is as $i) {
            if(strcasecmp($i->getFilename(), ".") == 0
                || strcasecmp($i->getFilename(), "..") == 0 || strcasecmp($i->getFilename(), "chat_group") == 0 ) continue;
            $chat = new Chat();
            $chat->load($i->getPathname());
            array_push($temp, $chat);
        }
        return $temp;
    }
}

$exec = new ChatSeeker();
foreach($exec->seekByMemberId(1) as $item) {
    if($item instanceof Chat) {
        for($i = $item->rbegin(); $i !== $item->rend(); $item->rnext($i)) {
            var_dump($i);
        }
    }
}
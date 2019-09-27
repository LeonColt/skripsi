<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 22/01/2017
 * Time: 15:50
 */
abstract class PageBuilder
{

    public function show() {
        echo "<!DOCTYPE HTML><html>";
        echo "<head>";
        $this->head();
        echo "</head>";
        echo "<body>";;
        $this->body();
        echo "</body>";
        echo "</html>";
    }
    protected abstract function body();
    protected abstract function head();
}
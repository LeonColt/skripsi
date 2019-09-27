<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 22/01/2017
 * Time: 19:17
 */
class Navigator
{
    public function show() {
        $this->previous();
        echo "<li>";
        echo "<a href=\"#\" id=\"menu2\" class='main_menu'>Menu1</a>";
        echo "</li>";
        echo "<li>";
        echo "<a href=\"#\" id=\"menu3\" class='main_menu'>Menu2</a>";
        echo "</li>";
        echo "<li class='dropdown'>";
        echo "<a href=\"javascript:void(0)\" id=\"menu4\" class='dropbtn main_menu'>Menu3</a>";
        echo "<div class='dropdown-content'>";
        echo "<a href=\"#\" class='sub_menu'>Link 1</a>";
        echo "</div>";
        echo "</li>";
        $this->next();
    }
    private final function menuLogin() {
    }
    private final function menuPendaftaran() {

    }
    private final function previous() {
        echo "<li>";
        echo "<a href=\"#\" id=\"menu1\" class='main_menu'>Previous</a>";
        echo "</li>";
    }
    private final function next() {
        echo "<li>";
        echo "<a href=\"#\" id=\"menu5\" class='main_menu'>Next</a>";
        echo "</li>";
    }
}
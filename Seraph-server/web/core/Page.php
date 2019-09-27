<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 22/01/2017
 * Time: 18:11
 */
require_once standart_path.'core/PageBuilder.php';
require_once standart_path.'web/core/Navigator.php';
abstract class Page extends PageBuilder
{
    protected function head()
    {
        // TODO: Implement head() method.
        echo "<meta charset=\"UTF-8\">";
        echo "<title>";
        $this->title();
        echo "</title>";
        $this->link();
        $this->script();
    }
    protected function header() {
        echo "<div>";
        echo "<div>";
        echo "<a href=\"";
        echo "?".var_request."=".Request::HOME;
        echo "\" class=\"logo\"><img src=\"images/logo.png\" alt=\"\"></a>";
        $this->navigation();
        echo "</div>";
        echo "</div>";
    }
    protected abstract function bodyPage();
    protected function footer() {
        echo "<div>";
        echo "<ul>
                        <li id=\"facebook\">
                            <a href=\"http://freewebsitetemplates.com/go/facebook/\">facebook</a>
                        </li>
                        <li id=\"twitter\">
                            <a href=\"http://freewebsitetemplates.com/go/twitter/\">twitter</a>
                        </li>
                        <li id=\"googleplus\">
                            <a href=\"http://freewebsitetemplates.com/go/googleplus/\">googleplus</a>
                        </li>
                    </ul>";
        echo "<p>";
        $start = 2017;
        $current = (int)date('Y');

        echo "@ copyright ";
        if($current - $start === 0) echo $start;
        else echo $current." - ".$start;
        echo ". all rights reserved.";
        echo "</p>";
        echo "</div>";
    }
    protected function body()
    {
        // TODO: Implement body() method.
        echo "<div id=\"background\">";

        //begin header
        echo "<div id=\"header\">";
        $this->header();
        echo "</div>";
        //end header

        //begin body
        echo "<div id=\"body\">";
        $this->bodyPage();
        echo "</div>";
        //end body

        //begin footer
        echo "<div id=\"footer\">";
        $this->footer();
        echo "</div>";
        //end footer

        echo "</div>";
    }
    protected function navigation(){
        echo "<ul>";
        $navigator = new Navigator();
        $navigator->show();
        echo "</ul>";
    }
    public function title() {
        echo "Seraph";
    }
    public function link() {
        echo "<link rel=\"stylesheet\" href=\"css/style.css\" type=\"text/css\">";
    }
    public function script() {}
}
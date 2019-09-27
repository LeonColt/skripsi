<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 22/01/2017
 * Time: 18:30
 */
require_once 'core/Page.php';
class Home extends Page
{
    protected function bodyPage()
    {
        // TODO: Implement bodyPage() method.
        echo "<div>
                    <div>
                        <div class=\"featured\">
                            <img src=\"images/featured-game.png\" alt=\"\">
                        </div>
                        <div class=\"section\">
                            <div>
                                <h3>This is just a place holder</h3>
                                <div>
                                    <p>
                                        This is just a place holder, so you can see what the site would look like. You can replace all this text with your own text. You can remove any link to our website from this website template, you're free to use this website template without linking back to us. You can replace all this text with your own text. You can remove any link to our website from this website template, you're free to use this website template <a href=\"blog.html\" class=\"more\">read more</a>
                                    </p>
                                </div>
                            </div>
                            <div>
                                <h3>games</h3>
                                <div>
                                    <a href=\"games.html\" class=\"figure\"><img src=\"images/games.jpg\" alt=\"\"></a>
                                    <span><a href=\"games.html\">This is just a place holder</a></span>
                                </div>
                            </div>
                            <div>
                                <h3>media</h3>
                                <div>
                                    <a href=\"media.html\" class=\"figure\"><img src=\"images/media.jpg\" alt=\"\"></a>
                                    <span><a href=\"media.html\">This is just a place holder</a></span>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>";
    }
}
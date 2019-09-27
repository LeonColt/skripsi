<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 20/12/2016
 * Time: 17:39
 */
class UploadFiles
{
    public final function has($key) {
        return in_array($key, $_FILES);
    }
}
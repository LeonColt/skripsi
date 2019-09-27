<?php 
require_once 'global_variable.php';
header("HTTP/1.0 404 Not Found - command not found", true, 404);
                        require standart_path."404.html";
                        exit();
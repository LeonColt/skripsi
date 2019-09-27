<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 18/01/2017
 * Time: 18:15
 */
class Message
{
    const PUBLISHER = "publisher";
    const PUBLISHED = "published";
    const TIME = "time";
    private $publisher, $published;
    private $time;
    private $image_path, $video_path, $file_path;
    public function __construct(int $publisher, string $published, int $time, string $image_path, string $video_path, string $file_path)
    {
        $this->publisher = $publisher;
        $this->published = $published;
        $this->time = $time;
        $this->image_path = $image_path;
        $this->video_path = $video_path;
        $this->file_path = $file_path;
    }

    public function getPublished() : string {return $this->published;}
    public function getPublisher() : User {return $this->publisher;}
    public function getTime() : int {return $this->time;}
    public function getImage() : FileHandler {return new FileHandler($this->image_path, "rb");}
    public function getVideo() : FileHandler {return new FileHandler($this->video_path, "rb");}
    public function getImagePath() : string{return $this->image_path;}
    public function getVideoPath() : string{return $this->video_path;}
}
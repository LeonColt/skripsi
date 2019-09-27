<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 06/07/2016
 * Time: 14:42
 */
class FileUpdate
{
    private $id, $nama, $format;
    public function __construct(int $id, string $nama, int $format)
    {
        $this->id = $id;
        $this->format = $format;
        $this->nama = $nama;
    }

    public function execute()
    {
        $update = new Update('file');
        $update->apppend_set('nama', '?');
        $update->apppend_set('format', '?');
        $update->append_where('id=?');
        $update->append_parameter($this->nama);
        $update->append_parameter($this->format);
        $update->append_parameter($this->id);
    }
}
<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 21/12/2016
 * Time: 15:29
 */
require_once standart_path.'server_script/file_controller/file_object/RecycleBinObject.php';
class GetRecycleBin
{
    const INPUT_TIME = "cf-grb-93080328385495305839485";
    const LOGIN_ID = "cf-grb-3948092489024";

    const GET_RECYCLE_BIN_ID = "cf-grb-4343254543534424234"; //string then cast to UINT64
    const GET_RECYCLE_BIN_NAME = "cf-grb-3435465643534343224"; //string
    const GET_RECYCLE_BIN_DIRECTORY = "cf-grb-9483209482904824"; //bool
    const GET_RECYCLE_BIN_SIZE = "cf-grb-3843420948490380"; //string then cast to UINT64
    const GET_RECYCLE_BIN_OWNER = "cf-grb-83904809238427487429824"; //string
    const GET_RECYCLE_BIN_DATE_DELETED = "cf-grb-394832904892048204294"; //string
    private $login_id, $benutzer, $time;
    public function __construct(int $time, $login_id) {
        $this->time = $time;
        $this->login_id = $login_id;
        $this->benutzer = getUser($login_id);
    }

    public function execute() : array {
        if($this->benutzer === null ) throw new Exception("Access Denied");
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);

        if((int)$this->time === -1) return $this->prepareFile($this->getFile($runner));
        else
        {
            //set_time_limit(0); // set limit waktu tak terbatas
            $sleep_time = 1; //tick every seconds
            $total_sleep_max = 5; //15 menit
            //$total_sleep_max = 900; //15 menit
            $sleep_time_total = 0;
            for(;;)
            {
                //echo "\0";
                //if( connection_aborted()) break;
                if($this->isThereAnyUpdate($runner)) return $this->prepareFile($this->getFile($runner));
                else {
                    sleep($sleep_time);
                    $sleep_time_total += $sleep_time;
                    if($sleep_time_total > ($total_sleep_max -1)) return $this->prepareFile($this->getFile($runner));
                }
            }
        }
        return array();
    }

    private function prepareFile(array $files) : array {
        $result = array();
        foreach( $files as $file) {
            /* @var $file RecycleBinObject */
            $temp = array();
            $temp[GetRecycleBin::GET_RECYCLE_BIN_ID] = (string)$file->getId();
            $temp[GetRecycleBin::GET_RECYCLE_BIN_NAME] = $file->getName();
            $temp[GetRecycleBin::GET_RECYCLE_BIN_DIRECTORY] = $file->isDirectory();
            if($file->isDirectory()) $temp[GetRecycleBin::GET_RECYCLE_BIN_SIZE] = "0";
            else $temp[GetRecycleBin::GET_RECYCLE_BIN_SIZE] = (string)((file_exists(standart_path.'daten/'.($file->getId()))) ? filesize(standart_path.'daten/'.($file->getId())) : "0");
            $temp[GetRecycleBin::GET_RECYCLE_BIN_OWNER] = $file->getOwner()->getUsername();
            $temp[GetRecycleBin::GET_RECYCLE_BIN_DATE_DELETED] = $file->getDateDeleted();
            array_push($result, $temp);
        }
        return $result;
    }

    private final function isThereAnyUpdate(Runner &$runner) : bool {
        $select = new Select('recyle_bin');
        $select->append_column('1');
        $select->append_where("file_owner = ?");
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->benutzer->getId()));
        $select->append_where(" UNIX_TIMESTAMP(date_deleted) > ?", 'AND');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->time));
        $select->append_where("original = 1", 'AND');
        $runner->clear_query();
        $runner->append_query($select);
        $runner->execute();
        return count($select->result()) > 0;
    }
    private function getFile(Runner $runner) : array {
        $select = new Select('recyle_bin');
        $select->append_column('id', 'recyle_bin', 'id');
        $select->append_column('file_name');
        $select->append_column('is_directory');
        $select->append_column('id', 'benutzer', 'owner_id');
        $select->append_column('username', 'benutzer', 'owner_username');
        $select->append_column('email', 'benutzer', 'owner_email');
        $select->append_column('parent');
        $select->append_column('parent_undeleted');
        $select->append_column('original');
        $select->append_column('date_deleted');
        $select->append_join('JOIN', 'benutzer');
        $select->append_on('recyle_bin', 'file_owner', '=', 'benutzer', 'id');
        $select->append_where("recyle_bin.original = 1");
        $select->append_where("recyle_bin.file_owner = ?", 'AND');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->benutzer->getId()));
        $runner->clear_query();
        $runner->append_query($select);
        $runner->execute();
        $res = array();
        foreach($select->result() as $item) {
            $bin = new RecycleBinObject();
            $bin->setId((int)$item['id']);
            $bin->setName($item["file_name"]);
            $bin->setAsDirectory((((int)$item['is_directory'] === 1) ? true : false));
            $bin->setOwner(new User($item['owner_id'], $item['owner_username'], $item['owner_email'], NULL, false));
            //$bin->setParent($this);
            $bin->setOriginal($item["original"]);
            $bin->setDateDeleted($item["date_deleted"]);
            array_push($res, $bin);
        }
        return $res;
    }
}
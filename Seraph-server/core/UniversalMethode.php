<?php
/**
 * Created by PhpStorm.
 * User: LeonColt
 * Date: 11/27/2016
 * Time: 4:48 PM
 */

if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
/**
 * @param string $login_id
 * @return null|User
 * @throws Exception
 */
function getUser($login_id = null) {
    $session = new Session();
    $session->startSession();
    if($session->isContainByKey(session_login))
    {
        $session_login = unserialize($session->getSessionByKey(session_login));
        if($session_login instanceof Session_login)
        {
            if ($session_login->isSessionCredebilityCorrect($login_id, null, null) )
                return $session_login->getUser();
        }
    }
    return null;
}

function convertBytes( $value ) {
    if ( is_numeric( $value ) ) {
        return $value;
    } else {
        $value_length = strlen($value);
        $qty = substr( $value, 0, $value_length - 1 );
        $unit = strtolower( substr( $value, $value_length - 1 ) );
        switch ( $unit ) {
            case 'k':
                $qty *= 1024;
                break;
            case 'm':
                $qty *= 1048576;
                break;
            case 'g':
                $qty *= 1073741824;
                break;
        }
        return $qty;
    }
}

function getMaxUploadFileSize() {
    return (int)convertBytes(ini_get('upload_max_filesize'));
}

function getMaxMysqlFileChunked() {
    require_once standart_path.'core/database/Runner.php';
    $runner = new Runner();
    $runner->connect(host, port, database, database_username, database_password);
    $runner->append_query("SHOW VARIABLES LIKE 'max_allowed_packet';", null, null, Runner::QUERY_SELECT);
    $runner->execute();
    return (int)$runner->result($runner->get_default_key())[0][0][1];
}

function getMaxInnoDbLogFileTenPercent() {
    require_once standart_path.'core/database/Runner.php';
    $runner = new Runner();
    $runner->connect(host, port, database, database_username, database_password);
    $runner->append_query("SHOW VARIABLES LIKE 'innodb_log_file_size';", null, null, Runner::QUERY_SELECT);
    $runner->execute();
    return (int)(((int)$runner->result($runner->get_default_key())[0][0][1])/2);
}

function isPrimaryKeyDuplicated(Runner &$runner, $id, string $table_name, string $column_name) : bool {
    if($runner === null ) {
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
    }
    $select = new Select($table_name);
    $select->append_column('1');
    $select->append_where(($column_name.' = ?'));
    $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $id));
    $runner->clear_query();
    $runner->append_query($select);
    $runner->execute();
    $runner->clear_query();
    return count($select->result()) > 0;
}
function isOwnerFileEqual($left, $right) : bool {
    if( $left === null && $right === null ) return true;
    else {
        if(!is_numeric($left) || !is_numeric($right)) return false;
        else if((int)$left !== (int)$right) return false;
        else return true;
    }
}
function getSecurityToken() {
    require_once standart_path.'core/Random.php';
    $random = new Random();
    $length = $random->random_number_int(SECURITY_TOKEN_MIN, SECURITY_TOKEN_MAX);
    return $random->alphaNumeric($length);
}
function canDo(User $user, int $do, $input) : bool {
    require_once standart_path.'server_script/PrivilegeChecker.php';
    $checker = new PrivilegeChecker($user);
    return $checker->canDo($do, $input);
}
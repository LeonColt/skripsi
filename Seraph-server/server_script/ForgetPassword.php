<?php

/**
 * Created by PhpStorm.
 * User: LC
 * Date: 03/02/2017
 * Time: 17:35
 */
require_once standart_path.'core/Random.php';
class ForgetPassword
{
    const ID_USERNAME_EMAIL = "cf-fp-32948239048023948239482394";
    private $id_username_email;
    public function __construct(string $id_username_email){$this->id_username_email = $id_username_email;}
    public function execute() {
        $runner = new Runner();
        $runner->connect(host, port, database, database_username, database_password);
        $user = $this->loadUser($runner);
        $random = new Random();
        $password = $random->alphaNumeric(8);
        $update = new Update('benutzer');
        $update->apppend_set('pass', '?');
        $update->append_parameter(new Parameter($update->getParameterVariableIntegerOrder(), password_hash($password, PASSWORD_DEFAULT)));
        $update->append_where('id = ?');
        $update->append_parameter(new Parameter($update->getParameterVariableIntegerOrder(), $user->getId()));
        $runner->clear_query();
        $runner->append_query($update);
        $runner->execute();
        $message = "Your new Password : ".$password;
        $headers = "MIME-Version: 1.0\r\n";
        $headers .= "Content-type: text/html; charset=iso-8859-1\r\n";
        $headers .= "To: ".$user->getUsername()." <".$user->getEmail().">\r\n";
        $headers .= "From: SeraphAdmin <".EMAIL_ADMIN_NO_REPLY.">\r\n";
        if( !mail($user->getEmail(), "Registration Confirmation", $message, $headers) )
            throw new Exception("Mail Cannot Be Sent");
    }
    /**
     * @return User
     * @throws Exception
     */
    private function loadUser(Runner &$runner) {
        $select = new Select('benutzer');
        $select->append_column('id');
        $select->append_column('username');
        $select->append_column('email');
        $select->append_where('id = ?');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->id_username_email));
        $select->append_where('username = ?', 'OR');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->id_username_email));
        $select->append_where('email = ?', 'OR');
        $select->append_parameter(new Parameter($select->getParameterVariableIntegerOrder(), $this->id_username_email));
        $select->fetchAssoc();
        $runner->clear_query();
        $runner->append_query($select);
        $runner->execute();
        if(count($select->result()) === 0 ) throw new Exception("User Not Found");
        $result = $select->result()[0];
        $user = new User($result["id"], $result["username"], $result["email"], false);
        return $user;
    }
}
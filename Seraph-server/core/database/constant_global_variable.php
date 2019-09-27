<?php
/**
 * Created by PhpStorm.
 * User: LC
 * Date: 19/01/2016
 * Time: 18:25
 */
if(!defined("standart_path")) {
    header("HTTP/1.0 404 Not Found - command not found", true, 404);
    exit();
}
const ascending = "ASC";
const descending = "DESC";

const db_operator_equal = "=";
const db_operator_not_equal = "!=";
const db_operator_greater_than = ">";
const db_operator_less_than = "<";
const db_operator_greater_than_or_equal = ">=";
const db_operator_less_than_or_equal = "<=";
const db_operator_between = "BETWEEN";
const db_operator_like = "LIKE";
const db_operator_in = "IN";

const join = "JOIN";
const join_inner = "INNER JOIN";
const join_left = "LEFT JOIN";
const join_right = "RIGHT JOIN";
const join_full = "FULL JOIN";
const join_full_outer = "FULL OUTER JOIN";
<?php

ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

$API_KEY = getenv("api_key");
$PASSWORD = getenv("api_pass");

$username = getenv("idp_sql_user");
$password = getenv("idp_sql_pass");
$hostname = "localhost";
$database = "idp";

$date_format = "Y-m-d H:i:s";

$mysqli = new mysqli($hostname, $username, $password, $database);
if ($mysqli->connect_errno) {
    die("Failed to connect to database! " . $mysqli->connect_errno . ") " . $mysqli->connect_error);
}

?>
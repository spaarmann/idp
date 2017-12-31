<?php

error_reporting(E_ERROR | E_WARNING | E_PARSE); ini_set('display_errors', '1');

$username = getenv("idp_sql_user");
$password = getenv("idp_sql_pass");
$hostname = "localhost";
$database = "idp";

$mysqli = new mysqli($hostname, $username, $password, $database);
if ($mysqli->connect_errno) {
    die("Failed to connect to database! " . $mysqli->connect_errno . ") " . $mysqli->connect_error);
}

if (!($stmt = $mysqli->prepare("SELECT time, value FROM temperature WHERE time BETWEEN ? AND ?"))) {
    die("Failed to prepare statement! " . $mysqli->errno . ") " . $mysqli->error);
}

$start_time = urldecode($_GET['start']);
$end_time = urldecode($_GET['end']);

if (!$stmt->bind_param("ss", $start_time, $end_time)) {
    die("Failed to bind params! " . $mysqli->errno . ") " . $mysqli->error);
}

if (!$stmt->execute()) {
    die("Failed to execute statement! " . $mysqli->errno . ") " . $mysqli->error);
}

if ($result = $stmt->get_result()) {
    $json = "[";

    while ($row = $result->fetch_assoc()) {
        //printf('{ "time": "%s", "value": "%s"}, ', $row["time"], $row["value"]);
        $json = $json . '{ "time": "' . $row["time"] . '", "value": "' . $row["value"] . '"},';
    }

    $json = substr($json, 0, strlen($json)-1);
    $json = $json . ']';

    echo $json;

    $result->free();
}

$stmt->close();
$mysqli->close();
?>
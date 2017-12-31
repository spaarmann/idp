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

$start_time = urldecode($_GET['start']);
$end_time = urldecode($_GET['end']);


if ($step = $_GET['valuegen-step']) {
    
} else {
    if (!($stmt = $mysqli->prepare("SELECT time, temperature, ph FROM measurements WHERE time BETWEEN ? AND ?"))) {
        die("Failed to prepare statement! " . $mysqli->errno . ") " . $mysqli->error);
    }

    if (!$stmt->bind_param("ss", $start_time, $end_time)) {
        die("Failed to bind params! " . $mysqli->errno . ") " . $mysqli->error);
    }

    if (!$stmt->execute()) {
        die("Failed to execute statement! " . $mysqli->errno . ") " . $mysqli->error);
    }

    if ($result = $stmt->get_result()) {
        $json = "[";

        while ($row = $result->fetch_assoc()) {
            $json = $json . '{ "time": "' . $row["time"] . '", "temp": "' . $row["temperature"] . '", "ph": "' . $row["ph"] . '"},';
        }

        $json = substr($json, 0, strlen($json)-1);
        $json = $json . ']';

        echo $json;

        $result->free();
    }
}

$stmt->close();
$mysqli->close();
?>
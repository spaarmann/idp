<?php

require 'common.php';

$time = urldecode($_GET['time']);
$temp = $_GET['temperature'];
$ph = $_GET['ph'];

$key = $_GET['key'];

if ($key !== $API_KEY) {
  die("Invalid API Key!");
}

if (!($stmt = $mysqli->prepare("INSERT INTO measurements (time, temperature, ph) VALUES (?, ?, ?)"))) {
  die("Failed to prepare statement! " . $mysqli->errno . ") " . $mysqli->error);
}

if (!($stmt->bind_param("sss", $time, $temp, $ph))) {
  die("Failed to bind params! " . $mysqli->errno . ") " . $mysqli->error);
}

if (!$stmt->execute()) {
  printf("Error executing: " . $mysqli->errno . ") " . $mysqli->error . "<br />");
}

$stmt->close();
$mysqli->close();

?>
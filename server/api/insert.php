<?php

require 'common.php';

$time = urldecode($_GET['time']);
$table = $_GET['table'];

if ($table === "measurements") {
  $temp = $_GET['temperature'];
  $ph = $_GET['ph'];
} else if ($table === "nutrients") {
  $value = $_GET['measurement'];
} else {
  die("Invalid table!");
}

if ($table === "measurements") {
  $key = $_GET['key'];

  if ($key !== $API_KEY) {
    die("Invalid API Key!");
  }
} else if ($table === "nutrients") {
  $pass = $_GET['password'];

  if ($pass !== $PASSWORD) {
    die("Invalid password!");
  }
}

if ($table === "measurements") {
  $stmtstr = "INSERT INTO `measurements` (`time`, `temperature`, `ph`) VALUES (?, ?, ?)";
} else if ($table === "nutrients") {
  $stmtstr = "INSERT INTO `nutrients` (`time`, `measurement`) VALUES (?, ?)";
}

if (!($stmt = $mysqli->prepare($stmtstr))) {
  die("Failed to prepare statement! " . $mysqli->errno . ") " . $mysqli->error);
}

if ($table === "measurements") {
  if (!($stmt->bind_param("sss", $time, $temp, $ph))) {
    die("Failed to bind params! " . $mysqli->errno . ") " . $mysqli->error);
  }
} else if ($table === "nutrients") {
  if (!($stmt->bind_param("ss", $time, $value))) {
    die("Failed to bind params! " . $mysqli->errno . ") " . $mysqli->error);
  }
}

if (!$stmt->execute()) {
  printf("Error executing: " . $mysqli->errno . ") " . $mysqli->error . "<br />");
}

$stmt->close();
$mysqli->close();

printf("Inserted values successfully!");
?>
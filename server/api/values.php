<?php

require 'common.php';

$start_time = urldecode($_GET['start']);
$end_time = urldecode($_GET['end']);


if (isset($_GET['valuegen-step'])) {
    $step = $_GET['valuegen-step'];
    $start = date_create_from_format($date_format, $start_time);
    $end = date_create_from_format($date_format, $end_time);

    $current = $start;

    if (!($stmt = $mysqli->prepare("INSERT INTO measurements (time, temperature, ph) VALUES (?, ?, ?)"))) {
        die("Failed to prepare statement! " . $mysqli->errno . ") " . $mysqli->error);
    }

    if (!($stmt->bind_param("sss", $current_str, $t, $p))) {
        die("Failed to bind params! " . $mysqli->errno . ") " . $mysqli->error);
    }

    $mysqli->query("START TRANSACTION");

    while ($current < $end) {
        printf("Generating value <br />");
        $current_str = date_format($current, $date_format);

        $t = (float)rand() / (float)getrandmax() * 10.0 + 17.0;
        $p = (float)rand() / (float)getrandmax() * 14;

        if (!$stmt->execute()) {
            printf("Error executing: " . $mysqli->errno . ") " . $mysqli->error . "<br />");
        }

        $current = date_add($current, new DateInterval("PT".$step."S"));
    }

    $stmt->close();
    $mysqli->query("COMMIT");
    echo "Generated test values!";
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

    $json = '{ "measurements": [';
    if ($result = $stmt->get_result()) {
        $anyValues = false;

        while ($row = $result->fetch_assoc()) {
            $anyValues = true;
            $json = $json . '{ "time": "' . $row["time"] . '", "temp": "' . $row["temperature"] . '", "ph": "' . $row["ph"] . '"},';
        }

        if ($anyValues)
            $json = substr($json, 0, strlen($json)-1);
        $json = $json . '],';

        $result->free();
    }

    $stmt->close();

    if (!($stmt = $mysqli->prepare("SELECT time, measurement FROM nutrients WHERE time BETWEEN ? AND ?"))) {
        die("Failed to prepare statement! " . $mysqli->errno . ") " . $mysqli->error);
    }

    if (!$stmt->bind_param("ss", $start_time, $end_time)) {
        die("Failed to bind params! " . $mysqli->errno . ") " . $mysqli->error);
    }

    if (!$stmt->execute()) {
        die("Failed to execute statement! " . $mysqli->errno . ") " . $mysqli->error);
    }

    if ($result = $stmt->get_result()) {
        $anyValues = false;

        $json = $json . '"nutrients": [';

        while ($row = $result->fetch_assoc()) {
            $anyValues = true;
            $json = $json . '{ "time": "' . $row["time"] . '", "measurement": "' . $row["measurement"] . '"},';
        }

        if ($anyValues)
            $json = substr($json, 0, strlen($json)-1);
        $json = $json . ']';

        $result->free();
    }

    $json = $json . '}';
    echo $json;

    $stmt->close();
}

$mysqli->close();
?>
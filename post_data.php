<?php

require "cred.php";

$api_key = ""; 
$lat = $lng = "";
$celcius = $farenheits = "";
$voltage_battery = $current_motor = $power = $consumption = "";
$reading_time = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
        $lat = test_input($_POST["lat"]);
        $lng = test_input($_POST["lng"]);
        $celcius = test_input($_POST["celcius"]);
        $farenheits = test_input($_POST["farenheits"]);
        $voltage_battery = test_input($_POST["voltage_battery"]);
        $current_motor = test_input($_POST["current_motor"]);
        $power = test_input($_POST["power"]);
        $consumption = test_input($_POST["consumption"]);

        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
        // Inserting into database
        $sql = "INSERT INTO gps (lat, lng)
        VALUES ('" . $lat . "', '" . $lng . "');" .
        "INSERT INTO lm35 (celcius, farenheits)
        VALUES ('" . $celcius . "', '" . $farenheits . "');" .
        "INSERT INTO ina226 (voltage_battery, current_motor, power, consumption)
        VALUES ('" . $voltage_battery . "', '" . $current_motor . "', '" . $power  . $consumption . "');";
        
        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    
        $conn->close();
    }
    else {
        echo "Wrong API Key provided.";
    }

}
else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}

?>
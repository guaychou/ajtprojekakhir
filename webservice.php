<?php

$servername = "localhost";
$username = "CHANGE ME";
$password = "CHANGE ME";
$dbname = "CHANGE ME";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 


$output1="";
$output2="";
if( isset($_POST['suhu']) && isset($_POST['kelembapan']) ){
	if ($_POST['suhu'] < 16 ) {
		$output1= "Cold";

}	else if ($_POST['suhu']<25) {
		$output1="Comfort";
} 
	else if ($_POST['suhu']>26) {
		$output1="Hot";
}


	 if ($_POST['kelembapan']<65){
		$output2="Dry";
}	else if ($_POST['kelembapan']<80){
		$output2="Comfort";

}	else if ($_POST['kelembapan']>=80){
		$output2="Wet";
}}


if ($output1==$output2){
	echo $output1;

}else 
	echo "The temperature is $output1 , value = ",$_POST['suhu']," &  The humidity is $output2 , value = ",$_POST['kelembapan'];
	echo "\n";

$suhu = $_POST['suhu'];
$kelembapan = $_POST['kelembapan'];
$sql = "INSERT INTO ajtprojekakhir (suhu,kelembapan,classSuhu,classKelembapan) VALUES ($suhu,$kelembapan,'$output1','$output2')";

if ($conn->query($sql) === TRUE) {
    echo "New record created successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();

?>

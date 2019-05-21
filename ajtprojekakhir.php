<html>
<head>
<style>
table {
  font-family: arial, sans-serif;
  border-collapse: collapse;
  width: 100%;
}

td, th {
  border: 1px solid #dddddd;
  text-align: left;
  padding: 8px;
}

tr:nth-child(even) {
  background-color: #dddddd;
}
</style>
</head>
<table>
  <tr>
    <th>Suhu</th>
    <th>Kelembapan</th>
    <th>Class Suhu</th>
    <th>Class kelembapan</th>
  </tr>
<?php
$servername = "localhost";
$username = "ajt";
$password = "secret";
$dbname = "ajt";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT *  FROM ajtprojekakhir" ;
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
       // echo "Suhu: " . $row["suhu"]. " - Kelembapan: " . $row["kelembapan"]. "- Class Suhu " . $row["classSuhu"]. " - ".$row["classKelembapan"]." <br> ";
	echo "<tr>";
        echo "<td>".$row["suhu"]."</td>";
	echo "<td>".$row["kelembapan"]."</td>";
	echo "<td>".$row["classSuhu"]."</td>";
	echo "<td>".$row["classKelembapan"]."</td>";
    }
} else {
    echo "0 results";
}
$conn->close();
?>
 </tr>
</table>

</body>
</html>

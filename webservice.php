<?php
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
?>

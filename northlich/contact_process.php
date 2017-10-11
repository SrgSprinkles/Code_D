<?php
	$name = $_POST['yourname'];
	$youremail = $_POST['youremail'];
	$title = $_POST['title'];
	$organization = $_POST['organization'];
	$to = "adam@reversedout.com";
	$subject = "Contact Request from Northlich";
	$message = "Below are details of User : \n";
	$message .= "Name : $yourname \n";
	$message .= "Email : $youremail \n";
	$message .= "Title : $title \n";
	$message .= "Organization : $organization \n";
	$from = "adam@reversedout.com";
	$headers = "From:" . $from;
	mail($to,$subject,$message);
	header('Location: index.html');
	exit;
?>
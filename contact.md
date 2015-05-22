---
layout: page
title: Contact
---

# Contact

Please contact me with any questions, inputs, or concerns:

<p style="text-align:left">
Megan Kress
<br>
Michigan State University
<br><br>
</p>

<?php 
if (isset($_REQUEST['submitted'])) {
// Initialize error array.
  $errors = array();
  // Check for a proper First name
  if (!empty($_REQUEST['firstname'])) {
  $firstname = $_REQUEST['firstname'];
  $pattern = "/^[a-zA-Z0-9\_]{2,20}/";// This is a regular expression that checks if the name is valid characters
  if (preg_match($pattern,$firstname)){ $firstname = $_REQUEST['firstname'];}
  else{ $errors[] = 'Your Name can only contain _, 1-9, A-Z or a-z 2-20 long.';}
  } else {$errors[] = 'You forgot to enter your First Name.';}
  
  // Check for a proper Last name
  if (!empty($_REQUEST['lastname'])) {
  $lastname = $_REQUEST['lastname'];
  $pattern = "/^[a-zA-Z0-9\_]{2,20}/";// This is a regular expression that checks if the name is valid characters
  if (preg_match($pattern,$lastname)){ $lastname = $_REQUEST['lastname'];}
  else{ $errors[] = 'Your Name can only contain _, 1-9, A-Z or a-z 2-20 long.';}
  } else {$errors[] = 'You forgot to enter your Last Name.';}
  
  //Check for a valid phone number
  if (!empty($_REQUEST['phone'])) {
  $phone = $_REQUEST['phone'];
  $pattern = "/^[0-9\_]{7,20}/";
  if (preg_match($pattern,$phone)){ $phone = $_REQUEST['phone'];}
  else{ $errors[] = 'Your Phone number can only be numbers.';}
  } else {$errors[] = 'You forgot to enter your Phone number.';}
  
  if (!empty($_REQUEST['redmapleacer']) || !empty($_REQUEST['chinesepistache']) || !empty($_REQUEST['raywoodash'])) {
  $check1 = $_REQUEST['redmapleacer'];
  if (empty($check1)){$check1 = 'Unchecked';}else{$check1 = 'Checked';}
  $check2 = $_REQUEST['chinesepistache'];
  if (empty($check2)){$check2 = 'Unchecked';}else{$check2 = 'Checked';}
  $check3 = $_REQUEST['raywoodash'];
  if (empty($check3)){$check3 = 'Unchecked';}else{$check3 = 'Checked';}
  } else {$errors[] = 'You forgot to enter your Phone number.';}
  }
  //End of validation 
  
  if (isset($_REQUEST['submitted'])) {
  if (empty($errors)) { 
  $from = "From: Our Site!"; //Site name
  // Change this to your email address you want to form sent to
  $to = "your@email.com"; 
  $subject = "Admin - Our Site! Comment from " . $name . "";
  
  $message = "Message from " . $firstname . " " . $lastname . " 
  Phone: " . $phone . " 
  Red Maple Acer: " . $check1 ."
  Chinese Pistache: " . $check2 ."
  Raywood Ash: " . $check3 ."";
  mail($to,$subject,$message,$from);
  }
}
?>

<?php 
  //Print Errors
  if (isset($_REQUEST['submitted'])) {
  // Print any error messages. 
  if (!empty($errors)) { 
  echo '<hr /><h3>The following occurred:</h3><ul>'; 
  // Print each error. 
  foreach ($errors as $msg) { echo '<li>'. $msg . '</li>';}
  echo '</ul><h3>Your mail could not be sent due to input errors.</h3><hr />';}
   else{echo '<hr /><h3 align="center">Your mail was sent. Thank you!</h3><hr /><p>Below is the message that you sent.</p>'; 
  echo "Message from " . $firstname . " " . $lastname . " <br />Phone: ".$phone." <br />";
  echo "<br />Red Maple Acer: " . $check3 . "";
  echo "<br />Chinese Pistache: " . $check2 . "";
  echo "<br />Raywood Ash: " . $check3 . "";
  }
  }
//End of errors array
  ?>
  
  <h2>Contact us</h2>
  <p>Fill out the form below.</p>
  <form action="" method="post">
  <label>First Name: <br />
  <input name="firstname" type="text" value="- Enter First Name -" /><br /></label>
  <label>Last Name: <br />
  <input name="lastname" type="text" value="- Enter Last Name -" /><br /></label>
  <label>Phone Number: <br />
  <input name="phone" type="text" value="- Enter Phone Number -" /><br /></label>
  <label>Red Maple Acer: 
  <input name="redmapleacer" type="checkbox" value="Red Maple Acer" /><br /></label>
  <label>Chinese Pistache: 
  <input name="chinesepistache" type="checkbox" value="Chinese Pistache" /><br /></label>
  <label>Raywood Ash: 
  <input name="raywoodash" type="checkbox" value="Raywood Ash" /><br /></label>
  <input name="" type="reset" value="Reset Form" /><input name="submitted" type="submit" value="Submit" />
  </form>




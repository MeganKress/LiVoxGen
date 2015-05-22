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
 $to = "recipient@example.com";
 $subject = "Hi!";
 $body = "Hi,\n\nHow are you?";
 if (mail($to, $subject, $body)) {
   echo("<p>Email successfully sent!</p>");
  } else {
   echo("<p>Email delivery failed…</p>");
  }
 ?>

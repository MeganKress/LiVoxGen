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
// The message
$message = "Line 1\r\nLine 2\r\nLine 3";

// In case any of our lines are larger than 70 characters, we should use wordwrap()
$message = wordwrap($message, 70, "\r\n");

// Send
mail('kressmeg@msu.edu', 'LiVoxGen Mail', $message);
?>

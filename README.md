<h1>IR Signal Project</h1>
This project is code for a remote control to send IR signals.
<br><br>
<ul>
  <li>index.php - Web page with buttons that interface with the pi</li>
  <li>main.c	- Produces an IR signal based on input from either the web or physical buttons</li>
</ul>
<hr>
<h3>Resources</h3>
<ul>
	<li>Web Server Communication - http://www.linuxhowtos.org/C_C++/socket.htm</li>
	<li>GPIO Library - http://wiringpi.com/</li>
	<li>WiringPi Pinout Guide - https://projects.drogon.net/raspberry-pi/wiringpi/pins/</i>
	<li>IR Signal Generator - http://www.eevblog.com/2013/08/12/eevblog-506-ir-remote-control-arduino-protocol-tutorial/</li>
</ul>
<hr>
<h3>Notes</h3>
<p>index.php needs to be copied to the webserver directory, probably something like /var/www/html</p>

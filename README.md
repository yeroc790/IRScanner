<h1>IR Signal Project</h1>
This project is code for a remote control to send IR signals.
<br><br>
<ul>
  	<li>index.php 	- Web page with buttons that interface with the pi</li>
  	<li>remote.c	- Provides functionality for on/off buttons</li>
	<li>server.c 	- Creates a socket on localhost for index.php to connect to</li>
	<li>functions.c	- Contains functions and variable declarations</li>
</ul>
<hr>
<h3>Compiling</h3>
<ul>
	<li>Geany: gcc -Wall -o "%e" "%f" - lwiringPi</li>
	<li>Terminal: gcc -Wall -o [objectName] [fileName] -lwiringPi</li>
</ul>
<hr>
<h3>Web Server</h3>
<p>Move index.php to the webserver directory<br>The current webaddress is tv-remote.ad.cs.southwesternadventist.edu</p>
<hr>
<h3>Resources</h3>
<ul>
	<li>Web Server Communication - http://www.linuxhowtos.org/C_C++/socket.htm</li>
	<li>GPIO Library - http://wiringpi.com/</li>
	<li>WiringPi Pinout Guide - https://projects.drogon.net/raspberry-pi/wiringpi/pins/</i>
	<li>IR Signal Generator - http://www.eevblog.com/2013/08/12/eevblog-506-ir-remote-control-arduino-protocol-tutorial/</li>
</ul>

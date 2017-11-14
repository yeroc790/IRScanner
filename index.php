<html>
<body>
	<div class="main">
		<form action="" method="post">
			<input type="submit" name="off" value="Turn Off" />
			<input type="submit" name="on" value="Turn On" />
			<input type="submit" name="toggle" value="Toggle" />
		</form>
	</div>
	<div class="output">
		<?php
			if (isset($_POST['on']) || isset($_POST['off']) || isset($_POST['toggle'])) {
				if (isset($_POST['on'])) {
					$msg = "on";
					unset($_POST['on']);
				} else if (isset($_POST['off'])) {
					$msg = "off";
					unset($_POST['off']);
				} else if (isset($_POST['toggle'])) {
					$msg = "toggle";
					unset($_POST['toggle']);
				}
				
				$fp = fsockopen("tcp://127.0.0.1", 51717, $errno, $errstr, 30);
			    if (!$fp) {
					echo "$errstr ($errno)<br />\n";
			    } else {
					fwrite($fp, $msg);
					while (fgets($fp, 128)) {
						echo fgets($fp, 128); // If you expect an answer
					}
					echo "Message sent";
					fclose($fp); // To close the connection
		    	}
				unset($_POST['on']);
			}
		?>
	</div>
</body>
</html>

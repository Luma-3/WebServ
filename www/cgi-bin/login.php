<?php

function renderTemplate($template, $variables) {
	extract($variables);
	ob_start();
	include $template;
	return ob_get_clean();
}

if (isset($_COOKIE['username'])) {
	header('HTTP/1.1 200 OK');
	header('Location: /cgi-bin/index.php');
	exit(200);
}

$username = $_POST['username'];
setcookie('username', $username, time() + 3600, '/', 'localhost');


header('HTTP/1.1 200 OK');
header('Location: /cgi-bin/index.php');
exit(200)

?>
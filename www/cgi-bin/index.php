<?php

if (!isset($_COOKIE['username'])) {
	header('Location: /html/login.html');
	exit;
}

function renderTemplate($template, $variables) {
	extract($variables);
	ob_start();
	include $template;
	return ob_get_clean();
}
echo renderTemplate(__DIR__ . '/../html/index.html', ['username' => $_COOKIE['username']]);

?>
<?php

session_start();

function renderTemplate($template, $variables) {
	extract($variables);
	ob_start();
	include $template;
	return ob_get_clean();
}

echo renderTemplate(__DIR__ . '/../html/account.html', ['username' => $_SESSION['username'], 'session_token' => $_SESSION['session_token']]);

?>
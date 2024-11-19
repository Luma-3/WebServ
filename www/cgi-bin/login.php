<?php

session_start();


$username = $_POST['username'];
$password = $_POST['password'];

try {
	$sqlite = new SQLite3(__DIR__ . '/../database/users.db');
	$sqlite->exec('CREATE TABLE IF NOT EXISTS users (username TEXT, password TEXT, session_token TEXT)');

	$statement = $sqlite->prepare('SELECT password FROM users WHERE username = :username');
	$statement->bindValue(':username', $username);
	$result = $statement->execute();

	$row = $result->fetchArray(SQLITE3_ASSOC);

	if (!password_verify($password, $row['password'])) {
		header('HTTP/1.1 401 Unauthorized');
		echo renderTemplate(__DIR__ . '/../html/user_log_error.html', ['message' => 'Invalid username or password']);
		exit(401);
	}

	$session_token = bin2hex(random_bytes(16));

	$statement = $sqlite->prepare('UPDATE users SET session_token = :session_token WHERE username = :username');
	$statement->bindValue(':session_token', $session_token);
	$statement->bindValue(':username', $username);
	$statement->execute();

} catch (Exception $e) {
	header('HTTP/1.1 500 Internal Server Error');
	echo renderTemplate(__DIR__ . '/../html/user_log_error.html', ['message' => 'Internal server error']);
	exit(500);
}

function renderTemplate($template, $variables) {
	extract($variables);
	ob_start();
	include $template;
	return ob_get_clean();
}

$_SESSION['username'] = $username;
$_SESSION['session_token'] = bin2hex(random_bytes(16));

header('HTTP/1.1 200 OK');
header('Location: /cgi-bin/index.php');
exit(200)

?>
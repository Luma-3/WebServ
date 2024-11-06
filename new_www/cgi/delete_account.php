<?php

session_start();

try {
	$username = $_SESSION['username'];
	$sqlite = new SQLite3(__DIR__ . '/../database/users.db');
	$sqlite->exec('CREATE TABLE IF NOT EXISTS users (username TEXT, password TEXT, session_token TEXT)');

	$statement = $sqlite->prepare('DELETE FROM users WHERE username = :username');
	$statement->bindValue(':username', $username);
	$result = $statement->execute();

	// $row = $result->fetchArray(SQLITE3_ASSOC);

	// if (!password_verify($password, $row['password'])) {
	// 	header('HTTP/1.1 401 Unauthorized');
	// 	echo "Invalid username or password";
	// 	exit();
	// }

	// $session_token = bin2hex(random_bytes(16));

	// $statement = $sqlite->prepare('UPDATE users SET session_token = :session_token WHERE username = :username');
	// $statement->bindValue(':session_token', $session_token);
	// $statement->bindValue(':username', $username);
	// $statement->execute();

} catch (Exception $e) {
	header('HTTP/1.1 500 Internal Server Error');
	echo "Database error";
	exit();
}

function renderTemplate($template, $variables) {
	extract($variables);
	ob_start();
	include $template;
	return ob_get_clean();
}

// $_SESSION['username'] = $username;
// $_SESSION['session_token'] = $session_token;

header('Content-Type: application/json');
header('HTTP/1.1 200 OK');
echo json_encode(['message' => 'User deleted successfully']);
exit()

?>
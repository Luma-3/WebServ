<?php

	function renderTemplate($template, $variables) {
		extract($variables);
		ob_start();
		include $template;
		return ob_get_clean();
	}

	$username = $_POST['username'];
	$password = $_POST['password'];
	$confirm_password = $_POST['confirm_password'];
	
	if ($password != $confirm_password) {
		header('HTTP/1.1 400 Bad Request');
		echo renderTemplate(__DIR__ . '/../html/user_log_error.html', ['message' => 'Passwords do not match']);
		exit();
	}

	try {
		$sqlite = new SQLite3(__DIR__ . '/../database/users.db');
		$sqlite->exec('CREATE TABLE IF NOT EXISTS users (username TEXT, password TEXT, session_token TEXT)');

		$statement = $sqlite->prepare('SELECT COUNT(*) as count FROM users WHERE username = :username');
		$statement->bindValue(':username', $username);
		$result = $statement->execute();

		$row = $result->fetchArray(SQLITE3_ASSOC);

		if ($row['count'] > 0) {
			header('HTTP/1.1 409 Conflict');
			echo renderTemplate(__DIR__ . '/../html/user_log_error.html', ['message' => 'Username already exists']);
			exit();
		}

		$statement = $sqlite->prepare('INSERT INTO users (username, password) VALUES (:username, :password)');
		$statement->bindValue(':username', $username);
		$statement->bindValue(':password', password_hash($password, PASSWORD_DEFAULT));
		$statement->execute();
	} catch (Exception $e) {
		header('HTTP/1.1 500 Internal Server Error');
		echo renderTemplate(__DIR__ . '/../html/user_log_error.html', ['message' => 'Internal Server Error']);
		exit();
	}

	header('HTTP/1.1 307 Temporary Redirect');
	header('Location: /html/sign_in_success.html');
	exit();
?>
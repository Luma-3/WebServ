<?php
$NAME = $_POST['name'] ?? ($_GET['name'] ?? 'no name');
$age = $_POST['age'] ?? ($_GET['age'] ?? 'no age');
echo "name={$NAME}&age={$age}&cookie=";

?>

<?php
// Retrieve POST body content
$postBody = file_get_contents('php://input');

// Process the POST body
echo "Received POST Body: " . $postBody;
?>
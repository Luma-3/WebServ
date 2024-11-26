function sendDeleteRequest() {
	const url = '/cgi-bin/delete_account.php';
	const options = {
		method: 'DELETE',
		headers: {
			'Content-Type': 'application/json',
		},
		body: JSON.stringify()
	};

	fetch(url, options)
		.then(response => {
			if (response.ok) {
				return response.json();
			}
			throw new Error('Network response was not ok.');
		})
		.then(data => {
			console.log('Success:', data);
			alert('Account deleted successfully.');
			window.location.href = '/html/sign_in.html';
		})
		.catch(error => {
			console.error('There was a problem with the fetch operation:', error);
			alert('Failed to delete account.');
		});
}
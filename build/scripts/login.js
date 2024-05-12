const loginForm = document.getElementById('loginForm');
const emailField = document.getElementById('email')
const passwordField = document.getElementById('password');

loginForm.addEventListener('submit', (event) => {
    event.preventDefault();
    const formData = new FormData(loginForm);
    const urlEncodedData = new URLSearchParams(formData).toString();  // convert data
    fetch('/login', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded'  // choose http-protocol
          },
        body: urlEncodedData
    })
    .then(response => {
        if (!response.ok) {
            return response.json().then(data => {
                throw new Error(data.message);
            });
        }
        return response.json();
    })
    .then(message => {
        window.location.href = '/index.html';
    })
    .catch(error => {
        alert(error.message);
    });
});

const registrationForm = document.getElementById('registrationForm');
const passwordField = document.getElementById('password');
const confirmPasswordField = document.getElementById('confirmPassword');

registrationForm.addEventListener('submit', (event) => {
    event.preventDefault();
    if (passwordField.value.length < 7) {
        alert("Пароль должен быть не менее 7 символов!");
        return;
    } else if (passwordField.value !== confirmPasswordField.value) {
        alert("Пароли не совпадают!")
        return;
    }
    const formData = new FormData(registrationForm);
    const urlEncodedData = new URLSearchParams(formData).toString();  // convert data
    fetch('/register', {
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

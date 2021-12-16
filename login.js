
const loginForm = document.getElementById("login-form");
const loginButton = document.getElementById("login-form-submit");
const loginErrorMsg = document.getElementById("login-error-msg");

loginButton.addEventListener("click", (e) => {
    e.preventDefault();
    const username = loginForm.username.value;
    const password = loginForm.password.value;
    console.log(password);
    var myHeaders = new Headers();
    myHeaders.append("Content-Type", "application/x-www-form-urlencoded");

    var urlencoded = new URLSearchParams();
    urlencoded.append("name", username);
    urlencoded.append("pass", password);
        var requestOptions = {
        method: 'POST',
        headers: myHeaders,
        body: urlencoded
    };

    fetch("http://127.0.0.1:1337/login", requestOptions)
    .then(response => response.json())
    .then(result => {
        if(result !== "wrong input"){
            console.log(result);
            
        }
    })
    .catch(error => console.log('error', error));
    if (username === "user" && password === "otto") {
        window.location = "main-page.html";
        return true;
    } else {
        loginErrorMsg.style.opacity = 1;
    }
})

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
            sessionStorage.setItem('id', result.id);
            sessionStorage.setItem('name', result.name);
            sessionStorage.setItem('isLogged', 'true');
            window.location = "main-page.html";
        }
        else{
            loginErrorMsg.style.opacity = 1;
        }
    })
    .catch(error => console.log('error', error));
    
});

function setCookie(cname, cvalue, exdays) {
    const d = new Date();
    d.setTime(d.getTime() + (exdays*24*60*60*1000));
    let expires = "expires="+ d.toUTCString();
    document.cookie = cname + "=" + cvalue + ";" + expires + ";path=/";
  }

  function getCookie(cname) {
    let name = cname + "=";
    let ca = document.cookie.split(';');
    for(let i = 0; i < ca.length; i++) {
      let c = ca[i];
      while (c.charAt(0) == ' ') {
        c = c.substring(1);
      }
      if (c.indexOf(name) == 0) {
        return c.substring(name.length, c.length);
      }
    }
    return "";
  }

var myHeaders = new Headers();
myHeaders.append("Content-Type", "application/x-www-form-urlencoded");

var bodyRequest = new URLSearchParams();
bodyRequest.append("id", sessionStorage.getItem('id'));

var requestOptions = {
    method: 'POST',
    headers: myHeaders,
    body: bodyRequest
};

let date = [];
let values = [];

fetch("http://127.0.0.1:1337/heartrate", requestOptions)
    .then(response => response.json())
    .then(result => {
        let test = [];
        result.forEach(element => {
            let splited = element.date.split('T');
            let deux = splited[1].split(':');
            let final = "";
            deux.forEach(e => {
                final += e.toString();
            })
            
            date.push(parseInt(final));
            test.push(element.value);
            values.push(element.value);
            var ctx = document.getElementById('myChart').getContext('2d');
                console.log(values, date);
                const labels = date;
                const data = {
            labels: labels,
            datasets: [{
                label: 'My First Dataset',
                data: values,
                fill: false,
                borderColor: 'rgb(75, 192, 192)',
                tension: 0.1
            }]
            };
            var myChart = new Chart(ctx, {
                    type: 'line',
                    data: data,
                    options: {
                        showXLabels: 3
                    }
            });
            

        });
    });


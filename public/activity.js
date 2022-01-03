var myHeaders = new Headers();
myHeaders.append("Content-Type", "application/x-www-form-urlencoded");

var bodyRequest = new URLSearchParams();
bodyRequest.append("id", sessionStorage.getItem('id'));

var requestOptions = {
    method: 'POST',
    headers: myHeaders,
    body: bodyRequest
};
let dates =[];
let values =[];

fetch("http://127.0.0.1:1337/isSeated", requestOptions)
.then(response => response.json())
.then(result => {
    console.log(result);
    result.forEach(element => {
        let d = element.date.split('.')[0];
        dates.push(d);
        values.push(element.state);
        
    })
    var ctx = document.getElementById('Chart').getContext('2d');
                const labels = dates;
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
                        responsive: true,
                        interaction: {
                          intersect: false,
                          axis: 'x'
                        },
                        plugins: {
                          title: {
                            display: true,
                            text: (ctx) => 'Step ' + ctx.chart.data.datasets[0].stepped + ' Interpolation',
                          }
                        }
                      }
            });
})


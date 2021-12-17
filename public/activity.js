const CHART = document.getElementById('myChart');

var myChart = new Chart(CHART, {
    type: 'line',
    data: {
        labels: Object.keys(data),
        datasets: [{
            label: 'My first dataset',
            fill: false,
            lineTension: 0,
            backgroundColor: "rgba(75,192,192,0.4)",
            borderColor: "rgba(75,192,192,1)",
            borderCapStyle: 'butt',
            borderDash: [],
            borderDashOffset: 0.0,
            borderJointStyle: 'miter',
            data: Object.values(data)
        }]
    }
})
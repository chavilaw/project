var X_AXIS = 'Date';
var Y_AXIS = 'Duration';
var BEGIN_AT_ZERO = false;
var SHOW_GRID = true;
var SHOW_LEGEND = false;

var ctx = document.getElementById('chart1').getContext('2d');
const data = {
    datasets: [{
        label: 'My First Dataset',
        data: [65, 59, 80, 81, 56, 55, 40],
        fill: false,
        borderColor: 'rgb(75, 192, 192)',
        tension: 0.1
    }]
};

const config = {
    type: 'line',
    data: data,
};

const chart = new Chart(ctx, config);
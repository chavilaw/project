new Chart(document.getElementById("chart1"), {
    type: 'line',
    data: {
        labels: [1200,1205,1210,1215,1220,1225,1230,1235,1240,1245],
        datasets: [{
            data: [1,0,1,1,0,0,1,0,0,0],
            label: "Username",
            borderColor: "#3e95cd",
            fill: false
        }]
    },
    options: {
        title: {
            display: true,
            text: 'Heart rate monitor data'
        }
    }
});
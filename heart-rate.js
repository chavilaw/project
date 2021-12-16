new Chart(document.getElementById("chart2"), {
    type: 'line',
    data: {
        labels: [1200,1205,1210,1215,1220,1225,1230,1235,1240,1245],
        datasets: [{
            data: [55,58,60,54,55,54,52,49,52,53],
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

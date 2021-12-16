$.getJSON('heart-rate.json', function(data) {
    console.log(data); //see your data ( works in Chrome / FF with firebug)
    console.log(data["date"]["value"]) //should output 1
});



fetch("heart-rate.json")
    .then(response => response.json())
    .then(json => console.log(json));
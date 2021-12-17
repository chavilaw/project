/*$.getJSON('heart-rate.json', function(data) {
    console.log(data); //see your data ( works in Chrome / FF with firebug)
    console.log(data["date"]["value"]) //should output 1
});
*/

var myHeaders = new Headers();
myHeaders.append("Content-Type", "application/x-www-form-urlencoded");

var bodyRequest = new URLSearchParams();
bodyRequest.append("id", 1);

var requestOptions = {
    method: 'POST',
    headers: myHeaders,
    body: bodyRequest
};

fetch("http://127.0.0.1:1337/heartrate", requestOptions)
.then(response => response.json())
.then(result => {
    let array = [];
    let values = [];
    result.forEach(element => {
        let splited = element.date.split('T');
        let splited2 = splited[1].split(':');
        let final = splited2[0].toString() + splited2[1].toString();
        console.log(final);
        array.push(final);

        console.log(element.value);
        values.push(element.value);


    });
    console.log(array);
    console.log(values);
});
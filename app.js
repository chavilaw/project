var express = require('express');
var mysql = require('mysql');
var jwt = require('jsonwebtoken')
var mqtt =  require('mqtt')
var cors =  require('cors');
var fs =  require('fs');
var path = require('path')

var connection = mysql.createConnection({
    host     : 'localhost',
    user     : 'root',
    password : 'rootroot', 
    database: "LifeNotification"
  });


var app = express();
    
let i  =0 ;
app.use(cors())
let oldDate = 0;
let currentDate = new Date();
let newDate = currentDate.getSeconds();

app.use(express.json()) // for parsing application/jsonapp.use(express.json()) // for parsing application/json
app.use(express.urlencoded({ extended: true })) 
app.use(express.static(__dirname + '/public'));

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, '/public/main-page.html'));
})

app.post('/signup', (req, res) => {
    connection.connect((err) => {
        console.log('connected')
        var sql = "INSERT INTO users (id, name, password) VALUES (NULL,'"  + req.body.name + "', '" + req.body.pass + "')";
        connection.query(sql, (err) => {
            if(err){
                console.log(err)
            }
            console.log("1 record inserted")
            res.send("1 user added to the database");
        })
    });
})

app.post('/login', (req, res) => {
        connection.query("SELECT * FROM users WHERE name = '"+ req.body.name+"'", function (err, result) {
          if (err) throw err;
          if(result[0] !== undefined){
            if(result[0].password === req.body.pass){
                let r = Object.values(JSON.parse(JSON.stringify(result)))[0]
                res.send(result[0])
            }
            else{
                res.send("wrong password")
            }
        }
        else{
            res.send('wrong username')
        }
        });
})

app.post('/heartrate', (req, res) => {
    if(req.body.id !== undefined){
        connection.connect(function(err) {
            if(err){
                console.log(err);
            }
            connection.query("SELECT * FROM heartrate WHERE id = '"+ req.body.id+"'", function (err, result) {
              if (err) throw err;
                  let r = Object.values(JSON.parse(JSON.stringify(result)))
                  res.send(r)
            });
          });
    }
})

app.post('/isSeated', (req, res) => {
    if(req.body.id !== undefined){
        connection.connect(function(err) {
            if(err){
                console.log(err);
            }
            connection.query("SELECT * FROM isseated WHERE id = '"+ req.body.id+"'", function (err, result) {
              if (err) throw err;
                  let r = Object.values(JSON.parse(JSON.stringify(result)))
                  res.send(r)
            });
          });
    }
})

app.listen(1337, () => {
    console.log('ready on port 1337');
});

const host ="otso.hopto.org"

const portmqtt = 1883

const clientId = `mqtt_${Math.random().toString(16).slice(3)}`;

const connectUrl = `mqtt://${host}:${portmqtt}`
let message = "";

const client = mqtt.connect(connectUrl, {
    clientId,
    clean: true,
    connectTimeout: 4000,
    username: 'SmartIotMQTT',
    password: 'SmartIot',
    reconnectPeriod: 1000,
  })

  const topic = 'LifeNotification/isSeated'
  const heartRate = 'LifeNotification/heartrate'
  client.on('connect', () => {
    console.log('Connected')
    client.subscribe([topic, heartRate], () => {
    })
  })

  client.on('message', (topic, payload) => {
    currentDate = new Date();
      i++;
      newDate = currentDate.getSeconds();
      let res;
      if(topic === "LifeNotification/eartRate"){
          res = payload.toString();
      }
      else{
        res = JSON.parse(payload.toString());
      }
      const date = currentDate.getFullYear().toString() + "-" + currentDate.getMonth().toString() + "-" + currentDate.getDate().toString() + " " + currentDate.getHours().toString() + ":" + currentDate.getMinutes().toString() + ":" + currentDate.getSeconds().toString();
      if(topic === "LifeNotification/isSeated"){
      if(res.state !== undefined){$
            var sql = "INSERT INTO isseated (id, date, state) VALUES ("+ res.id+",'"  + date + "', '"+res.state+"')";
            connection.query(sql, (err) => {
                if(err){
                    console.log(err)
                }
                console.log("1 record inserted")
            })
      }
    }
    else if(topic === "LifeNotification/heartrate"){
        if(i%100 === 0){
            console.log("seconde")
            connection.connect(function(err){
                let id = 1;
                let value = res;
                console.log('connected')
                var sql = "INSERT INTO heartrate (id, date, value) VALUES ("+ id+",'"  + date + "', '"+value+"')";
                connection.query(sql, (err) => {
                    if(err){
                        console.log(err)
                    }
                    console.log("1 record inserted")
                })
            oldDate =  newDate;
            })
                
        }
    }
  })


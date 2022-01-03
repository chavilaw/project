# IoT Project

## Product background and objectives

Concept: System for detecting emergency situations for a user in a wheelchair (elderly, disabled, etc.). 

Two devices: One on a wheelchair (embedded seat pad), other one wearable (on wrist?)

Wheelchair unit detects if the user is sitting down or left the chair.

Connects to wearable device and to the Internet via Wi-Fi.

Wearable device detects heart rate and barometric pressure which then calculates the change of height.

## Webpage design and programming


website and a phone page/app interface as one to access the data, which also automatically sends text messages to direct family members. 

what the interface will show
heart rate in the past few days, 2 types of diagram display (for the medical professionals)
diagram of different data
seat movement - the sleeping hours (leave the seat over 1 hours) 

web page
have user name and password setting
nurses/doctor can access the data from their devices

column
heart diagram




need for the website
graphs for the heart rate for a week
time off the seat, sleep duration graph
altimeter data, plot, flot charts

other programming on the website
Login - username and password connected to the device
Send direct text to close relative when emergency occur

## Server and database

We needed to create a server which is useful for the communication between the website and the database. There is 3 tables in the database: one for the users (user id, username and password), one for the heart-rate (user id, date and the value) and one for the position of the customer, to know if the person is on the weelchair or not (user id, date and value).
We used node.js and express.js for creating APIs and mySql for the database.

### APIs

#### Login
* URL : `` /login ``
* Method : 
  `` POST 
  body : username, password
  
 If the username and the password of the request body are matching with a line of the users table of the database, the result of the request is a JSON object of the user. Then we create a session storage on the browser to save the informations of the user.
 If the informations are not matching, the result of the request is 'wrong information'.
 
#### Registration

* URL : `` /signup ``
* Method : 
  `` POST `` 
  body : username, password
  
 With this request we can add a line in the users table of the database with the informations of the request.
 
#### Heart-rate

* URL : `` /heartrate ``
* Method : 
   `` POST ``
  body : id
  
 If the id of the body's request is matching with an id of the database, the result of the request is an array with all the data of the heartrate stored in the database of the user. 
 
#### Is Seated

* URL : `` /isSeated ``
* Method : 
  `` POST ``
  body : id
  
If the id of the body's request is matching with an id of the database, the result of the request is an array with all the saved position of the customer.

We still have a lot of work to do with these APIs (security for example).

### Communication with the embedded

For the communication with the embedded, we are using the MQTT protocol. 
We are using the MQTT library of node.js.

We subscribe to 2 topics: `` LifeNotification/heartrate `` and `` LifeNotification/isSeated ``. When there is a new message in one of these topics, the value of the message is added to the right database.

## Embedded design and programming
### Components
- 2x LPCXpresso1549 development board
- Arduino UNO
- ESP8266 Wi-Fi module
- 2x BMP280 barometric pressure and altitude sensor
- MAXREFDES117 Heart-Rate and Pulse-Oximetry Monitor utilizing MAX30102 optical heart rate sensor

### Project execution
Concept of the project was to design and build a system that would detect emergency situations for elderly and disabled users. 

Originally, we wanted to use two separate devices, which would connect to each other and share data via BLE. The system would use Wi-Fi and MQTT to send data to a web server for it to be monitored. 

### Final product
On the final week before presentations, we came across on some difficulties on the embedded side. We knew this was going to be the toughest part since all our group members majors is networking.

The delivery on our sensors delayed quite a bit so we were in a hurry to learn how to program them and use them in practice.

We could not get data out from our sensors to send to our server, so we switched our LPC boards to one Arduino UNO. We were far more familiar with programming on an Arduino development board and this way we could at least demonstrate that the concept and idea of the system works. The idea was to use the Arduino to send to the MQTT server data for heartrate, altitude and a simple button for whether a person is seated. While all the sensors worked individually, the Arduino's serial port stopped working with all of the features, which seemed to be related to the low memory warnings upon uploading the compiled code. So at last minute we settled on showing only heartrate data.

For the demonstartion, we had the Arduino connected via USB/emulated serial port to a laptop running a program called [TTY2MQTT](https://www.metacodes.pro/funcodes/blog/using_tty2mqtt_to_bridge_between_serial_communication_and_mqtt/) that monitors serial port input and uploads the heartrate data to an MQTT server running on a Raspberry Pi at a group member's home.

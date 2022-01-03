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

## Embedded design and programming
### Components
- 2x LPCXpresso1549 development board
- Arduino UNO
- Wi-Fi module
- 2x BMP280 barometric pressure and altitude sensor
- Optical heart rate sensor

Sensors are interfaced via I2C and communication between sensors and web server is via Wi-Fi and MQTT.

## Project execution
Concept of the project was to design and build a system that would detect emergency situations for elderly and disabled users. 

Originally, we wanted to use two separate devices, which would connect to each other and share data via BLE. The system would use Wi-Fi and MQTT to send data to a web server for it to be monitored. 

## Final product
On the final week before presentations, we came across on some difficulties on the embedded side. We knew this was going to be the toughest part since all our group members majors is networking.

The delivery on our sensors delayed quite a bit so we were in a hurry to learn how to program them and use them in practice.

We could not get data out from our sensors to send to our server, so we switched our LPC boards to one Arduino UNO. We were far more familiar with programming on an Arduino development board and this way we could at least demonstrate that the concept and idea of the system works. 

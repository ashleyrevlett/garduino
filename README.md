#garduino
---

Garduino is an Arduino-based garden monitoring system.
Monitor temperature, light level and humidity of a greenhouse/garden
and display the readings via a publicly accessible website.

## System Components:

####Garduino (Arduino): 
Garduino is a simple sketch that senses temperature, light, and humidity.
It uses the Arduino's Ethernet library to serve a CSV file over HTTP.
Requires Arduino w/ Ethernet Shield and an IP address.

####Greenhouse Application (C++ Daemon):
Greenhouse is a C++ application runs an infinite loop, regularly reading data 
from an Arduino via HTTP, recording data to a SQLite DB, and updating a CSV file for the web app to read.
Requires g++, cURL and SQLite3.

####Web Application (HTML and d3.js):
Lives on same server as Greehouse App and displays data from CSV file
in visual form w/ d3.js; requires web server to host files
Currently this is just a webpage that displays a d3.js graph of temperature. 
In progress.	

##Usage:

**Arduino:**
Connect your Arduino to an [Ethernet Shield](http://arduino.cc/en/Main/ArduinoEthernetShield) and plug it into your router. 
Attach a light sensor to ```pin A0``` and a temperature sensor to ```pin A3```. 
Upload the "arduino/garduino.ino" sketch to the Arduino.
Let it run. 

**Server:**
Set up a server (I've tested both Debian Squeeze and a Mac) with Apache, cURL, and SQLite3 installed, then:
```
git clone https://github.com/ashleyrevlett/garduino.git
cd server/application
make
./greenhouse ARDUINO_URL (ex.: ./greenhouse http://192.168.1.191)
```
Create a virtual host that points to the server/public_html directory of the Garduino.
To make the web app accessible outside your local network, you'll need to get a static IP
address (or virtual static IP) and configure your router to forward port 80 traffic to 
your server's IP Address.

## Todo's

* finish flesh out the UI for the web app
* adding the ability for the Arduino to react to its environment, like turning lights and irrigation on and off. 
* sending text or email notifications to someone if certain events occur (ex., lights go off at odd time, temp is too high, etc.).

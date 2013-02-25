#garduino
---


Garduino is an Arduino-based garden monitoring system.
Monitor temperature, light level and humidity of a greenhouse/garden
and display the readings via a publicly accessible website.

__Usage:__

Arduino:
Upload the "arduino/garduino.ino" sketch to an Arduino that's connected
to your local network via an [Ethernet Shield](http://arduino.cc/en/Main/ArduinoEthernetShield).

Server:
    Set up a server (I've tested both Debian Squeeze and a Mac) with cURL and SQLite3 installed, then:
    git clone https://github.com/ashleyrevlett/garduino.git
    cd server/application
    make
    ./greenhouse ARDUINO_URL (ex.: ./greenhouse http://192.168.1.191)

To use the web app, install a web server like Apache and create 
a virtual host that points to the server/public_html directory of the Garduino.
To make the web app accessible outside your local network, you'll need to get a static IP
address (or virtual static IP) and configure your router to forward port 80 traffic to 
your server's IP Address.
 
## System Components:

####Arduino Gardaemon: 
Garduino is a simple sketch that senses temperature, light, and humidity.
It uses the Arduino's Ethernet library to serve a CSV file over HTTP.
Requires Arduino w/ Ethernet Shield and an IP address.

####Greenhouse Application:
"Greenhouse" is a C++ application runs an infinite loop, regularly reading data 
from an Arduino via HTTP, recording data to a SQLite DB, and updating a CSV file for the web app to read.
Compiling the app requires cURL and SQLite3.
Future phase: directly asking the Arduino to turn on/off lights and pipes.

####Web Application:
Lives on same server as Greehouse App and displays data from CSV file
in visual form w/ d3.js; requires web server to host files
Currently this is just a webpage that displays a d3.js graph of temperature. 
In progress.	

## Todo's
* finish flesh out the UI for the web app
* adding the ability for the Arduino to react to its environment, like turning lights and irrigation on and off. 
* sending text or email notifications to someone if certain events occur (ex., lights go off at odd time, temp is too high, etc.).

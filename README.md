#garduino
========

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
 
### System Components:

__Arduino Gardaemon__: Arduino sketch that sense temperature, light, and humidity and broadcasts
	 current readings, that are saved to a csv file and served via HTTP;
	 requires Arduino Ethernet Board and accessible IP address

__Greenhouse Application__: C++ app that reads data from the Arduino via HTTP, 
	records data to SQLite DB, and creates a CSV file for the web app to read; 
	requires cURL and SQLite3

__Web Application__: lives on same server as Greehouse App and displays data from CSV file
	in visual form w/ d3.js; requires web server to host files
	
 
### Garduino (Arduino) Notes: 

Garduino is a simple sketch that uses the Arduino's Ethernet library to serve a CSV file
with a single line - the current readings. Requires an Arduino w/ Ethernet board.
  
### Greenhouse Notes:

The Greenhouse app runs an infinite loop, reading data via an http request from a file written by an Arduino
every x seconds, then recording the result to a database and writing a CSV file. 
Future phase: directly asking the Arduino to turn on/off lights and pipes.
 
### Web Application  Notes: 

Currently this is just a webpage that displays a d3.js graph of temperature. 
In progress.



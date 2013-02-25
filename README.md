garduino
========

Garduino is n Arduino garden monitoring system.
Monitor the environment of a greenhouse (or houseplants)
and display the readings via a publicly accessible website.
Future plans include adding support for adjusting the lights
and irrigation based on current readings. 
 
### System Components:

_Arduino Gardaemon_: Arduino sketch that sense temperature, light, and humidity and broadcasts
	 current readings, that are saved to a csv file and served via HTTP;
	 requires Arduino Ethernet Board and accessible IP address

_Greenhouse Application_: C++ app that reads data from the Arduino via HTTP, 
	records data to SQLite DB, and creates a CSV file for the web app to read; 
	requires cURL and SQLite3

_Web Application_: lives on same server as Greehouse App and displays data from CSV file
	in visual form w/ d3.js; requires web server to host files
  
### Greenhouse App Notes:

The Arduino IP Address is hard-coded in the Greenhouse.h file. 
(Future plan: to pass the IP as a parameter on running the app.)
Compile the app by running the makefile in the 'server/application' directory.
Then type ./greenhouse to run the app.

The Greenhouse app runs an infinite loop, reading data via an http request from a file written by an Arduino
every x seconds, then recording the result to a database and writing a CSV file. 
Future phase: directly asking the Arduino to turn on/off lights and pipes;
 
### Arduino Gardaemon Notes: 

Gardaemon is a simple sketch that uses the Arduino's Ethernet library to serve a CSV file
with a single line - the current readings. Requires an Arduino Ethernet board.

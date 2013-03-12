#garduino
---

Garduino is an Arduino-based garden monitoring system.
Monitor temperature, light level and humidity of a greenhouse/garden
and display the readings via a publicly accessible website.

## System Components:

####Garduino (Arduino): 
Garduino is a simple sketch that senses temperature, light, and humidity.
It uses the Arduino's Ethernet library to serve a CSV file over HTTP.
Requires Arduino w/ Ethernet Shield, Arduino Time and Ethernet libraries installed.
Sends data in format of:
```unsigned large int time_since_arduino_boot, float light (range: 0-100), float temp, float soil moisture (range: 0-100)```

####Greenhouse Application (C++ Daemon):
Greenhouse is a C++ application runs an infinite loop, regularly reading data 
from an Arduino via HTTP, recording data to a SQLite DB, and updating a CSV file for the web app to read.
Requires g++, cURL and SQLite3.

####Web Application (HTML and d3.js):
Lives on same server as Greehouse App and displays data from CSV file
in visual form w/ d3.js; requires web server to host files
Currently this is just a webpage that displays a d3.js graph of temperature. 	

##Usage:

**Arduino:**
Connect your Arduino to an [Ethernet Shield](http://arduino.cc/en/Main/ArduinoEthernetShield) and plug it into your router. 
Attach a light sensor to ```pin A0``` and a temperature sensor to ```pin A3```. 
Upload the "arduino/garduino.ino" sketch to the Arduino.
Let it run. 
TODO: add circuit diagram, detail nail/soil moisture circuit

**Server:**
I used Debian Squeeze with Apache, but the server and OS is really up to you. Curl, sqlite3, g++ required.
```
sudo apt-get install build-essential apache2 libsqlite3-dev libcurl3-gnutls-dev
cd /var/www
sudo git clone https://github.com/ashleyrevlett/garduino.git
cd server/application/database
cp greenhouse3.db.sample greenhouse3.db
cd ..
sudo make
sudo ./greenhouse ARDUINO_URL (ex.: ./greenhouse http://192.168.1.191)
```
Create a virtual host that points to the server/public_html directory of the Garduino. 
On Apache, you can use the included sample file as a starting point.
Replace "myGarduino.com" your domain name:
```
sudo cp server/conf/vhost.com.sample /etc/apache2/sites-available/myGarduino.com
vi /etc/apache2/sites-available/myGarduino.com
sudo a2ensite myGarduino.com
sudo /etc/init.d/apache2 reload
```

To make the web app accessible outside your local network, you'll need to get a static IP
address (or virtual static IP, like from dyndns.org) and configure your router to forward port 80 traffic to 
the IP address of the server we set up above.

## Todo's

* add data archive view to web app
* add alert/email notification functionality (ex., plant is thirsty, temp too high, etc.).
* adding ability for Arduino to turning lights and irrigation on and off based on readings. 

###License
	Copyright (C) 2013 ashley revlett - ashleyrevlett@gmail.com
	License: GNU General Public License, version 3 (GPL-3.0)

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
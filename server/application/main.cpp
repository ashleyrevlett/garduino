/*
 
 ### Description:
 
 Monitor Greenhouse environment, and
 adjust lights and water as needed
 to maintain optimal plant health
 
 
 ### Components:
 
 Greenhouse: C++ app (client) that reads data from file on Arduino server, stores, analyzes and reacts to data
 Gardaemon: Arduino (plus http server) that monitors conditions and records data to file, also hosts file server
 
 
 ### Notes on Greenhouse:
 
 The Greenhouse program runs an infinite loop, reading data via an http request from a file written by an Arduino
 every x seconds, then recording the result to a database. Currently Greenhouse is a client only.
 Future phase: directly asking the Arduino to turn on/off lights and pipes;
 use sockets to send a request to the arduino directly, such as refresh or turn off lights
 
 
 ### Class Design:
 
 Greenhouse
 time
 temp
 humidity
 lumens
 
 Lamp
 status (on/off)
 - flipSwitch
 
 WaterPipe
 status (open/closed)
 - flipSwitch
 
 
 ### plan for Greenhouse:
 
 daemon:
 set up application
 loop forever:
 send http request to arduino
 parse request results
 connect to db
 write results to db
 
 also need another app to display current measurements and stats on past measurements...
 ...but that will be later
 
 
 
 */


#include <iostream>
#include <unistd.h>
#include "Greenhouse.h"

int main() {
    
    Greenhouse *gh = new Greenhouse;
    
    while (1) {
        
        gh->refreshReadings();
        gh->printReadings();
        gh->createCSV();

        std::cout << "Sleeping for 15 seconds." << std::endl << std::endl;
        sleep(15);
        
    };
    
	return 0;
	
}

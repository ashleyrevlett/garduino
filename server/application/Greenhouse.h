/**
 * @package Garduino
 * @file 	Greenhouse.h
 * Query an Arduino for its environmental readings, 
 * record the results to a database, and serve the contents 
 * of the database to a web application (as a CSV file).
 * 
 * The url argument must give the absolute URL of the Arduino,
 * as set in the garduino.ino file (default: 192.168.1.191).
 * 
 * @param  url  a URL giving the location of the Arduino; ex. http://192.168.1.191
 * @return      void; infinite loop
 */


#ifndef __Greenhouse__Greenhouse__
#define __Greenhouse__Greenhouse__

#include <iostream>
#include <time.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sqlite3.h>


#define BUFFER_SIZE 800
#define CSV_FILE "../public_html/data.csv"
#define DB_FILE "database/greenhouse3.db"

class Greenhouse {
    	
   /**
    * A struct to store the Arduino data temporarily
    */	
    struct write_result {
        char *data;
        int pos;
    };
        
public:
	
   /**
    * Create new Greenhouse object
	* @param 	const char * Arduino IP Address
    * @return	time_t current UNIX epoch time (in seconds)
    */	
	Greenhouse( const char * ipAddress );
    ~Greenhouse();
	
	/// Basic Getters and Setters
	float getTemp() { return temp; };
	float getHumidity() { return humidity; };
	float getLumens() { return lumens; };
	float getSoilMoisture() { return soil_moisture; };
	const char * getArduinoIP() { return arduinoIP; };

   /**
    * Return current UNIX timestamp
    * @return	time_t current UNIX epoch time (in seconds)
    */
    time_t getTime();

   /**
    * Update Arduino readings and class properties
    */
	void refreshReadings();

   /**
    * Print current data to stdout; for debugging
    */	
	void printReadings();

    
private:
	const char * arduinoIP;
	float temp;
	float humidity;
	float soil_moisture;
	float lumens;	

   /**
    * Record Greenhouse's current readings to the DB
    * @return	int	true or false depending on whether DB write was successful
    */
	int recordReadings();
	
	static int sql_callback(void *NotUsed, int argc, char **argv, char **azColName);
   	
	static size_t curl_write( void *ptr, size_t size, size_t nmemb, void *stream);

};


#endif /* defined(__Greenhouse__Greenhouse__) */

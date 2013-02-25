//
//  Greenhouse.h
//  Greenhouse
//

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
    
    struct write_result {
        char *data;
        int pos;
    };
    
    
    
private:
	const char * arduinoIP;
	float temp;
	float humidity;
	float lumens;	
	int recordReadings();
	static int sql_callback(void *NotUsed, int argc, char **argv, char **azColName);
   	static size_t curl_write( void *ptr, size_t size, size_t nmemb, void *stream);
    
public:
	Greenhouse( const char * ipAddress );
    ~Greenhouse();
	float getTemp() { return temp; };
	float getHumidity() { return humidity; };
	float getLumens() { return lumens; };
	const char * getArduinoIP() { return arduinoIP; };
    time_t getTime();
	void refreshReadings();
	void printReadings();
	void createCSV();

};


#endif /* defined(__Greenhouse__Greenhouse__) */

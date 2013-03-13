/**
 * @package Garduino
 * @file	DataService.h
 * Read the last several Arduino results from the database,
 * then write them to a CSV file that's accessible by the web app.
 * Called by Greenhouse each time the database is updated,
 * so the web app always shows the latest data.
 * 
 * @todo 
 * @see      Greenhouse
 */


#ifndef __Greenhouse__DataService__
#define __Greenhouse__DataService__

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sqlite3.h>

using std::fstream;


#define DATA_FILENAME "../public_html/data.csv" /// publicly-accessible file
#define TOTAL_RESULTS 1000 /// to be redone as a variable

class DataService {
    
public:
	DataService();
    ~DataService();

   /**
    * Read the latest results from the DB
    * and create the CSV file
    */	
	void refreshDataService();
	
private:
	
   /**
    * Callback for when SQL data is returned;
    * write the returned rows to the file
    */	
	static int sql_callback(void *NotUsed, int argc, char **argv, char **azColName);

};


#endif /* defined(__Greenhouse__DataService__) */

//
//  DataService.h
//  DataService
//
//  this writes a static json file to the public_html directory
//  and is called every time the db is updated
//

#ifndef __Greenhouse__DataService__
#define __Greenhouse__DataService__

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sqlite3.h>

using std::fstream;


#define DATA_FILENAME "../public_html/data.csv"
#define TOTAL_RESULTS 1000

class DataService {
    
public:
	DataService();
    ~DataService();
	void refreshDataService();
	
private:
	static int sql_callback(void *NotUsed, int argc, char **argv, char **azColName);

};


#endif /* defined(__Greenhouse__DataService__) */

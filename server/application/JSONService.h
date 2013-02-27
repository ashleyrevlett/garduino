//
//  JSONService.h
//  JSONService
//
//  this writes a static json file to the public_html directory
//  and is called every time the db is updated
//

#ifndef __Greenhouse__JSONService__
#define __Greenhouse__JSONService__

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sqlite3.h>

using std::fstream;

#define JSON_FILENAME "../public_html/data.csv"
#define TOTAL_RESULTS 1000

class JSONService {
    
public:
	JSONService();
    ~JSONService();
	
private:
	static int sql_callback(void *NotUsed, int argc, char **argv, char **azColName);
	

};



#endif /* defined(__Greenhouse__JSONService__) */

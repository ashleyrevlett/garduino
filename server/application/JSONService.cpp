//
//  JSONService.cpp
//  JSONService
//

#include "JSONService.h"


JSONService::JSONService () { 

	// select most recent 100 db entries and write them to a json file
	const char * db_file = "database/greenhouse3.db";	
	
    // prepare for db connection
    sqlite3 *db;
    int returnCode;
    char *zErrMsg = 0;
     
    // compose sql command
    const char* sql_code = "SELECT * FROM stats_table LIMIT 100;";     	 
     
    returnCode = sqlite3_open(db_file, &db);
    if( returnCode ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
		// exit(1);
    }
     
    returnCode = sqlite3_exec(db, sql_code, sql_callback, 0, &zErrMsg);
    if( returnCode!=SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    
   sqlite3_close(db);
}

int JSONService::sql_callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	// write a json (or for now a csv file) to the public_html dir
    for(int i=0; i<argc; i++){
		if (argv[i]) {
			std::fstream json_file;
			json_file.open(JSON_FILENAME, fstream::in | fstream::out | fstream::app);
			json_file << argv[i] << ", ";
			json_file.close();

		};
		if (i==argc-1) {
			std::fstream json_file;
			json_file.open(JSON_FILENAME, fstream::in | fstream::out | fstream::app);
			json_file << argv[i] << "\n";
			json_file.close();			
		};
    };
    return 0;
	
}




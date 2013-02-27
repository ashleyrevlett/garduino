//
//  DataService.cpp
//  DataService
//

#include "DataService.h"


DataService::DataService () { 

	// select most recent 100 db entries and write them to a json file
	const char * db_file = "database/greenhouse3.db";	
	
    // prepare for db connection
    sqlite3 *db;
    int returnCode;
    char *zErrMsg = 0;
     
    // compose sql command
	char buffer[50];
	sprintf(buffer, "SELECT * FROM stats_table LIMIT %d;", TOTAL_RESULTS);     	 
	const char* sql_code = buffer;
     
    returnCode = sqlite3_open(db_file, &db);
    if( returnCode ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
		exit(1);
    }
     
	// clear the existing file
	std::fstream json_file;
	json_file.open(JSON_FILENAME, fstream::out | fstream::trunc);
	json_file << "date,temp,humidity,light,soil\n";
	json_file.close();
	 
	// write each row
    returnCode = sqlite3_exec(db, sql_code, sql_callback, 0, &zErrMsg);
    if( returnCode!=SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    
   sqlite3_close(db);
}

int DataService::sql_callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	std::fstream json_file;
	
	// append a line to the file for each row
    for(int i=0; i<argc; i++){
		if (argv[i] && (i!=argc-1)) {
			json_file.open(JSON_FILENAME, fstream::in | fstream::out | fstream::app);
			json_file << argv[i] << ",";
			json_file.close();

		};
		if (i==argc-1) {
			json_file.open(JSON_FILENAME, fstream::in | fstream::out | fstream::app);
			json_file << argv[i] << "\n";
			json_file.close();			
		};
    };
    return 0;
	
}




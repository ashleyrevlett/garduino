//
//  Greenhouse.cpp
//  Greenhouse
//

#include "Greenhouse.h"


Greenhouse::Greenhouse () {
    humidity = 0;
}

Greenhouse::~Greenhouse() {
}

void Greenhouse::printReadings() {
    std::cout << "Local time at the greenhouse is " << this->getTime() << std::endl;
    std::cout << "Temp: " << this->getTemp() << " degrees F" << std::endl;
    std::cout << "Humidity: " << this->getHumidity() << " humidity" << std::endl;
    std::cout << "Light: " << this->getLumens() << " lux" << std::endl;
}

void Greenhouse::createCSV() {
    FILE * pFile;
    pFile = fopen(CSV_FILE, "a+");
    if (pFile!=NULL) {
	fprintf(pFile, "%ld, %.2f, %.2f\r\n", this->getTime(), this->getTemp(), this->getLumens() );
	fclose(pFile);
    } else
	    std::cout << "unable to open file";
}

time_t Greenhouse::getTime() {
  	time_t epoch_time;
	time(&epoch_time);
    
	return epoch_time;
	
}

size_t Greenhouse::curl_write( void *ptr, size_t size, size_t nmemb, void *stream) {
    
    struct write_result *result = (struct write_result *)stream;
    
    if(result->pos + size * nmemb >= BUFFER_SIZE - 1) {
        fprintf(stderr, "curl error: too small buffer\n");
        return 0;
    }
    
    memcpy(result->data + result->pos, ptr, size * nmemb);
    
    result->pos += size * nmemb;
    
    return size * nmemb;
}


void Greenhouse::refreshReadings() {
    
    // connect to arduino or testing file (csv)    
    std::cout << "Starting curl..." << std::endl;
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    
    if(curl) {
        
        std::cout << "Curl initialized..." << std::endl;
        
        // Create the 200-char write buffer for curl
	char * data;
	data = (char*) malloc (sizeof(char) * 200);

	struct write_result write_result;
	write_result.data = data;
	write_result.pos = 0;
	
	curl_easy_setopt(curl, CURLOPT_URL, ARDUINO_URL); // specify url
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // allow redirects
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L ); // no progress bar:
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write); // callback function
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &write_result); // where to save data
	
	std::cout << "Curl options set, prepared to perform" << std::endl;
	res = curl_easy_perform(curl);
	
	if(res != CURLE_OK)
	    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

	// clean up
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	std::cout << "cURL cleaned up succesfully" << std::endl;

	// prepare to split the results up
	std::cout << "Processing results..." << std::endl;
	// data[write_result.pos] = '\0'; // null terminate the result
	std::string data_container[3];
	std::string data_string(data); // copy data into string for convenience
	char delimiter[] = ",";
	char delimiter2[] = "\n";
	int start_pos = 0;
	int end_pos = 0;
	int n = 0;
	
	// 3 readings to find in results
	while (n < 3) {
	    
	    for (unsigned int i=start_pos; i < data_string.length(); i++) {
		
		// search for comma or newline
		if ( (strncmp( &data[i], &delimiter2[0], 1) == 0) || 
		     (strncmp( &data[i], &delimiter[0], 1) == 0)) 
		{
		    // split when found, store result
		    end_pos = i - start_pos;
		    data_container[n] = data_string.substr(start_pos, end_pos);
		    // prepare for next loop
		    start_pos = i+1;
		    n++;
            break;
		}
		
	    }
	}

	// set vars to found data
	std::cout << "Recording data..." << std::endl;
	this->lumens = ::atof(data_container[1].c_str());
	this->temp = ::atof(data_container[2].c_str());
	
	// record readings to db
	int result = this->recordReadings();
	std::cout << "DB Record readings result: " << result << std::endl;

    // free(data);

   }

};

int Greenhouse::recordReadings() {

     // prepare for db connection
     sqlite3 *db;
     int returnCode;
     const char *db_file = DB_FILE;
     char *zErrMsg = 0;
     
     // compose sql command
     unsigned int timeString = this->getTime();
     int n;
	 int tempHumidity = 0;
     char buffer[144];
     n = sprintf(
         buffer,
         "insert into TABLE_NAME (epochtime, temp, humidity, lux ) values (%d, %3.2f, %d, %3.2f);",
         timeString, this->getTemp(), tempHumidity, this->getLumens()
     );
     
     const char *sql_code = (const char *)buffer;
     
     std::cout << "SQL code: " << buffer << std::endl;
     
     returnCode = sqlite3_open(db_file, &db);
     if( returnCode ){
         fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
         sqlite3_close(db);
         return(1);
     }
     
     returnCode = sqlite3_exec(db, sql_code, sql_callback, 0, &zErrMsg);
     if( returnCode!=SQLITE_OK ){
         fprintf(stderr, "SQL error: %s\n", zErrMsg);
         sqlite3_free(zErrMsg);
     }
    
    sqlite3_close(db);
    
    return returnCode;
    
}

int Greenhouse::sql_callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}


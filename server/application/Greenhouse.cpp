//
//  Greenhouse.cpp
//  Greenhouse
//

#include "Greenhouse.h"


Greenhouse::Greenhouse ( const char * ipAddress ): arduinoIP( ipAddress )  
{ 
	humidity = 0; 
}


Greenhouse::~Greenhouse() 
{}


void Greenhouse::refreshReadings() 
{
    
    // connect to arduino or testing file (csv)    
    std::cout << "Connecting to: " << this->getArduinoIP() << std::endl;	
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    
    if(curl) {
                
	    // Create the 200-char write buffer for curl
		char * data;
		data = (char*) malloc (sizeof(char) * 200);

		struct write_result write_result;
		write_result.data = data;
		write_result.pos = 0;

		const char * ip =  this->getArduinoIP();
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(curl, CURLOPT_URL, ip ); // specify url
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // allow redirects
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L ); // no progress bar:
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write); // callback 
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &write_result);
		
		res = curl_easy_perform(curl);
	
		if(res != CURLE_OK)
		    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		// clean up
		curl_easy_cleanup(curl);
		curl_global_cleanup();

		// prepare to split the results up
		data[write_result.pos] = '\0'; // null terminate the result
		std::string data_container[4];
		std::string data_string(data); // copy data into string for convenience
		char delimiter[] = ",";
		char delimiter2[] = "\n";
		int start_pos = 0;
		int end_pos = 0;
		int n = 0;
	
		// 4 readings to find in results
		while (n < 4) {
	    
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

	// set vars to found data & record to db
	std::cout << "Recording to database..." << std::endl;
	this->lumens = ::atof(data_container[1].c_str());
	this->temp = ::atof(data_container[2].c_str());
	this->soil_moisture = ::atof(data_container[3].c_str());
	int result = this->recordReadings();

    // free(data);

   }

}


int Greenhouse::recordReadings() 
{

     // prepare for db connection
     sqlite3 *db;
     int returnCode;
     const char *db_file = DB_FILE;
     char *zErrMsg = 0;
     
     // compose sql command
     unsigned int timeString = this->getTime();
     int n;
	 int tempHumidity = 0;
     char buffer[256];
     n = sprintf(buffer,
         "CREATE TABLE IF NOT EXISTS stats_table (id int primary key,epochtime int,temp real,humidity real,lux real, soil_moisture real); INSERT into stats_table (epochtime, temp, humidity, lux, soil_moisture) values (%d, %3.2f, %d, %3.2f, %3.2f);",
         timeString, this->getTemp(), tempHumidity, this->getLumens(), this->getSoilMoisture()
     );
     
     const char *sql_code = (const char *)buffer;
         
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



time_t Greenhouse::getTime() 
{
  	time_t epoch_time;
	time(&epoch_time);
    
	return epoch_time;
	
}


void Greenhouse::printReadings() 
{
    std::cout << "Local time: " << this->getTime() << std::endl;
    std::cout << "Temp: " << this->getTemp() << " degrees F" << std::endl;
    std::cout << "Humidity: " << this->getHumidity() << " humidity" << std::endl;
    std::cout << "Light: " << this->getLumens() << " lux" << std::endl;
	std::cout << "Soil Moisture: " << this->getSoilMoisture() << " out of 100" << std::endl;	
}

size_t Greenhouse::curl_write( void *ptr, size_t size, size_t nmemb, void *stream) 
{
    
    struct write_result *result = (struct write_result *)stream;
    
    if(result->pos + size * nmemb >= BUFFER_SIZE - 1) {
        fprintf(stderr, "curl error: too small buffer\n");
        return 0;
    }
    
    memcpy(result->data + result->pos, ptr, size * nmemb);
    
    result->pos += size * nmemb;
    
    return size * nmemb;
}


int Greenhouse::sql_callback(void *NotUsed, int argc, char **argv, char **azColName)
{
/*    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
*/
    return 0;
}



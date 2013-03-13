/**
 * @package	Garduino
 * @file 	main.cpp
 * Regularly updates a database with the Arduino's readings,
 * and updates the CSV file that's used by the web application
 * to display the latest readings.
 * 
 * Requires: garduino.ino program installed and running prior 
 * on the Arduino prior to running this app.
 *
 * @return      void; infinite loop
 */
	 
#include <iostream>
#include <unistd.h>
#include <string.h>
#include "Greenhouse.h"
#include "DataService.h"

#define SLEEP_TIME 120 // in seconds


int main(int argc, char* argv[]) 
{

	// make sure command line parameter is correct
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " [ARDUINO_URL]" << std::endl;
		std::cerr << "// where [ARDUINO_URL] is 'http://' + IP Address" << std::endl;
		return 1;
	}

	// create new Greenhouse and check on it forever
    Greenhouse *gh = new Greenhouse ( argv[1] );    
    while (1) {

		// update the readings and record to DB
        gh->refreshReadings();
        gh->printReadings();
				
		// write data file for website
		DataService * service = new DataService();
		service->refreshDataService();

		// sleep
        std::cout << "Sleeping for " << SLEEP_TIME << " seconds." << std::endl << std::endl;
        sleep(SLEEP_TIME);
        
    };
    
	return 0;
	
}

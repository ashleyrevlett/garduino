// main.cpp
// garduino

// Arduino IP address is passed as parameter

#include <iostream>
#include <unistd.h>
#include <string.h>
#include "Greenhouse.h"
#include "JSONService.h"

int main(int argc, char* argv[]) 
{

	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " [ARDUINO_URL]" << std::endl;
		std::cerr << "// where [ARDUINO_URL] is 'http://' + IP Address" << std::endl;
		return 1;
	}

    Greenhouse *gh = new Greenhouse ( argv[1] );
    
    while (1) {

		// update the readings and record to DB
        gh->refreshReadings();
        gh->printReadings();		
		// write JSON file for website
		JSONService *json = new JSONService();

        std::cout << "Sleeping for 15 seconds." << std::endl << std::endl;
        sleep(15);
        
    };
    
	return 0;
	
}

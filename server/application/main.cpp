// main.cpp
// garduino


#include <iostream>
#include <unistd.h>
#include "Greenhouse.h"

int main() {
    
    Greenhouse *gh = new Greenhouse;
    
    while (1) {
        
        gh->refreshReadings();
        gh->printReadings();
        gh->createCSV();

        std::cout << "Sleeping for 15 seconds." << std::endl << std::endl;
        sleep(15);
        
    };
    
	return 0;
	
}

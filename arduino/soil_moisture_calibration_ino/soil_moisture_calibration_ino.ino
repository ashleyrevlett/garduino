/*
  Web Server
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 Based on Web Server code from Arduino.cc:
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 adapted Feb 2013
 by Ashley Revlett

PINS: 
A0: light sensor
A1: soil moisture level (0-1020)
A3: temperature
 
 */

#include <SPI.h>
#include <Ethernet.h> 
#include <Time.h>

const int LIGHT_PIN = A0;
const int SOIL_PIN = A1;
const int TEMP_PIN = A3;

unsigned long time;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1, 191);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connnection: close");
          client.println();
                    
          // output time since arduino started running
          time_t now = now();
          time = int(now);
          client.print( time );
          client.print( "," );

          // read 3 light levels and average them (reduce noise)          
          int lightReading = analogRead(LIGHT_PIN);
          int lightReading2 = analogRead(LIGHT_PIN);		  
          int lightReading3 = analogRead(LIGHT_PIN);		  		  
		  lightReading = (lightReading + lightReading2 + lightReading3) / 3;
          lightReading = map(lightReading, 0, 1023, 100, 0);          
          client.print(lightReading);
          client.print(",");

          // read 3 soil moisture levels
          int soilReading = analogRead(SOIL_PIN);
          int soilReading2 = analogRead(SOIL_PIN);		  
          int soilReading3 = analogRead(SOIL_PIN);		  		  
		  soilReading = (soilReading + soilReading2 + soilReading3) / 3;
          soilReading = map(soilReading, 0, 1023, 0, 100);          
          client.print(soilReading);
          client.print(",");		  

          // read 3 temps and avg them                    
          int tempReading = analogRead(TEMP_PIN);
          int tempReading2 = analogRead(TEMP_PIN);
          int tempReading3 = analogRead(TEMP_PIN);		  		  
		  tempReading = (tempReading + tempReading2 + tempReading3) / 3; 
          float voltage = (tempReading * 5.0) / 1024.0;         
          float temperatureC = (voltage - 0.5) * 100;          
          float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;          
          client.print(temperatureF);
          client.print("\n");      
          client.println();
              
          break;
          
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    
    // give the client time to receive the data
    delay(5);
    
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
   
  }
}



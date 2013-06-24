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
D2: DHT22 temperature sensor connected to 10K pull-up resistor

OUTPUTS: time_since_boot, light (0-100), temp, soil moisture (0-100)
 
*/

#include <SPI.h>
#include <Ethernet.h> 
#include <Time.h>
#include "DHT.h"

#define DHTTYPE DHT22   // DHT 22  (AM2302)

// sensor pin numbers
const int LIGHT_PIN = A0;
const int SOIL_PIN = A1;
const int TEMP_PIN = 2; // digital; DHT22 sensor w/ temp and humidity

// IP address to assign to Arduino 
IPAddress ip(192,168,1,191);

// MAC address of the Arduino (make one up)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Initialize the Ethernet server library
EthernetServer server(80);

// Initialize the DHT sensor library
DHT dht(TEMP_PIN, DHTTYPE);


void setup() {
  
 // Open serial communications and wait for port to open
  Serial.begin(9600);
  dht.begin();
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

}


float read_humidity() {
  float h1 = dht.readHumidity();
  float h2 = dht.readHumidity();
  float h3 = dht.readHumidity();
  h1 = (h1 + h2 + h3) / 3; 
  return h1;  
}

float read_temperature() {
  float t1 = dht.readTemperature();
  float t2 = dht.readTemperature();
  float t3 = dht.readTemperature();  
  float temperatureC =  (t1 + t2 + t3) / 3; 
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0; 
  return temperatureF;
}

float read_light_level() {
  int lightReading = analogRead(LIGHT_PIN);
  int lightReading2 = analogRead(LIGHT_PIN);      
  int lightReading3 = analogRead(LIGHT_PIN);            
  lightReading = (lightReading + lightReading2 + lightReading3) / 3;
  lightReading = map(lightReading, 0, 1023, 100, 0);  
  return lightReading;     
}

float read_soil_moisture() {
  int soilReading = analogRead(SOIL_PIN);
  int soilReading2 = analogRead(SOIL_PIN);      
  int soilReading3 = analogRead(SOIL_PIN);            
  soilReading = (soilReading + soilReading2 + soilReading3) / 3;
  soilReading = map(soilReading, 0, 1023, 0, 100);     
  return soilReading;
}


void loop() {

  // take the current readings
  time_t now_time = int( now() );
  float lightReading = read_light_level();
  float temperatureF = read_temperature();
  float humidity = read_humidity();
  float soilReading = read_soil_moisture();

  // print current status to serial
  Serial.print("Time:");
  Serial.println(now_time);
  Serial.print("Light:");
  Serial.println(lightReading);
  Serial.print("Temperature:");
  Serial.println(temperatureF);
  Serial.print("Humidity:");
  Serial.println(humidity);  
  Serial.print("Soil:");
  Serial.println(soilReading);  


  // listen for incoming http connection and 
  // serve text file with latest reading to client  
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
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
                 
          client.print( now_time );
          client.print( "," );
          client.print(lightReading);
          client.print(",");
          client.print(temperatureF); 
          client.print(",");          
          client.print(humidity); 
          client.print(",");                        
          client.print(soilReading);
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
  
  // take readings every 5 seconds
  delay(5000);

}



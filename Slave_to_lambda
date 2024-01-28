//upload stuff

#include <WiFi.h> // include the WiFi library
#include <HTTPClient.h> // include the HTTPClient library

//slave stuff
#define RXD2 16 // define the pin number for RXD2
#define TXD2 17 // define the pin number for TXD2
String incoming; // create a String variable "incoming"

const char* ssid = "SixStars"; // set the SSID of the WiFi network
const char* password = "Ilovewael1968"; // set the password of the WiFi network

String serverName = "https://y40cmoim0k.execute-api.us-east-2.amazonaws.com/DepA"; // set the server name to the API deployment link

unsigned long lastTime = 0; 
unsigned long timerDelay = 10000;
unsigned long Uptime; 

//sensor stuff
const int analogPin = A0;
int sensorValue;

void setup() {
 Serial.begin(115200); 
 Serial.println("Started the receiver (black board)"); // print the message "Started the receiver (black board)"
 Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // start the serial communication 
 WiFi.begin(ssid, password); // start the WiFi connection
 
 Serial.println("Connecting"); // print the message "Connecting"
 while (WiFi.status() != WL_CONNECTED) { 
 delay(500); // wait for 500 milliseconds
 Serial.print("."); // print a dot
 }

 Serial.println(""); // print a new line
 Serial.print("Connected to WiFi network with IP Address: "); // print the message "Connected to WiFi network with IP Address: "
 Serial.println(WiFi.localIP()); // print the local IP address of the device

 Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading."); 
}

void loop() {
 while (Serial2.available() > 0) { 
 incoming = Serial2.readStringUntil('\n'); 
 Serial.println(incoming); // print the data that was read
 
 if (WiFi.status() == WL_CONNECTED) { // check if the WiFi status is connected
 String serverPath = serverName + "/?packet=" + (String)incoming; 
 Serial.println(serverPath); // print the server path
 
 HTTPClient http; // create an HTTPClient object "http"
 http.begin(serverPath.c_str()); // initialize the HTTPClient object with the server path
 
 int httpResponseCode = http.GET(); // send a GET request and store the HTTP response code in the "httpResponseCode" variable
 
 if (httpResponseCode > 0) { // check if the HTTP response code is greater than 0
 Serial.print("HTTP Response code: "); 
 Serial.println(httpResponseCode); 
 String payload = http.getString();
 Serial.println(payload);
 } else { //.if it is not greater than 0
 Serial.print("Error code: "); // print the message "Error code: "
 Serial.println(httpResponseCode); // print the HTTP response code
 }
 
 http.end(); // close the HTTP connection
 }
 else { 
 Serial.println("WiFi Disconnected"); // print the message "WiFi Disconnected"
 }
 } 

}

#include "painlessMesh.h"
#include <ArduinoJson.h>

// MESH Details
#define MESH_PREFIX "FireMesh"       // name for your MESH
#define MESH_PASSWORD "Ilovedad100"  // password for your MESH
#define MESH_PORT 5555               // default port
#define RXD2 16
#define TXD2 17
int nodeNumber = 2;

Scheduler userScheduler;  // to control your personal task
painlessMesh mesh;

int flamePin = 34;
int sensorValue = 0;
// General stuff
int gasPin = 35;
int sensorValue1 = 0;
int tempPin = 32;
int sensorValue2 = 0;
int humidPin = 33;
int sensorValue3 = 0;
// User stub
void sendMessage();  // Prototype so PlatformIO doesn't complain

// Create task to send the MQ-135 sensor value
Task taskSendMessage(TASK_SECOND * 5, TASK_FOREVER, &sendMessage);

void sendMessage() {
  sensorValue = analogRead(flamePin);
  sensorValue1 = analogRead(gasPin);
  sensorValue2 = analogRead(tempPin);
  sensorValue3 = analogRead(humidPin);

  // Create a JSON document with the node number and MQ-135 value
  DynamicJsonDocument jsonReadings(200);  // 200 is the capacity of the JSON document
  jsonReadings["node"] = nodeNumber;
  jsonReadings["flame"] = sensorValue;
  jsonReadings["gas"] = sensorValue1;
  jsonReadings["temp"] = sensorValue2;
  jsonReadings["humid"] = sensorValue3;

  // Serialize the JSON document to a string
  String msg;
  serializeJson(jsonReadings, msg);

  // Send the message via the mesh network
  mesh.sendBroadcast(msg);
}

// Needed for painless library
void receivedCallback(uint32_t from, String& msg) {
  Serial.printf("Received from %u msg=%s\n", from, msg.c_str());

  // Parse the received JSON message
  DynamicJsonDocument myObject(200);  // 200 is the capacity of the JSON document, adjust if needed
  deserializeJson(myObject, msg);

  int node = myObject["node"];
  int gasValue = myObject["gas"];
  int flameValue = myObject["flame"];  
  int tempValue = myObject["temp"];
  int humidValue = myObject["humid"];

  //sensor stuff
  float coConcentration = map(gasValue, 0,  500, 0, 1000); 
  float co2Concentration = map(gasValue, 0, 1023, 0, 2000); 

  String toSend = (String)node + "," + (String)coConcentration + "," + (String)co2Concentration + "," + (String)flameValue + "," + (String)tempValue + "," + (String)humidValue + "\n" ; //sending the message to the slave

  Serial2.write(toSend.c_str());
  Serial.println(toSend); //printing the values to the serial monitor

  // Print the received values
  Serial.print("Node: ");
  Serial.println(node);
  Serial.print("Flame reading: ");
  Serial.println(flameValue);
  Serial.print("Gas reading: ");
  Serial.println(gasValue);
  Serial.print("Temp reading: ");
  Serial.println(tempValue);
  Serial.print("Humid reading: ");
  Serial.println(humidValue);
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("New Connection, nodeId = %u\n", nodeId); //saying that their is a new connection
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n"); //their is a changed connection
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset); //adjusted time
}

void setup() {
  Serial.begin(115200);

  // Mesh stuff
  mesh.setDebugMsgTypes(ERROR | STARTUP);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();

  // Add the following lines from the provided setup code
  Serial.println("Started the receiver (black board)");
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
  mesh.update(); //updating the mesh network for the sensor values
 
}

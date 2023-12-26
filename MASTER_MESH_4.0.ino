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

// General stuff
int mq135Pin = 35;
int sensorValue = 0;

// User stub
void sendMessage();  // Prototype so PlatformIO doesn't complain

// Create task to send the MQ-135 sensor value
Task taskSendMessage(TASK_SECOND * 5, TASK_FOREVER, &sendMessage);

void sendMessage() {
  sensorValue = analogRead(mq135Pin);

  // Create a JSON document with the node number and MQ-135 value
  DynamicJsonDocument jsonReadings(200);  // 200 is the capacity of the JSON document, adjust if needed
  jsonReadings["node"] = nodeNumber;
  jsonReadings["mq135"] = sensorValue;

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
  int mq135Value = myObject["mq135"];
   //sensor stuff
  float coConcentration = map(mq135Value, 0,  500, 0, 1000); 
  float co2Concentration = map(mq135Value, 0, 1023, 0, 2000); 

  String toSend = (String)node + "," + (String)coConcentration + "," + (String)co2Concentration +"\n" ;

  Serial2.write(toSend.c_str());
  Serial.println(toSend);

  // Print the received values
  Serial.print("Node: ");
  Serial.println(node);
  Serial.print("MQ-135 Reading: ");
  Serial.println(mq135Value);
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
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
  mesh.update();
  delay(1000);
}

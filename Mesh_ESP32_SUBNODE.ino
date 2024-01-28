#include "painlessMesh.h"
#include <Arduino_JSON.h>

// MESH Details
#define MESH_PREFIX "FireMesh"   // name for my MESH
#define MESH_PASSWORD "Ilovedad100" // password for my MESH
#define MESH_PORT 5555           // default port

// Analog pin where MQ-135 sensor is connected
const int mq135Pin = 35;
int nodeNumber = 10;

// String to send to other nodes with sensor readings
String readings;

Scheduler userScheduler; // to control my personal task
painlessMesh mesh;

// User stub
void sendMessage(); // Prototype so PlatformIO doesn't complain
String getReadings(); // Prototype for sending sensor readings

// Create tasks: to send messages and get readings;
Task taskSendMessage(TASK_SECOND * 5, TASK_FOREVER, &sendMessage);

String getReadings() {
  JSONVar jsonReadings;
  jsonReadings["node"] = nodeNumber;
  
  // Read analog value from MQ-135 sensor
  int FlameValue = analogRead(34);
  jsonReadings["flame"] = FlameValue;
  int mq135Value = analogRead(mq135Pin);
  jsonReadings["gas"] = mq135Value;  
  int tempValue = analogRead(32);
  jsonReadings["temp"] = tempValue;
  int humidValue = analogRead(33);
  jsonReadings["humid"] = humidValue;

  readings = JSON.stringify(jsonReadings);
  return readings;
}

void sendMessage() {
  String msg = getReadings();
  mesh.sendBroadcast(msg);
}

void receivedCallback(uint32_t from, String &msg) {
  Serial.printf("Received from %u msg=%s\n", from, msg.c_str());
  JSONVar myObject = JSON.parse(msg.c_str());
  int node = myObject["node"];
  Serial.print("Node: ");
  Serial.println(node);
  int FlameValue_1 = myObject["flame"];
  Serial.println(FlameValue_1);
  int mq135Value = myObject["gas"];  
  Serial.println(mq135Value);
  int TempValue_1 = myObject["temp"];
  Serial.println(TempValue_1);
  int HumidValue_1 = myObject["humid"];
  Serial.println(HumidValue_1);
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

  mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  //mesh.setDebugMsgTypes(ERROR | STARTUP); // set before init() so that you can see startup messages

  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();  
}

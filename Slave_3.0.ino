#define RXD2 16
#define TXD2 17

String incoming;

void setup() {
  Serial.begin(115200);
  Serial.println("Started the receiver (black board");
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() { 
  while (Serial2.available() >= 1) {
    //Serial.print(char(Serial2.read()));
    incoming = Serial2.readStringUntil('\n');
    Serial.println(incoming);
  }
}
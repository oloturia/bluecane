#include <SoftwareSerial.h>
SoftwareSerial gpsSerial(8,9); // RX, TX for the GPS module
/*
 Read string $GPGLL from GPS, formats the string so it can be pasted
 on Google Maps http://maps.google.com?q=<string> and outputs in the
 hw serial.

 This code is in the public domain.
*/


void setup() {
  Serial.begin(9600);  //the hardware serial can be connected to BT
  gpsSerial.begin(9600);
 // Serial.println("Ready");
}

void loop() {
      //if (gpsSerial.available()) {
      Serial.write(gpsSerial.read());
      //}
}

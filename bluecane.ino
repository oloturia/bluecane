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
  Serial.println("Ready");
}

void loop() {
  char serChar;
  while(Serial.available()){
    if (Serial.read() == *"R") {
      recgps();
    }
  }
}

void recgps() {
  String gpsDecode = ""; //every single char of the GPS is stored in
this string, when it senses that a string is starting then
  String latitude = "";  //stores the chars in the right variable.
  String longitude = "";
  char gpsChar;          //the single char from the GPS module
  boolean dollar = false;//every new line from GPS starts with a dollar
  boolean gpgll = false; //the GPS outputs a lot of info, but we need
just long and lat, $GPGLL line is the shortest
  int xind;              //an index
  while(gpsSerial.available()){
      gpsChar = gpsSerial.read();
      if (gpsChar == *"$") {      //senses a dollar
        dollar = true;
      }
      if (dollar) {
        gpsDecode.concat(gpsChar);
      }
      if ((gpsDecode.length() == 6) && (gpgll == false)) {
        if(gpsDecode == "$GPGLL"){  //it's the $GPGLL string
          xind = 0;
          gpgll = true;
        } else {
          gpsDecode = "";
          dollar = false;
        }
      }
      if (gpgll) {
        if ((xind >=2) && (xind <= 11)) { //latitude chars, from 2 to
11 ("$GPGLL" excluded)
          latitude.concat(gpsChar);
          if (xind == 3) {
            latitude.concat(*" ");
          }
        }
        if (xind == 13) {
          if (gpsChar == *"S") {          //if in the southern
hemisphere, puts the minus sign
            latitude = "-"+latitude;
          }
        }
        if ((xind >=15) && (xind <=25)) { //longitude stays between
the 15th and the 25th char
          longitude.concat(gpsChar);
          if (xind == 17) {
            longitude.concat(*" ");
          }
        }
        if (xind == 27) {
            if (gpsChar == *"W") {        //it's negative in the
western hemisphere
              longitude = "-"+longitude;
            }
        }
        if (xind == 28) {                //28 there are more things in
the GPS' output, like clock and crc, but they are no use
          Serial.print(latitude);       //for our purpose, at the moment
          Serial.print(",");
          Serial.println(longitude);
          gpgll = false;
          dollar = false;
          gpsDecode = "";
          latitude = "";
          longitude = "";
          xind = 0;
          return;
        } else {
          xind +=1;
        }
      }
  }
}

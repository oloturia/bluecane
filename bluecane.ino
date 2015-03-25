
/*
 Read string $GPRMC from GPS, formats the string so it can be pasted
 on Google Maps http://maps.google.com?q=<string> and outputs in the
 hw serial. The format of latitude is DD MM.MMMM and longitude is DDD MM.MMMM
 This code is in the public domain.
*/


#include <SoftwareSerial.h>
SoftwareSerial gpsSerial(8,9); // RX, TX for the GPS module

void setup() {
  Serial.begin(9600);  //the hardware serial can be connected to BT
  gpsSerial.begin(9600);
  Serial.println("Ready");
}

/*
void loop() {
      if (gpsSerial.available()) {
        Serial.write(gpsSerial.read());
      }
}
*/
/*
void loopdisabled1(){
  String gpsDecode = "";
  char gpsChar;
  boolean dollar = false;
  boolean nmea_valid = false;
  while (true){
    while(gpsSerial.available()) {
      gpsChar = gpsSerial.read();
      if (gpsChar == *"$") {
        Serial.print(gpsDecode);
        gpsDecode = "";
        nmea_valid = false;
        dollar = true;
      }
      if (dollar) {
        gpsDecode.concat(gpsChar);
      }
      if ((gpsDecode.length() == 6) && nmea_valid == false) {
        if(gpsDecode != "$GPTXT") {
          nmea_valid = true;
        } else {
          dollar = false;
          gpsDecode = "";
        }
      }
    }
  }
}*/


void loop() {
  String gpsDecode = ""; //every single char of the GPS is stored in this string, when it senses that a string is starting then
  String latitude = "";  //stores the chars in the right variable. 
  String longitude = "";
  char gpsChar;          //the single char from the GPS module
  boolean dollar = false;//every new line from GPS starts with a dollar
  boolean gprmc = false; //
  int xind;              //an index
  while(true) {
    delay(10);
    while(gpsSerial.available()){
         if (gprmc) {
          if (gpsChar == *"$") {

            Serial.print(gpsDecode);
            dollar = false;
            gpsDecode = "";
            gprmc = false;
          }
        }
        gpsChar = gpsSerial.read();
        if (gpsChar == *"$") {      //senses a dollar
          dollar = true;
        }
        if (dollar) {
          gpsDecode.concat(gpsChar);
        }
        if ((gpsDecode.length() == 6) && (gprmc == false)) {
          if(gpsDecode == "$GPRMC"){  
            xind = 0;
            gprmc = true;
          } else {
            gpsDecode = "";
            dollar = false;
          } 
        }

        if (gprmc) {
          if ((xind >=14) && (xind <= 23)) { //latitude chars, from 13 to 22 ("$GPRMC excluded)
            latitude.concat(gpsChar);
            if (xind == 15) {                //a space separates degrees to minutes
              latitude.concat(*"+");
            }
          }
          if (xind == 25) {
            if (gpsChar == *"S") {          //if in the southern hemisphere, puts the minus sign
              latitude = "-"+latitude;
            } else {
              latitude = "+"+latitude;
            }
          }
          if ((xind >=27) && (xind <=37)) { //longitude stays between the 26th and the 36th char
            longitude.concat(gpsChar);
            if (xind == 29) {
              longitude.concat(*"+");
            }          
          }
          if (xind == 39) {
              if (gpsChar == *"W") {        //it's negative in the western hemisphere
                longitude = "-"+longitude;
              } else {
                longitude = "+"+longitude;
              }
          }
          if (xind == 39) {               //28 there are more things in the GPS' output, like clock and crc, but they are no use
            Serial.print("$");            //for our purpose, at the moment, dollar is for marking the start of the line
            Serial.print(latitude);
            Serial.print(",");
            Serial.print(longitude);
            gprmc = false;
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
}

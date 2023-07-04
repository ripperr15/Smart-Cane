#include <TinyGPS.h>
#include <SoftwareSerial.h>

const int trigPin = 9;
const int echoPin = 10;
const int vibration = 6;
int state = 0;
const int pin = 7;
float gpslat, gpslon;
TinyGPS gps;
SoftwareSerial sgps(4, 5);
SoftwareSerial sgsm(2, 3);
static const uint32_t GPSBaud = 9600; 

long duration;
int distanceCm, distanceInch;
void setup()
{ 

 Serial.begin(9600); 
  sgsm.begin(9600);
  sgps.begin(9600);

 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(vibration, OUTPUT);


 pinMode(6, OUTPUT); // Connect  Vibration Pin D6
 pinMode(11, OUTPUT); // Connect Buzzer Pin D11

}
void loop()
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distanceCm= duration*0.034/2;
distanceInch = duration*0.0133/2;
//Serial.println("Distance: ");
//Serial.println(distanceCm);
delay (100);
// See the Ultrasonic Sensor Value in Serial Monitor

  
if(distanceCm < 25)  // You can Change the value 
{
  
  digitalWrite(11, HIGH);  // Buzzer ON
  digitalWrite(6, HIGH);  // Vibratiom ON 
 
}

else
{
    digitalWrite(11,LOW);  // Buzzer OFF
    digitalWrite(6,LOW);  // Vibraton OFF 
}
sgps.listen();
  while (sgps.available())
  {
    int c = sgps.read();
    if (gps.encode(c))
    {
      gps.f_get_position(&gpslat, &gpslon);
    }
  }
sgsm.listen();
    if (sgsm.available() > 0) {
      String    c = sgsm.readString();
      c.trim();
      if (c.indexOf("GET-GPS") >= 0) {
        sgsm.print("\r");
        delay(1000);
        sgsm.print("AT+CMGF=1\r");
        delay(1000);
        /*Replace XXXXXXXXXX to 10 digit mobile number &
          ZZ to 2 digit country code*/
        sgsm.print("AT+CMGS=\"+014374213663\"\r");
        delay(1000);
        //The text of the message to be sent.
      sgsm.print("Smart Cane Location - ");
      sgsm.print("https://www.google.com/maps/?q=");
      sgsm.print(gpslat, 6);
      sgsm.print(",");
      sgsm.print(gpslon, 6);
        delay(1000);
        sgsm.write(0x1A);
        delay(1000);
      }
  delay(100);
    }
}




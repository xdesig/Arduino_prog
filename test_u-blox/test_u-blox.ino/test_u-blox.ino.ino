
/* XDeSIG 2016
  This sample code demonstrates the simple use of a u-blox http://bit.ly/28Z8vFb
  GPS SHIELD IteadStudio http://wiki.iteadstudio.com/Arduino_GPS_shield
  and u-center sotware,  https://www.u-blox.com/en/evaluation-software-and-tools.
  It requires the use a Arduino Leonardo or use SotwareSerial https://www.arduino.cc/en/Reference/SoftwareSerial
*/


void setup()
{
  Serial.begin(9600); //comunication to u-center
  
  Serial1.begin(38400); // gps 
 
}

void loop()
{
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial1.available())
    {
      char g = Serial1.read();
       Serial.write(g); 
    }
  }
}

#include <Wire.h>
#define I2CAddressESPWifi 6

char response[11] ="0123456789";

void setup()
{
  Serial.begin(115200);
  Wire.begin(I2CAddressESPWifi);
  Wire.onReceive(espWifiReceiveEvent);
  Wire.onRequest(espWifiRequestEvent);
}

void loop()
{
  delay(1);
}

void espWifiReceiveEvent(int count)
{
  Serial.print("Received[");
  while (Wire.available())
  {
    char c = Wire.read();
    Serial.print(c);
  }
  Serial.println("]");
  //calc response.
  String ts = getTime();
  strncpy(response,ts.c_str(),sizeof(response)-1);
}

void espWifiRequestEvent()
{
  /*Serial.print("Sending:[");
  Serial.print(response);
  Serial.println("]");*/
  Wire.write(response);
}

String getTime(){
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  String ts = "U:";
  if (hr<10) ts+="0";
  ts += hr;
  ts +=":";
  if ((min%60)<10) ts+="0";
  ts += min % 60;
  ts +=":";
  if ((sec%60)<10) ts+="0";
  ts += sec % 60;
  return(ts);
}

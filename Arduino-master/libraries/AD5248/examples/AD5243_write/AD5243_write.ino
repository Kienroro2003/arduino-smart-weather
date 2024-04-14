//
//    FILE: AD5243_write.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo
//     URL: https://github.com/RobTillaart/AD5248


#include "AD5248.h"

AD5243 AD01;


void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println(__FILE__);
  Serial.print("AD5248_LIB_VERSION: ");
  Serial.println(AD5248_LIB_VERSION);

  Wire.begin();
  Wire.setClock(400000);

  bool b = AD01.begin();
  Serial.println(b ? "true" : "false");
  Serial.println(AD01.isConnected());
}


void loop()
{
  for (int val = 0; val < 255; val++)
  {
    AD01.write(1, val);
    AD01.write(0, 255 - val);
    Serial.println(val);
    delay(20);
  }
}


//  -- END OF FILE --

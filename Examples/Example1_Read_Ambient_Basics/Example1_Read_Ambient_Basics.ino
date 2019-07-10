/*
  This example code will walk you through how to read ambient light values.
  Chances are good that you'll use this sensor in various environments so it'll
  also walk you through setting the gain and integration time that allow for
  different ranges of lux values. For example using the default gain of 100ms
  gives you a maximum reading of 30,199 Lux. This is great for daylight
  readings but not DIRECT sun. 
  
  SparkFun Electronics
  Author: Elias Santistevan
  Date: July 2019

	License: This code is public domain but if you use this and we meet someday, get me a beer! 

	Feel like supporting our work? Buy a board from Sparkfun!
	https://www.sparkfun.com/products/15436

*/

#include <Wire.h>
#include "SparkFun_VEML6030_Ambient_Light_Sensor.h"

#define AL_ADDR 0x48

SparkFun_Ambient_Light light(AL_ADDR);

// Possible values: .125, .25, 1, 2
// Both .125 and .25 should be used in most cases except darker rooms.
// A gain of 2 should only be used if the sensor will be covered by a dark
// glass.
float gain = .125;

// Possible integration times: 800, 400, 200, 100, 50, 25
int time = 800;
long luxVal = 0; 

void setup(){

  Wire.begin();
  Serial.begin(115200);

  if(light.begin())
    Serial.println("Ready to read some light!"); 
  else
    Serial.println("Could not communicate with the sensor!");

  light.setGain(gain);
  light.setIntegTime(time);

  Serial.println("Reading settings..."); 
  Serial.print("Gain: ");
  float gainVal = light.readGain();
  Serial.print(gainVal, 3); 
  Serial.print(" Integration Time: ");
  int timeVal = light.readIntegTime();
  Serial.println(timeVal);

}

void loop(){

  luxVal = light.readLight();
  Serial.print("Ambient Light Reading: ");
  Serial.print(luxVal);
  Serial.println(" Lux");  
  delay(1000);

}

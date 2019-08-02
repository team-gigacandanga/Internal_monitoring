#include <Wire.h>
#include <BH1750.h>

//Define o sensor BH1750
BH1750 lightMeter;

void setup() {

 Serial.begin(115200);
 Wire.begin();
 lightMeter.begin();

}

void loop() {

  uint16_t lux = lightMeter.readLightLevel();

  Serial.print("Luminosidade: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);
  

}

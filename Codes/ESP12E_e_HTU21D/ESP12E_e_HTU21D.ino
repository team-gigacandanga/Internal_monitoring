#include <SparkFunHTU21D.h>
#include <Wire.h>


//Definindo o sensor HTU21D

HTU21D htu21d;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  htu21d.begin();

}

void loop() {
  float umd = htu21d.readHumidity();
  float temp = htu21d.readTemperature();

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" || Umd: ");
  Serial.println(umd);

}

/*
Brasília, 25 de Julho de 2019. [P&D]Gigacandanga 
SMCAI V1.0 - Sistema de Monitoramento e Controle para Ambientes Internos 

[Microcontrolador] ESP12E;
[Sensores] BH1750 (Luminosidade), HTU21D (Temperatura e Umidade), HC-SR501 (Detector de Presença), TSOP4838 (Receptor IR);
[Atuadores] BT134-600E (Controlador de cargas AC), IRF840 (Controlador de Cargas DC), TIL32 (Emissor IR);

---------------------
Atualização V1.1
Autor: Ítalo Rodrigo 
DATA: 06/08/2019
---------------------
*/
#include <Wire.h>
#include <SparkFunHTU21D.h>
#include <BH1750.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>


//Declarando os Objetos
HTU21D htu21d;  //Definindo o sensor de Temperatura e Umidade
BH1750 GY30;    //Definindo o sensor de luminosidade

Ticker TimmerLuz;
Ticker TimmerUmd;
Ticker TimmerTemp;

//Variáveis Globais
float umd=0;
float temp=0; 
float lux=0;

void MedirLuz()
{ 
  Serial.print("Lux: ");
  Serial.print(lux);
  Serial.println(" lux");

}

void MedirUmd()
{ 
  Serial.print("Umidade: ");
  Serial.print(umd);
  Serial.println(" %");
}

void MedirTemp()
{ 
  Serial.print("Temperatura: ");
  Serial.print(temp);
  Serial.println(" C");
}



void setup() {
  Serial.begin(115200);
  Wire.begin();
  htu21d.begin();
  GY30.begin();
  
  TimmerUmd.attach(1,MedirUmd);
  TimmerTemp.attach(1,MedirTemp);
  TimmerLuz.attach(1,MedirLuz);
    
}

void loop() {
  //Leitura dos Sensores
  //Atualizando o Bus do I2C para Temp & Umd   
  umd= htu21d.readHumidity();
  temp = htu21d.readTemperature();
  lux = GY30.readLightLevel();
  delay(100); // 100 ms é o atraso mínimo, menor que isso o I2C não funcionará em perfeito estado.

}

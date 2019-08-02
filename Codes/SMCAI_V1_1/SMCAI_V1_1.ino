/*
Brasília, 25 de Julho de 2019. [P&D]Gigacandanga 
SMCAI V1.0 - Sistema de Monitoramento e Controle para Ambientes Internos 

[Microcontrolador] ESP12E;
[Sensores] BH1750 (Luminosidade), HTU21D (Temperatura e Umidade), HC-SR501 (Detector de Presença), TSOP4838 (Receptor IR);
[Atuadores] BT134-600E (Controlador de cargas AC), IRF840 (Controlador de Cargas DC), TIL32 (Emissor IR);

*/
#include <Wire.h>
#include <SparkFunHTU21D.h>
#include <BH1750.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//Definições do MQTT
#define TOP_PUB "GC/SMCAI/Sensor"
#define TOP_SUB "GC/SMCAI/Comand"
#define ID_MQTT "GC_SMCAI_1"

//WiFi
const char* SSID = "Gigacandanga";
const char* PASSWORD = "Aldebaran2@18noc316316";
WiFiClient esp12e;
PubSubClient MQTT(esp12e);

//Broker (Servidor MQTT)
const char* BROKER_MQTT = "iot.eclipse.org";
int BROKER_PORT = 1883;

//Definindo o sensor de Temperatura e Umidade
HTU21D htu21d;
//Definindo o sensor de luminosidade
BH1750 GY30;

//Subrotinas
void conectaWiFi();
void conectaMQTT();
void instruction(char* topic, byte* payload, unsigned int length);
void sensor_actuator(String comand);


void setup() {
  Serial.begin(115200);
  Wire.begin();
  GY30.begin();
  htu21d.begin();
  conectaWiFi();
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(instruction);
  
}

void loop() {
  conectaWiFi();
  conectaMQTT();
  MQTT.loop();

}

void conectaWiFi() {

  if (WiFi.status() == WL_CONNECTED) {
     return;
  }
  else{
        
      Serial.print("Conectando-se na rede: ");
      Serial.print(SSID);
      Serial.println("  Aguarde!");
    
      WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI  
      while (WiFi.status() != WL_CONNECTED) {
          delay(100);
          Serial.print(".");
      }
      
      Serial.println();
      Serial.print("Conectado com sucesso, na rede: ");
      Serial.print(SSID);  
      Serial.print("  IP obtido: ");
      Serial.println(WiFi.localIP());
  } 
}

void conectaMQTT() { 
  
    while (!MQTT.connected()) {
        Serial.print("Conectando ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)){ 
          Serial.println("Conectado ao Broker com sucesso!");
          if(MQTT.subscribe(TOP_SUB)) Serial.println("Subscrito no Topico com sucesso");
        }
        else Serial.println("Nao foi possivel se conectar ao broker.");      
    }
}

void instruction(char* topic, byte* payload, unsigned int length){

    String msg;

    //obtem a string do payload recebido
    for(int i = 0; i < length; i++){
       char c = (char)payload[i];
       msg += c;
    }
    Serial.print("Mensagem recebida = ");
    Serial.println(msg);
    sensor_actuator(msg);
    
}

void sensor_actuator(String comand){

  char msg[100];
  float umd = htu21d.readHumidity();
  float temp = htu21d.readTemperature();
  float lux = GY30.readLightLevel();

  if(comand == "0") sprintf(msg,"Temp: %.1f ºC, Umd: %.1f UR, Lum: %.1f lx", temp, umd, lux); //Todos os sensores
  else if(comand == "1") sprintf(msg,"Temp: %.1f ºC",temp);//Apenas Temperatura
  else if(comand == "2") sprintf(msg,"Umd: %.1f UR",umd);//Apenas Umidade
  else if(comand == "3") sprintf(msg,"Lum: %.1f lx",lux);//Apenas Luminosidade

  //Publica a mensagem
  MQTT.publish(TOP_PUB, msg);

}
  

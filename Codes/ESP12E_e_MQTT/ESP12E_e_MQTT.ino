#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define TOP_PUB "GC/SMCAI/Sensor"
#define TOP_SUB "GC/SMCAI/Comand"
#define ID_MQTT "GC_SMCAI_1"

//WiFi
const char* SSID = "Gigacandanga";
const char* PASSWORD = "Aldebaran2@18noc316316";

//Broker (Servidor MQTT)
const char* BROKER_MQTT = "iot.eclipse.org";
int BROKER_PORT = 1883;

//Variáveis e estruturas
WiFiClient esp12e;
PubSubClient MQTT(esp12e);

//Subrotinas
void conectaWiFi();
void conectaMQTT();
void on_message(char* topic, byte* payload, unsigned int length);
void publica();

void setup() {
  Serial.begin(115200);
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(on_message);
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

void on_message(char* topic, byte* payload, unsigned int length){
   
   String msg;
    
   //obtem a string do payload recebido
   for(int i = 0; i < length; i++){
       char c = (char)payload[i];
       msg += c;
    }
    Serial.print("Mensagem recebida = ");
    Serial.println(msg);

    if(msg == "0")publica(0);
    else if(msg == "1")publica(1);
    else if(msg == "2")publica(2);
    else if(msg == "3")publica(3);
}

void publica(int comand){

  char msg[100];
  float umd = 50.0; //htu21d.readHumidity();
  float temp = 25.0; //htu21d.readTemperature();
  float lux = 300.0; //GY30.readLightLevel();

  if(comand == 0) sprintf(msg,"Temp: %.1f ºC, Umd: %.1f UR, Lum: %.1f lx", temp, umd, lux); //Todos os sensores
  else if(comand == 1) sprintf(msg,"Temp: %.1f ºC",temp);//Apenas Temperatura
  else if(comand == 2) sprintf(msg,"Umd: %.1f UR",umd);//Apenas Umidade
  else if(comand == 3) sprintf(msg,"Lum: %.1f lx",lux);//Apenas Luminosidade

  //Publica a mensagem
  MQTT.publish(TOP_PUB, msg);
  
}

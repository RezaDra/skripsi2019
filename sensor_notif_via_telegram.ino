#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

//koneksi Wifi
char ssid[] = "RezaFti";    //masukan SSID wifi     
char password[] = "ezadr1701"; //password Wifi

//alamat bot telegram
#define BOTtoken "983270405:AAEfsC6PnVfx_N9dzxHKfxJx105Q2AmWbfg"  
#define chat_id "744562372"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

//inisialisasi pin
const int sensor = D2;
const int buzzer = D4;

void connectwifi(){
  //connect ke Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  // connected
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup(){
  Serial.begin(115200);
  pinMode(sensor, INPUT_PULLUP);
  pinMode(buzzer,OUTPUT);
  connectwifi();
}

void loop() {
  //seting sensor 
  if(isPeopleDetected())
    turnOnBuzzer();    
  else
    turnOffBuzzer();
}

void turnOnBuzzer(){
  digitalWrite(buzzer,HIGH);
  Serial.println("Gerakan Terdeteksi");
  bot.sendChatAction(chat_id, "Sedang Mengetik");
  Serial.println("Mengidentifikasi Adanya Gerakan");
  bot.sendMessage(chat_id, "Gerakan Terdeteksi", "");   
}

void turnOffBuzzer(){
  digitalWrite(buzzer,LOW);  
}

boolean isPeopleDetected(){
  int sensorValue = digitalRead(sensor);
  if(sensorValue == HIGH)
  {
    return true;
  }
  else
  {
    return false;
  }
}

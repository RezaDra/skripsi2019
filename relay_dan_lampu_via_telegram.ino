#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

//koneksi wifi
char ssid[] = "RezaFti";     // masukan network SSID Wifi
char password[] = "ezadr1701"; // password wifi

// alamat Bot Telegram
#define BOTtoken "944134661:AAFqroEvyONT-ltij3FkTKU5unN8XDD8hT4" 

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);         //client bot telegram

int Bot_mtbs = 1000; //memindai waktu pesan
long Bot_lasttime;   //pemindaian pesan terakhir yang telah di lakukan
bool Start = false;

const int relay = D2;
int ledStatus = 0;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/lampu_on") {
      digitalWrite(relay, LOW);   // kondisi menyalakan lampu
      ledStatus = 1;
      bot.sendMessage(chat_id, "Lampu Menyala", "");
    }

    if (text == "/lampu_off") {
      ledStatus = 0;
      digitalWrite(relay, HIGH);    // kondisi lampu mati
      bot.sendMessage(chat_id, "Lampu Mati", "");
    }

    if (text == "/status") {
      if(ledStatus){
        bot.sendMessage(chat_id, "Lampu Menyala", "");
      } else {
        bot.sendMessage(chat_id, "Lampu Mati", "");
      }
    }

    if (text == "/start") {
      String welcome = "Selamat datang di control lampu via telegram, " + from_name + ".\n";
      welcome += "Berikut contoh perintah untuk kontrol.\n\n";
      welcome += "/lampu_on : untuk menyalakan lampu\n";
      welcome += "/lampu_off : untuk mematikan lampu\n";
      welcome += "/status : untuk memeriksa status lampu\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}


void setup() {
  Serial.begin(115200);

  // koneksi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // upaya terhubung ke jaringan
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(relay, OUTPUT); // inisialisasi pin sebagai output
  delay(10);
  digitalWrite(relay, LOW); // inisialisasi pin off
}

void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    Bot_lasttime = millis();
  }
}

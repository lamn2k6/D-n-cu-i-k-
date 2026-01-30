#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <esp_now.h>

const char* ssid= "TP-Link_2F80";
const char* pass= "49828818";

WiFiClientSecure espSecure;
PubSubClient client(espSecure);

uint8_t SlaverMac[3][6]={
{0x00, 0x70, 0x07, 0x1C, 0xFF, 0x64},
{0x88, 0x57, 0x21, 0xBC, 0x56, 0x58},
{0xF0, 0x24, 0xF9, 0x0E, 0x9D, 0x0C}
};
String data_esp_now = "";


void find_send_slaver(String data){
  int n=data.indexOf(':');
  if (n==-1) return; //Bỏ qua nếu ko có ":"
  //Lấy id trước ":"
  int id=data.substring(0, n).toInt();
  //Lấy lệnh sau ":"
  String command= data.substring(n+1);
  command.trim();

  esp_now_send(SlaverMac[id], (uint8_t*)command.c_str(), command.length());
}

void Pub(){
  if(data_esp_now !=""){
  client.publish("group/4/slaver", data_esp_now.c_str());
  data_esp_now="";
  }
}

void Sub(char* topic, byte* payload, unsigned int length){
  String n;
  for (int i=0; i<length; i++){
    n+=(char)payload[i];
  }
  n.trim();
  Serial.println(n);

  if (String(topic)=="group/4/switch"){
    find_send_slaver(n);
  }
}

void connect_and_last_will(){
  client.connect("ESP32-4", "lamn2k6", "12345678.Aa", "vju/4/lastwill", 1, true, "offline");
}

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  esp_now_init();

  //Receiver đăng kí
  esp_now_register_recv_cb([](
    const esp_now_recv_info_t *info, const uint8_t *incoming, int len){
      data_esp_now="";
      for(int i=0; i<len; i++){
         data_esp_now+=(char)incoming[i];
         }
         Serial.println(data_esp_now);
    }
  );

  //Sender đăng kí
  for (int i=0; i<2; i++){
    esp_now_peer_info_t x={};
    memcpy(x.peer_addr, SlaverMac[i], 6);
    x.channel=0;
    x.encrypt=false;
    esp_now_add_peer(&x);
  }

  //mqtt
  espSecure.setInsecure();
  client.setServer("b22faecbf48544dcaf34b3dcebfa352b.s1.eu.hivemq.cloud", 8883);
  delay(5000);
  client.setCallback(Sub);



}
void loop() {
  if(!client.connected()){
    connect_and_last_will();
    delay(5000);
    client.subscribe("group/4/switch");
  }
  if(WiFi.status() != WL_CONNECTED){
    WiFi.begin(ssid, pass);
    delay(5000);
    connect_and_last_will();
  }
  client.loop();
  Pub();
  delay(10);
}

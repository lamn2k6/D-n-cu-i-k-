#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <PZEM004Tv30.h>

PZEM004Tv30 pzem(Serial2, 16, 17);
float U;
float I;
float P;
float Energy;
float f;
float PF;

uint8_t mac[]= {0x88, 0x57, 0x21, 0xBC, 0x4E, 0xDC};

String data_esp_now;
String state;
String device="3";
String payload;

volatile bool send_check=false;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    send_check = true;
  } else {
    send_check = false;
  }
}

void scan_channel(){
  if (send_check==false){
    for(int i=1; i<=13; i++){
      esp_wifi_set_channel(i, WIFI_SECOND_CHAN_NONE);
      esp_now_send(mac, (uint8_t*)payload.c_str(), payload.length()+1);
      delay(100);
    
    if (send_check==true){
      break;
    }
  }
}
}

void reset(){
  data_esp_now.trim();
  if(data_esp_now=="reset"){
    pzem.resetEnergy();
  }
  data_esp_now="";
}

void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);


  esp_now_init();
  // Dòng code mới (ép kiểu)
  esp_now_register_send_cb((esp_now_send_cb_t)OnDataSent);

  //Receiver
  esp_now_register_recv_cb([](const esp_now_recv_info_t *info, const uint8_t *incoming, int len){
      data_esp_now="";
      for(int i=0; i<len;i++){
        data_esp_now+=char(incoming[i]);
      }
      data_esp_now.trim();
      Serial.println(data_esp_now);
      if (data_esp_now == "On"){
      digitalWrite(2, HIGH);
    } else if(data_esp_now == "Off"){
      digitalWrite(2, LOW);
    }
    }
  );

  //Sender
  esp_now_peer_info_t x={};
  memcpy(x.peer_addr, mac, 6);
  x.channel=0;
  x.encrypt=false;
  esp_now_add_peer(&x);

    
}

void loop() {
  U= pzem.voltage();
  I= pzem.current();
  P= pzem.power();
  Energy  = pzem.energy();
  f= pzem.frequency();
  PF= pzem.pf();
  if(digitalRead(2)==HIGH){
    state="On";
  }else{
    state="Off";
  }

  reset();
 
  // Gói hết lại thành json
  payload="{";
  payload +="\"Device\":\"" + device+ "\",";
  payload +="\"State\":\"" + state+"\",";
  payload +="\"U\":" + String(U)+",";
  payload +="\"I\":" + String(I)+",";
  payload +="\"P\":" + String(P)+",";
  payload +="\"Energy\":" + String(Energy)+",";
  payload +="\"f\":" + String(f)+",";
  payload +="\"PF\":" + String(PF);
  payload +="}";
  payload +="\n";
  Serial.println(payload);

  esp_now_send(mac, (uint8_t*)payload.c_str(), payload.length()+1);
  delay(100);
  scan_channel();
  delay(random(4000, 8000));
  
}

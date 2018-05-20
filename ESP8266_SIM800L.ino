#include "Khaibao.h"
#include "Define.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266HTTPUpdateServer.h>

IPAddress ip10;
IPAddress gateway10;
IPAddress subnet10;
IPAddress DNS(8, 8, 8, 8);

ESP8266WebServer server(4999);
ESP8266HTTPUpdateServer httpUpdater;

void SetVariHC(String vari,String giatri);
void getHC();
void goidt(byte khancap);
void kttkcusd();
void receive_uart();
void init_SIM900A();
int sendAT(char* ATcommand, char* expected_answer, unsigned int timeout);
void power_on();
void send_SMS(String noidungsms="");
void kttk(String nd);
int limit_connect=0;
void setup() {
  //wdt_disable();
#ifdef OUT_CC || INT_SENS
  pinMode(IN3, INPUT);
  pinMode(IN2, INPUT);
  pinMode(IN1, INPUT);
 
  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
  digitalWrite(OUT1,LOW);
  digitalWrite(OUT2,LOW);
  digitalWrite(OUT3,LOW);
#endif
 pinMode(status_led, OUTPUT);
  digitalWrite(status_led,LOW);
  //digitalWrite(5,LOW);
    Serial.begin(115200);
#ifdef USING_SIM
  digitalWrite(status_led,HIGH);
  delay(1000);
  digitalWrite(status_led,LOW);
  delay(1000);
  digitalWrite(status_led,HIGH);
  delay(1000);
  digitalWrite(status_led,LOW);
  delay(1000);
  digitalWrite(status_led,HIGH);
  delay(1000);
  digitalWrite(status_led,LOW);
#endif
  //delay(5000);
  EEPROM.begin(1024);
  delay(10);
  if (!loadWiFiConf()) {
    resetModuleId();
    saveWiFiConf();
  }
  WiFi.setAutoConnect(false);
  WiFi.setAutoReconnect(false);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(WiFiConf.module_id, "88888888");
  scanWiFi();
  connect_wifi();
  statusmang=waitConnected();
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(status_led,LOW);
   WiFi.softAPdisconnect(true);
  } 
  httpUpdater.setup(&server, update_path, update_username, update_password);
  setupWeb();
  setupWiFiConf();  
  server.begin();
  //MDNS.addService("http", "tcp", 4999);
  timebegin=millis();
#ifdef USING_SIM
  getHC();
  power_on();
  init_SIM900A();
  manap=WiFiConf.sta_manap;
  manap.trim();
  guitinnhan=0;
#endif
  timeled = millis();
  timer_gio=timeled;
#ifdef OUT_CC || INT_SENS  
  guisms[0] = (digitalRead(IN1)==0)?0:1 ;
  guisms[1] = (digitalRead(IN2)==0)?0:1 ;
  guisms[2] = (digitalRead(IN3)==0)?0:1 ; 
#endif      
}
void loop() {
#ifdef USINGWIFI
  server.handleClient();
  switch (WiFi.status())
  {
    case WL_CONNECTED:
              if (statusmang==0){digitalWrite(status_led, LOW); 
              statusmang=1;
              cho=0;
              limit_connect=0;
              WiFi.softAPdisconnect(true);}     
              break;
    default:
          if (statusmang!=0){ statusmang=0;timeled = millis();WiFi.softAPdisconnect(false);} 
          if (limit_connect > 3){ESP.reset();}
          if (cho>=50){
            connect_wifi();
            limit_connect++;
            cho=0;
          }
          else
          {
                  if ( (unsigned long) (millis() - timeled) > 500 ){
                          if ( digitalRead(status_led) == LOW )digitalWrite(status_led, HIGH); 
                          else digitalWrite(status_led, LOW );
                          timeled = millis();
                          cho++;
                  }
          } 
          break;
  }
#endif
#ifdef USING_SIM
  receive_uart();
  switch (guitinnhan){
    case 1:
          guitinnhan=0;
          send_SMS(noidung);
          break;
    case 2:
          guitinnhan=0;
          goidt(0);
          break;
    case 3:
          guitinnhan=0;
          kttkcusd();
          break;
    case 5:
          guitinnhan=0;
          manap=manap+manapthe;
          manap=manap+"#";
          kttk(manap);
          break;
    case 6:
          guitinnhan=3;
          break;
    case 7:
          guitinnhan=0;
          send_SMS1(noidung);
          break;
    case 8:
          guitinnhan=0;
          goidt(1);
          break;
    default:
          break;
  }
  if (dtmf_sim != 10){
    SetVariHC("SIM_DTMF",String(dtmf_sim));
    dtmf_sim=10;
  }
 /* switch (dtmf_sim){
    case 0:
          SetVariHC("SIM_DTMF","0");
          dtmf_sim=10;
          break;
    case 1:
          SetVariHC("SIM_DTMF","1");
          dtmf_sim=10;
          break;
    case 2:
          SetVariHC("SIM_DTMF","2");
          dtmf_sim=10;
          break;
    case 3:
          SetVariHC("SIM_DTMF","3");
          dtmf_sim=10;
          break;
    case 4:
          SetVariHC("SIM_DTMF","4");
          dtmf_sim=10;
          break;
    case 5:
          SetVariHC("SIM_DTMF","5");
          dtmf_sim=10;
          break;
    case 6:
          SetVariHC("SIM_DTMF","6");
          dtmf_sim=10;
          break;
    case 7:
          SetVariHC("SIM_DTMF","7");
          dtmf_sim=10;
          break;
    case 8:
          SetVariHC("SIM_DTMF","8");
          dtmf_sim=10;
          break;
    case 9:
          SetVariHC("SIM_DTMF","9");
          dtmf_sim=10;
          break;
  }*/
#endif
#ifdef OUT_CC
switch (out){
  case 1:
    digitalWrite(OUT1,HIGH);
    digitalWrite(OUT2,LOW);
    digitalWrite(OUT3,LOW);
    out=4;
    break;
  case 2:
    digitalWrite(OUT1,LOW);
    digitalWrite(OUT2,HIGH);
    digitalWrite(OUT3,LOW);
    out=4;
    break;
  case 3:
    digitalWrite(OUT1,LOW);
    digitalWrite(OUT2,LOW);
    digitalWrite(OUT3,HIGH);
    out=4;
    break;
  case 4:
     delay(10);
     _int_timeout_relay ++;
     if (_int_timeout_relay >50){
      digitalWrite(OUT1,LOW);
      digitalWrite(OUT2,LOW);
      digitalWrite(OUT3,LOW);
      out=0;
      _int_timeout_relay=0;
     }
      break;
  
}
#endif
#ifdef INT_SENS
  if(digitalRead(IN1)==0){if (gui[0]==0){delay(50);if(digitalRead(IN1)==0){gui[0]=1;goidt(0);}}} //;Serial.println("IN1");digitalWrite(OUT3,HIGH); String tinnhan="Alarm 1 OPEN";send_SMS(tinnhan);}}}
  else if(digitalRead(IN1)==1){if (gui[0]==1){delay(50);if(digitalRead(IN1)==1){gui[0]=0;}}}
  if(digitalRead(IN2)==0){if (gui[1]==0){delay(50);if(digitalRead(IN2)==0){gui[1]=1;goidt(0);}}}//Serial.println("IN2");digitalWrite(OUT3,HIGH);String tinnhan="Alarm 2 OPEN";send_SMS(tinnhan);}}}
  else if(digitalRead(IN2)==1){if (gui[1]==1){delay(50);if(digitalRead(IN2)==1){gui[1]=0;}}}
  if(digitalRead(IN3)==0){if (gui[2]==0){delay(50);if(digitalRead(IN3)==0){gui[2]=1;goidt(0);}}}//Serial.println("IN3");digitalWrite(OUT3,HIGH);String tinnhan="Alarm 3 OPEN";send_SMS(tinnhan);}}}
  else if(digitalRead(IN3)==1){if (gui[2]==1){delay(50);if(digitalRead(IN3)==1){gui[2]=0;}}} 
#endif
  if ( (unsigned long) (millis() - timer_gio) > 10000 ){  
                          timer_gio = millis();
                          thoigian_gio++;
                          #ifdef USING_SIM
                            if (thoigian_gio > 2160){ thoigian_gio=0;guitinnhan=3;}
                            Serial.println("AT");
                            if (da_kttk){ if (sotien<15000 && sotien>1000){da_kttk=false; noidung="Chu y : So TK con "; noidung = noidung + String(sotien);noidung = noidung + "d. De nap soan cu phap NAP:mathe. gui den sdt nay";guitinnhan=1;}};
                          #endif
  }
}

//******************************************************************************************************


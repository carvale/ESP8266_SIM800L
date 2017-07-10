
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266HTTPUpdateServer.h>

#define status_led  0

IPAddress ip10;
IPAddress gateway10;
IPAddress subnet10;
IPAddress DNS(8, 8, 8, 8);

const char* update_path = "/firmware";
const char* update_username = "mhome";
const char* update_password = "fibaro";
const int buffer_size = 200;

ESP8266WebServer server(4999);
ESP8266HTTPUpdateServer httpUpdater;
#define WIFI_CONF_FORMAT {0, 0, 0, 1}
#define WIFI_CONF_START 0
#define NAME_PREF "GSM-"
const uint8_t wifi_conf_format[] = WIFI_CONF_FORMAT;
byte x=0;
int i=0;
byte gui=0;
byte gui1=0;
byte gui2=0;
int cho=0;
unsigned long previous=0;
unsigned long timeout=0;
unsigned long timebegin=0;
unsigned long timeled=0;
//char *ptr1=0;
int answer;
char aux_string[160];
char buffer[buffer_size+1];          // Data from UART
char response[buffer_size+1];
byte xbuff=0;


char uart_tam='\0';
//char c1=0;
char phone_number[13];

String noidung="123456789012345678901234567890123456789012345678901234567890123456789012345678904444444";
String manapthe;
String noidungkiemtratk;
String network_html;
String ipStr="Check Connect";
String SerialHC2="Check Connect";
String manap;

byte status_failmang=0;
//byte _resetketnoi=0;
byte statusmang=0;
byte statusnaptk=0;

char sdt_new[15];

byte guitinnhan=0;
//  byte ip1[4];
//  byte gateway[4];
//  byte subnet[4];
struct WiFiConfStruct {
  uint8_t format[4];
  char sta_ssid[32];
  char sta_pwd[64];
  char sta_ip[15];
  char sta_gateway[15];
  char sta_subnet[15];
  char sta_iphc2[20];
  char sta_passhc[100];
  char sta_SDT1[15];
  char sta_SDT2[15];
  char sta_SDT3[15];
  char sta_SDT4[15];
  char sta_global1[20];
  char sta_global2[20];
  char sta_language[3];
  char sta_manap[10];
  char sta_makttk[10];
  char module_id[32];
} WiFiConf = {
  WIFI_CONF_FORMAT,
  "mhome1",
  "nangniutoam",
  "192.168.99.120",
  "192.168.99.1",
  "255.255.255.0",
  "iphc2",
  "passhc",
  "x",
  "x",
  "x",
  "x",
  "global1",
  "global2",
  "1",
  "*100*",
  "*101#",
  ""
};
void SetVariHC(String vari,String giatri);
void getHC();
void goidt();
void kttkcusd();
void receive_uart();
void init_SIM900A();
//void erase_buffer();
int sendAT(char* ATcommand, char* expected_answer, unsigned int timeout);
//int sendATString(String ATcommand, char* expected_answer, unsigned int timeout) ;
void power_on();
void send_SMS(String noidungsms="");
void kttk(String nd);

void setup() {
  Serial.begin(115200);
  pinMode(12, INPUT);
  pinMode(14, INPUT);
  pinMode(16, INPUT);
  pinMode(13, INPUT);
  //pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(status_led, OUTPUT);
  digitalWrite(status_led,LOW);
  digitalWrite(5,LOW);
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
  //delay(5000);
  EEPROM.begin(1024);
  delay(10);
  if (!loadWiFiConf()) {
    resetModuleId();
    saveWiFiConf();
  }
  scanWiFi();
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(WiFiConf.sta_ssid, WiFiConf.sta_pwd);
  parseBytes1(WiFiConf.sta_ip, '.', 1, 4, 10);
  parseBytes1(WiFiConf.sta_gateway, '.', 2, 4, 10);
  parseBytes1(WiFiConf.sta_subnet, '.', 3, 4, 10);
  WiFi.config(ip10,gateway10,subnet10,DNS);
  statusmang=waitConnected();
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(status_led,HIGH);
   WiFi.softAP("GSM mHome","88888888",1,1);
   delay(1);   
  } else {
    WiFi.mode(WIFI_AP);
    WiFi.softAP("GSM mHome","88888888");
  }
  httpUpdater.setup(&server, update_path, update_username, update_password);
  setupWeb();
  setupWiFiConf();  
  server.begin();
  //MDNS.addService("http", "tcp", 4999);
  timebegin=millis();
   getHC();
  power_on();
  init_SIM900A();
  timeled = millis();
          manap=WiFiConf.sta_manap;
          manap.trim();
          Serial.println(manap);
}
void loop() {
 
  server.handleClient();
    switch (WiFi.status())
  {
    case WL_CONNECTED:
              if (statusmang==0){digitalWrite(status_led, HIGH); 
              statusmang=1;
              cho=0;
              WiFi.softAP("GSM mHome","88888888",1,1);}     
              break;
    default:
          if (statusmang!=0){ statusmang=0;timeled = millis();} 
          if (cho>=50){
            WiFi.begin(WiFiConf.sta_ssid, WiFiConf.sta_pwd);
            parseBytes1(WiFiConf.sta_ip, '.', 1, 4, 10);
            parseBytes1(WiFiConf.sta_gateway, '.', 2, 4, 10);
            parseBytes1(WiFiConf.sta_subnet, '.', 3, 4, 10);
            WiFi.config(ip10,gateway10,subnet10,DNS);
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
 /*  if (WiFi.status() == WL_CONNECTED) {
       if (statusmang==0){digitalWrite(status_led, HIGH); statusmang=1;cho=0;WiFi.softAP("GSM mHome","88888888",1,1);}                            
    }
    else {
      if (statusmang!=0){ statusmang=0;timeled = millis();}
         // server.handleClient();  
          if (cho>=50){
          WiFi.begin(WiFiConf.sta_ssid, WiFiConf.sta_pwd);
         // parseBytes(WiFiConf.sta_ip, '.', ip1, 4, 10);
        //  parseBytes(WiFiConf.sta_gateway, '.', gateway, 4, 10);
        //  parseBytes(WiFiConf.sta_subnet, '.', subnet, 4, 10);
            parseBytes1(WiFiConf.sta_ip, '.', 1, 4, 10);
  parseBytes1(WiFiConf.sta_gateway, '.', 2, 4, 10);
  parseBytes1(WiFiConf.sta_subnet, '.', 3, 4, 10);
  WiFi.config(ip10,gateway10,subnet10,DNS);
        //  WiFi.config(ip1, gateway, subnet);
          cho=0;
          }
          else
          {
                  if ( (unsigned long) (millis() - timeled) > 500 )
                      {
                          if ( digitalRead(4) == LOW )
                          {digitalWrite(status_led, HIGH);} 
                          else {
                                  digitalWrite(status_led, LOW );
                                  }
                                  timeled = millis();
                                  cho++;
                  }
          }      
    }*/
  receive_uart();
  switch (guitinnhan){
    case 1:
          guitinnhan=0;
          send_SMS(noidung);
          break;
    case 2:
          guitinnhan=0;
          goidt();
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
          goidt2();
          break;
    default:
          break;
  }
  /*if (guitinnhan==1){guitinnhan=0;send_SMS(noidung);}
  else if (guitinnhan==2){guitinnhan=0;goidt();}
  else if (guitinnhan==3){guitinnhan=0;kttkcusd();}
  else if (guitinnhan==5){guitinnhan=0;String manap=WiFiConf.sta_manap;manap.trim();manap=manap+manapthe;manap=manap+"#";kttk(manap);}
  else if (guitinnhan==6){guitinnhan=3;}
  else if (guitinnhan==7){guitinnhan=0;send_SMS1(noidung);}
  else if (guitinnhan==8){guitinnhan=0;goidt2();}*/
  if(digitalRead(13)==0){if (gui==0){delay(50);if(digitalRead(16)==0){Serial.println("IN1");gui=1;digitalWrite(5,HIGH); String tinnhan="Alarm 1 OPEN";send_SMS(tinnhan);goidt();}}}
  else if(digitalRead(13)==1){if (gui==1){delay(50);if(digitalRead(16)==1){gui=0;}}}
  if(digitalRead(14)==0){if (gui1==0){delay(50);if(digitalRead(14)==0){Serial.println("IN2");gui1=1;digitalWrite(5,HIGH);String tinnhan="Alarm 2 OPEN";send_SMS(tinnhan);goidt();}}}
  else if(digitalRead(14)==1){if (gui1==1){delay(50);if(digitalRead(14)==1){gui1=0;}}}
  if(digitalRead(12)==0){if (gui2==0){delay(50);if(digitalRead(12)==0){Serial.println("IN3");gui2=1;digitalWrite(5,HIGH);String tinnhan="Alarm 3 OPEN";send_SMS(tinnhan);goidt();}}}
  else if(digitalRead(12)==1){if (gui2==1){delay(50);if(digitalRead(12)==1){gui2=0;}}}    //String(WiFiConf.sta_noidung3)+
}

//******************************************************************************************************


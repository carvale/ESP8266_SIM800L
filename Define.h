/*
#define status_led  0  // vercu la 4 - 0
#define IN1  13  // Ver cu  la 16 - 13
#define IN2  2   // DUng là 14
#define IN3  12
#define OUT1  15  // 
#define OUT2  14   // Chueyern IN qua OUT cho CC
#define OUT3  4*/
////////////////////////////////////////////
// Chân 2 Không OUT Duoc  /////////////////
///////////////////////////////////////////
#define USINGWIFI true   //Bỏ là tắt chức năng wifi
#define USING_SIM true   // Bỏ là tắt dùng SIM
#define PIN_CONFIG  4
////////////////////////////////
#ifdef USING_SIM
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINT(x)
  const int buffer_size = 200;
  int answer;
  char aux_string[160];
  char buffer[buffer_size+1];          // Data from UART
  char response[buffer_size+1];
  byte xbuff=0;
  char uart_tam='\0';
  char phone_number[13];
  char sdt_new[15];

  String noidung="123456789012345678901234567890123456789012345678901234567890123456789012345678904444444";
  String manapthe;
  String noidungkiemtratk;
  String network_html;
  String ipStr="Check Connect";
  String SerialHC2="Check Connect";
  String manap;
#else
 #define DEBUG_PRINTLN(x)  Serial.println (x)
 #define DEBUG_PRINT(x)  Serial.println (x)
#endif
//////////////////////////////////////
//#define OUT_CC true  // Bỏ là khong dung cua cuon
//#define IN true       // Bỏ là không dùng IN
//#define INT_SENS true // Bỏ là không dùng IN
//#define Ver_old true  // Bỏ là dung chan Ver Mới
#ifdef Ver_old
  #define status_led  4
#else
  #define status_led  0
#endif
#ifdef OUT_CC
  byte out=0;
  int _int_timeout_relay=0;
  #ifdef Ver_old
    #define OUT1  15  // 
    #define OUT2  14   // Chueyern IN qua OUT cho CC
    #define OUT3  4
    #define IN1  13  // Ver cu  la 16 - 13
    #define IN2  2   // DUng là 14
    #define IN3  12
  #else
    #define OUT1  15  // 
    #define OUT2  14   // Chueyern IN qua OUT cho CC
    #define OUT3  4
    #define IN1  13  // Ver cu  la 16 - 13
    #define IN2  2   // DUng là 14
    #define IN3  12
  #endif
#elif INT_SENS
  #ifdef Ver_old
    #define IN1  16  // Ver cu  la 16 - 13
    #define IN2  14   // DUng là 14
    #define IN3  12
    #define OUT1  15  // 
    #define OUT2  2   // Chueyern IN qua OUT cho CC
    #define OUT3  4
  #else
    #define IN1  13  // Ver cu  la 16 - 13
    #define IN2  14   // DUng là 14
    #define IN3  12
    #define OUT1  15  // 
    #define OUT2  2   // Chueyern IN qua OUT cho CC
    #define OUT3  4
  #endif
#endif
/////////////////////////////////////////////
//  WIFI FORMAT ////////////////////////////
///////////////////////////////////////////
#define WIFI_CONF_FORMAT {0, 0, 0, 1}
#define WIFI_CONF_START 0
#define NAME_PREF "GSM-"
const uint8_t wifi_conf_format[] = WIFI_CONF_FORMAT;
struct WiFiConfStruct {
  uint8_t format[4];
  char sta_ssid[32];
  char sta_pwd[64];
  char sta_ip[15];
  char sta_gateway[15];
  char sta_subnet[15];
  char sta_iphc2[20];
  char sta_userhc[64];
  char sta_passhc[64];
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
  "192.168.99.4",
  "kythuat@kimsontien.com",
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
///////////////////////////////////////////
// Khai báo SMS Cho IN/OUT //////////////////
byte guisms[3]={0,0,0};

////////////////////////////////////////
// User pass Update ///////////////////
//////////////////////////////////////
const char* update_path = "/firmware";
const char* update_username = "mhome";
const char* update_password = "fibaro";
//////////////////////////////////////////
// Bien Luu Time/////////////////////
////////////////////////////////////////
unsigned long previous=0;
unsigned long timeout=0;
unsigned long timebegin=0;
unsigned long timeled=0;
unsigned long timer_gio=0;
int thoigian_gio=0;

//////////////////////
// Ket noi

byte x=0;
int i=0;
int cho=0;
byte statusmang=0;
byte statusnaptk=0;
int config_status=0;
long sotien;
boolean da_kttk=false;

byte guitinnhan=0;

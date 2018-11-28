///////////////////////////////////////////////////////////////////////
///Ham Chuyen doi user:password qua Base64 cho giao tiep qua mang///////
////////////////////////////////////////////////////////////////////////
const char PROGMEM b64_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

/* 'Private' declarations */
void a3_to_a4(unsigned char * a4, unsigned char * a3);
void a4_to_a3(unsigned char * a3, unsigned char * a4);
unsigned char b64_lookup(char c);

int base64_encode(char *output, char *input, int inputLen) {
  int j = 0;
  i = 0;
  int encLen = 0;
  unsigned char a3[3];
  unsigned char a4[4];

  while(inputLen--) {
    a3[i++] = *(input++);
    if(i == 3) {
      a3_to_a4(a4, a3);

      for(i = 0; i < 4; i++) {
        output[encLen++] = pgm_read_byte(&b64_alphabet[a4[i]]);
      }

      i = 0;
    }
  }

  if(i) {
    for(j = i; j < 3; j++) {
      a3[j] = '\0';
    }

    a3_to_a4(a4, a3);

    for(j = 0; j < i + 1; j++) {
      output[encLen++] = pgm_read_byte(&b64_alphabet[a4[j]]);
    }

    while((i++ < 3)) {
      output[encLen++] = '=';
    }
  }
  output[encLen] = '\0';
  return encLen;
}

int base64_decode(char * output, char * input, int inputLen) {
  int j = 0;
  i = 0;
  int decLen = 0;
  unsigned char a3[3];
  unsigned char a4[4];


  while (inputLen--) {
    if(*input == '=') {
      break;
    }

    a4[i++] = *(input++);
    if (i == 4) {
      for (i = 0; i <4; i++) {
        a4[i] = b64_lookup(a4[i]);
      }

      a4_to_a3(a3,a4);

      for (i = 0; i < 3; i++) {
        output[decLen++] = a3[i];
      }
      i = 0;
    }
  }

  if (i) {
    for (j = i; j < 4; j++) {
      a4[j] = '\0';
    }

    for (j = 0; j <4; j++) {
      a4[j] = b64_lookup(a4[j]);
    }

    a4_to_a3(a3,a4);

    for (j = 0; j < i - 1; j++) {
      output[decLen++] = a3[j];
    }
  }
  output[decLen] = '\0';
  return decLen;
}

int base64_enc_len(int plainLen) {
  int n = plainLen;
  return (n + 2 - ((n + 2) % 3)) / 3 * 4;
}

int base64_dec_len(char * input, int inputLen) {
  i = 0;
  int numEq = 0;
  for(i = inputLen - 1; input[i] == '='; i--) {
    numEq++;
  }
  return ((6 * inputLen) / 8) - numEq;
}

void a3_to_a4(unsigned char * a4, unsigned char * a3) {
  a4[0] = (a3[0] & 0xfc) >> 2;
  a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
  a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
  a4[3] = (a3[2] & 0x3f);
}

 void a4_to_a3(unsigned char * a3, unsigned char * a4) {
  a3[0] = (a4[0] << 2) + ((a4[1] & 0x30) >> 4);
  a3[1] = ((a4[1] & 0xf) << 4) + ((a4[2] & 0x3c) >> 2);
  a3[2] = ((a4[2] & 0x3) << 6) + a4[3];
}

unsigned char b64_lookup(char c) {
  if(c >='A' && c <='Z') return c - 'A';
  if(c >='a' && c <='z') return c - 71;
  if(c >='0' && c <='9') return c + 4;
  if(c == '+') return 62;
  if(c == '/') return 63;
  return -1;
}

/////////////////////////////////////////////////////////////////////////
void printWiFiConf(void) {
  Serial.println(WiFiConf.sta_ssid);
  Serial.println(WiFiConf.sta_ip);
  Serial.println(WiFiConf.module_id);
  //Serial.println(WiFiConf.sta_ssid);
 // Serial.println(WiFiConf.sta_ssid);
}

bool loadWiFiConf() {
  if (EEPROM.read(WIFI_CONF_START + 0) == wifi_conf_format[0] &&
      EEPROM.read(WIFI_CONF_START + 1) == wifi_conf_format[1] &&
      EEPROM.read(WIFI_CONF_START + 2) == wifi_conf_format[2] &&
      EEPROM.read(WIFI_CONF_START + 3) == wifi_conf_format[3])
  {
    for (unsigned int t = 0; t < sizeof(WiFiConf); t++) {
      *((char*)&WiFiConf + t) = EEPROM.read(WIFI_CONF_START + t);
    }
    printWiFiConf();
    return true;
  } else {
    return false;
  }
}
////////////////////////////////////
//Lưu thông số vào eeprom /////////
//////////////////////////////////
void saveWiFiConf(void) {
  for (unsigned int t = 0; t < sizeof(WiFiConf); t++) {
    EEPROM.write(WIFI_CONF_START + t, *((char*)&WiFiConf + t));
  }
  EEPROM.commit();
  printWiFiConf();
}
void setDefaultModuleId(char* dst) {
  uint8_t macAddr[WL_MAC_ADDR_LENGTH];
  WiFi.macAddress(macAddr);
  sprintf(dst, "%s%02x%02x", NAME_PREF, macAddr[WL_MAC_ADDR_LENGTH - 2], macAddr[WL_MAC_ADDR_LENGTH - 1]);
}
void resetModuleId(void) {
  uint8_t macAddr[WL_MAC_ADDR_LENGTH];
  WiFi.macAddress(macAddr);
  setDefaultModuleId(WiFiConf.module_id);
}
////////////////////////////////////
//do wifi /////////
//////////////////////////////////
void scanWiFi(void) {
  int founds = WiFi.scanNetworks();
  network_html = F("<ol>");
  for (i = 0; i < founds; ++i)
  {
    network_html += F("<li>");
    network_html += WiFi.SSID(i);
    network_html += F(" (");
    network_html += WiFi.RSSI(i);
    network_html += F(")");
    network_html += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    network_html += F("</li>");
  }
  network_html += F("</ol>");
}
////////////////////////////////////
//chờ kết nối mạng /////////
//////////////////////////////////
int waitConnected(void) {
  int wait = 0;
  while ( wait < 20 ) {
    if (WiFi.status() == WL_CONNECTED) {
      return (1);
    }
    //yield();
    delay(700);
    wait++;
  }
  return (0);
}

void printIP(void) {
}

void setupWiFiConf(void) {
  server.on("/wifi_conf", []() {
     String content = FPSTR(header); content += FPSTR(begin_title);
     String   content1 = F("<p>Wifi Connected: ");
    content1 += WiFiConf.sta_ssid;
    content1 += F("</br>IP Address: ");
    content1 += ipStr;
    content1 += F(" ( ");
    content1 += WiFiConf.module_id;
    content1 += F(".local");
    content1 += F(" )</p>");
    content1 += F("<p>");
     content1 += F("</p><form method='get' action='set_wifi_conf'>");
    content1 += F("<li><label for='ssid' class=\"req\">SSID : </label><input name='ssid' class=\"txt\" id='ssid' maxlength=32 value=") ;
    content1 += String(WiFiConf.sta_ssid) ;
    content1 +=  F(" ><br /><br />");

    content1 += F("<li><label for='pwd' class=\"req\">PASS : </label> <input type='password' class=\"txt\" name='pwd' id='pwd' value=");
    content1 += String(WiFiConf.sta_pwd);
    content1 += F("><br /><br />");

    content1 += F("<li><label for='ip' class=\"req\">IP : </label> <input name='ip' class=\"txt\" id='ip' value=");
    content1 += String(WiFiConf.sta_ip) ;
    content1 +=  F(" ><br /><br />");
    
    content1 += F("<li><label for='gateway' class=\"req\">GATEWAY : </label> <input  name='gateway' class=\"txt\" id='gateway' value=") ;
    content1 += String(WiFiConf.sta_gateway) ;
    content1 += F("><br /><br />");

    content1 += F("<li><label for='subnet' class=\"req\">SUBNET : </label> <input  name='subnet' class=\"txt\" id='subnet' value=");
    content1 +=  String(WiFiConf.sta_subnet);
    content1 += F("><br /><br />");
    content += "mHome - Wifi Setup";
    content += F("</title></head><body>");

    content += F("<h1>Wifi Setup</h1>");
    content += content1;
   content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Settings?\");'></form>");
    
    content += F(" </p>");
    content += network_html;
    content += F("</body></html>");
    server.send(200, F("text/html"), content);
    
  });
server.on("/sdt_conf", []() {
     String content = FPSTR(header); content += FPSTR(begin_title);
     String   content1 = F("<p>Wifi Connected: ");
    content1 += WiFiConf.sta_ssid;
    //content1 += F("</br>IP Address: ");
    //content1 += ipStr;
    //content1 += F(" ( ");
    //content1 += WiFiConf.module_id;
    //content1 += F(".local");
    //content1 += F(" )</p>");
    content1 += F("<p>");
    content1 += F("</p><form method='get' action='set_sdt_conf'>");
    content1 += F("<li><label for='SDT1' class=\"req\">Phone Number 1: </label> <input  name='SDT1' class=\"txt\" id='SDT1' value=");
    content1 +=String(WiFiConf.sta_SDT1);
    content1 +=F("><br /><br />");
    content1 += F("<li><label for='SDT2' class=\"req\">Phone Number 2: </label> <input name='SDT2' class=\"txt\" id='SDT2'value=");
    content1 +=String(WiFiConf.sta_SDT2);
    content1 +=F("><br /><br />");
    content1 +=F("<li><label for='SDT3' class=\"req\">Phone Number 3: </label> <input  name='SDT3' class=\"txt\" id='SDT3' value=");
    content1 +=String(WiFiConf.sta_SDT3);
    content1 +=F("><br /><br />");
    content1 += F("<li><label for='SDT4' class=\"req\">Phone Number 4: </label> <input  name='SDT4' class=\"txt\" id='SDT4' value=");
    content1 +=String(WiFiConf.sta_SDT4);
    content1 +=F("><br /><br />");
    content += "mHome - Phone Number";
    content += F("</title></head><body>");

    content += F("<h1>Phone Numbers Setup</h1>");
    content += content1;
    content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Settings?\");'></form>");
    
    content += F("<li>The phone numbers for call and sms function");
    content += F("<li>Pls, Check 'x' for phone number not using");
    
    
    content += F(" </p>");
    //content += network_html;
    content += F("</body></html>");
    server.send(200, F("text/html"), content);
   
  });
  server.on("/set_sdt_conf", []() {
    String new_SDT1 = server.arg("SDT1");
    String new_SDT2 = server.arg("SDT2");
    String new_SDT3 = server.arg("SDT3");
    String new_SDT4 = server.arg("SDT4");
    if (new_SDT1==""){new_SDT1="x";}
    if (new_SDT2==""){new_SDT2="x";}
    if (new_SDT3==""){new_SDT3="x";}
    if (new_SDT4==""){new_SDT4="x";}
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += WiFiConf.module_id;
    content += F(".local - set WiFi");
    content += F("</title></head><body>");
    content += F("<h1>Set WiFi of ESP8266</h1>");
      new_SDT1.toCharArray(WiFiConf.sta_SDT1, sizeof(WiFiConf.sta_SDT1));
      new_SDT2.toCharArray(WiFiConf.sta_SDT2, sizeof(WiFiConf.sta_SDT2));
      new_SDT3.toCharArray(WiFiConf.sta_SDT3, sizeof(WiFiConf.sta_SDT3));
      new_SDT4.toCharArray(WiFiConf.sta_SDT4, sizeof(WiFiConf.sta_SDT4));
      saveWiFiConf();
      content += F("<p>saved '");
      content += WiFiConf.sta_ssid;
      content += F("'... Thiết bị đang khởi động lại</p>");
      content += F("<body></html>");

    server.send(200, "text/html", content);
    delay(500);
    ESP.reset();
  });
  server.on("/tinnhan", []() {
    IPAddress ip = WiFi.localIP();
    //String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
   String content = FPSTR(header); content += FPSTR(begin_title);
   content += F("mHome - SMS");
    content += F("</title></head><body>");
    content += F("<h1>SMS Function</h1>");
    content += F("<p>");
    content += F("</p><form method='get' action='set_noidung'><li><label for='text' class=\"req\" >Content of the message: </label><input name='text' class=\"txt\" id='text' maxlength=32  ><br /><br />");
    content += F("<li><input type='submit' id=\"submitbtn\" value='Test' onclick='return confirm(\"Bạn muốn kiểm tra SMS ?\");'></form>");
    content += F("</body></html>");
    server.send(200, "text/html", content);
  });
  server.on("/cuocgoi", []() {
    IPAddress ip = WiFi.localIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += F("mHome - CALL");
    content += F("</title></head><body>");
    content += F("<h1>Call Function</h1>");
    content += F("<p>");
    content += F("</p><form method='get' action='set_call'>");
    content += F("<li><input type='submit'  id=\"submitbtn\" value='Test' onclick='return confirm(\"Bạn muốn kiểm tra CALL ?\");'></form>");
    content += F("</body></html>");
    server.send(200, "text/html", content);
  });
  ////////////////////////////
  // OUT COntrol ////////////
#ifdef OUT_CC
  server.on("/OUT1", []() {
    out=1;
    server.send(200, "text/html", "ok");
  });
    server.on("/OUT2", []() {
      out=2;
    server.send(200, "text/html", "ok");
  });
    server.on("/OUT3", []() {
      out=3;
    server.send(200, "text/html", "ok");
  });
#endif
///////////////////////
server.on("/set_ktk", []() {
    noidung = server.arg("text");
    guitinnhan=3;
   String content = "" ; //FPSTR(header); content += FPSTR(begin_title);
   //content += WiFiConf.module_id;
    //content += F(".local - set WiFi");
  //  content += F("</title></head><body>");
    content += F("ok");
    server.send(200, "text/html", content);
  });
server.on("/set_noidung", []() {
    noidung = server.arg("text");
    guitinnhan=1;
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += WiFiConf.module_id;
    content += F(".local - set WiFi");
    content += F("</title></head><body>");
    content += F("<h1>ok</h1>");
    server.send(200, "text/html", content);
  });
  server.on("/set_noidung1", []() {
    noidung = server.arg("text");
    String new_phone = server.arg("newphone");
    new_phone.toCharArray(sdt_new, sizeof(sdt_new));
    guitinnhan=7;
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += WiFiConf.module_id;
    content += F(".local - set WiFi");
    content += F("</title></head><body>");
    content += F("<h1>ok</h1>");
    server.send(200, "text/html", content);
  });
  server.on("/set_rsSim", []() {  
    guitinnhan=4;
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += WiFiConf.module_id;
    content += F(".local - set WiFi");
    content += F("</title></head><body>");
    content += F("<h1>Set WiFi of ESP8266</h1>");
    server.send(200, "text/html", content);   
  });
  server.on("/set_call", []() {
   // noidung = server.arg("text");
    guitinnhan=2;
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += WiFiConf.module_id;
    content += F(".local - set WiFi");
    content += F("</title></head><body>");
    content += F("<h1>Set WiFi of ESP8266</h1>");
    server.send(200, "text/html", content);
  });
  server.on("/set_call2", []() {
    String new_phone = server.arg("newphone");
    new_phone.toCharArray(sdt_new, sizeof(sdt_new));
    guitinnhan=8;
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += WiFiConf.module_id;
    content += F(".local - set WiFi");
    content += F("</title></head><body>");
    content += F("<h1>Set WiFi of ESP8266</h1>");
    server.send(200, "text/html", content);
  });
 server.on("/set_naptk", []() {
    manapthe = server.arg("Recharge");
    manapthe.trim();
    guitinnhan=5;
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += WiFiConf.module_id;
    content += F(".local - set WiFi");
    content += F("</title></head><body>");
    content += F("<h1>Set WiFi of ESP8266</h1>");
    server.send(200, "text/html", content);
  });
//////////////
  server.on("/set_wifi_conf", []() {
    String new_ssid = server.arg("ssid");
    String new_pwd = server.arg("pwd");
    String new_ip = server.arg("ip");
    String new_gateway = server.arg("gateway");
    String new_subnet = server.arg("subnet");
    
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += WiFiConf.module_id;
    content += F(".local - set WiFi");
    content += F("</title></head><body>");
    content += F("<h1>Set WiFi of ESP8266</h1>");
    if (new_ssid.length() > 0) {
      new_ssid.toCharArray(WiFiConf.sta_ssid, sizeof(WiFiConf.sta_ssid));
      new_pwd.toCharArray(WiFiConf.sta_pwd, sizeof(WiFiConf.sta_pwd));
      new_ip.toCharArray(WiFiConf.sta_ip, sizeof(WiFiConf.sta_ip));
      new_gateway.toCharArray(WiFiConf.sta_gateway, sizeof(WiFiConf.sta_gateway));
      new_subnet.toCharArray(WiFiConf.sta_subnet, sizeof(WiFiConf.sta_subnet));

      saveWiFiConf();
      content += F("<p>saved '");
      content += WiFiConf.sta_ssid;
      content += F("'... Thiết bị đang khởi động lại</p>");
      content += F("<body></html>");
    } else {
      content += F("<p>Empty SSID is not acceptable. </p>");
      content += F("<body></html>");
      Serial.println(F("Rejected empty SSID."));
    }
    server.send(200, "text/html", content);
    ESP.reset();
  });

  server.on("/module_id", []() {
    IPAddress ip = WiFi.localIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    char defaultId[sizeof(WiFiConf.module_id)];
    setDefaultModuleId(defaultId);
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += WiFiConf.module_id;
    content +=F(".local - Module ID");
    content += F("</title></head><body>");
    content += F("<h1>Module ID cho Wifi</h1>");
    content += F("<p>Module ID: ");
    content += WiFiConf.module_id;
    content += F("</br>IP: ");
    content += ipStr;
    content += F("</p>");
    content += F("<p>");
    content += F("<form method='get' action='set_module_id'><label for='module_id' class=\"req\">New Module ID: </label><input name='module_id' class=\"txt\" id='module_id' maxlength=32 value='");
    content += WiFiConf.module_id;
    content += F("'><input type='submit'  id=\"submitbtn\" onclick='return confirm(\"Are you sure you want to change the Module ID?\");'></form>");
    content += F(" Empty will reset to default ID '");
    content += defaultId;
    content += F("'</p>");
    content += F("</body></html>");
    server.send(200, "text/html", content);    
  });

  server.on("/set_module_id", []() {
    String new_id = server.arg("module_id");
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += F("<title>");
    content += WiFiConf.module_id;
    content += F(".local - set WiFi");
    content += F("</title>");
    content += F("</head><body>");
    if (new_id.length() > 0) {
      new_id.toCharArray(WiFiConf.module_id, sizeof(WiFiConf.module_id));
    } else {
      resetModuleId();
    }
    saveWiFiConf();
    content += F("<h1>Set WiFi of ESP8266</h1>");
    content += F("<p>Set Module ID to '");
    content += WiFiConf.module_id;
    content += F("' ... Reseting. </p>");
    content += F("</body></html>");
    server.send(200, "text/html", content);
    ESP.reset();
  });
  server.on("/Reboot", HTTP_GET, []() {
    IPAddress ip = WiFi.localIP();
    String content = FPSTR(header); content += FPSTR(begin_title);
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    content += F("mHome - Reset");
    content += F("</title></head><body>");
    content += F("<h1>Thông Tin :</h1>");
    content += F("<p>Cty : TNHH Kim Sơn Tiến ");
    content += F("</br>Địa chỉ : 65/39 Đường 339 Phường Tăng Nhơn Phú B,Q9,TP.HCM");
    content += F("</br>Phát Triển : Phạm An Nhàn");
    content += F("</p>");
    content += F("<p>");
    content += F("<form method='get' action='set_Reset'>");
    content += F("<input type='submit'  id=\"submitbtn\" value='Reset' onclick='return confirm(\"Bạn Có Muốn Khởi Động Lại Module ?\");'></form>");
    content += F("</p>");
    content += F("</body></html>");
    server.send(200, "text/html", content);
  });

server.on("/mang_didong", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += F("mHome - USSD");
    content += F("</title></head><body>");

     
    content += F("<h1>Cellular network settings</h1>");
    content += F("<p>Wifi conected : ");
    content += WiFiConf.sta_ssid;
   // content += F("</br>IP address: ");  
    content += F("</p><form method='get' action='set_mang_didong'>");

    content += F("<li><label for='manap' class=\"req\">USSD for Recharge code :&nbsp;</label><input name='manap' class=\"txt\" id='manap' maxlength=32 value=");
    content += String(WiFiConf.sta_manap);
    content += F(" ><br /><br />");

    content +=F("<li><label for='makttk' class=\"req\">USSD for Balance Check : </label> <input  name='makttk' class=\"txt\" id='pwdhc2' value=");
    content += String(WiFiConf.sta_makttk) ;
    content += F("><br /><br />");
    
    content += F("<input type='submit'  id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Setting ?\");'></form>");

    content += F("<li>Ex: Recharge code:55555555555 . USSD: *100* ->*100*55555555555#");
    content += F("<li>USSD for Balance Check : *101#,*102#...");
    
     content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });
  server.on("/set_mang_didong", []() {
    String new_naptk = server.arg("manap");
    String new_makttk = server.arg("makttk");
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += F("mHome - Wifi Setup");
    content += F("</title></head><body>");
    content += F("<h1>Lưu Wifi</h1>");
      new_naptk.toCharArray(WiFiConf.sta_manap, sizeof(WiFiConf.sta_manap));
      new_makttk.toCharArray(WiFiConf.sta_makttk, sizeof(WiFiConf.sta_makttk));
      saveWiFiConf();
      content += F("<p>Save '");
      content += WiFiConf.sta_ssid;
      content += F("' ...  device Rebooting !</p>");
    content += F("<body></html>");
    server.send(200, F("text/html"), content);
  });

  
  server.on("/hc2_conf", []() {    
    /*  int vitricat=0;
      char tamchar[128];
     sprintf(tamchar, "%s:%s|", WiFiConf.sta_userhc, WiFiConf.sta_passhc);
      for (byte tam=0;tam<sizeof(tamchar);tam++){
            if (tamchar[tam]=='|'){
                  vitricat=tam;
                  break;
            }
      }
      int encodedLen = base64_enc_len(vitricat-1);
      char encoded[encodedLen];
      base64_encode(encoded, tamchar, vitricat);*/
    String content = FPSTR(header); content += FPSTR(begin_title);
    String    content1 = ipStr;
    content1 += F(" ( ");
    content1 += WiFiConf.module_id;
    content1 += F(".local");
    content1 += F(" )</p>");
    content1 += F("<p>");
    content1 += F("</p><form method='get' action='set_hc2_conf'>");
    
    content1 += F("<li><label for='iphc2' class=\"req\">IP HC2:</label><input name='iphc2' class=\"txt\" id='iphc2' maxlength=32 value=");
    content1 += String(WiFiConf.sta_iphc2);
    content1 += F(" ><br /><br />");
    content1 +=F("<li><label for='userhc2' class=\"req\">User HC2: </label> <input name='userhc2' class=\"txt\"  id='userhc2' value=");
    content1 += String(WiFiConf.sta_userhc) ;
    content1 += F("><br /><br />");
    content1 +=F("<li><label for='pwdhc2' class=\"req\">PASS HC2: </label> <input type='password' class=\"txt\" name='pwdhc2' id='pwdhc2' value=");
    content1 += String(WiFiConf.sta_passhc) ;
    content1 += F("><br /><br />");

    content1 += F("<li><label for='global1' class=\"req\">Global 1:</label> <input name='global1' class=\"txt\" id='global1'value=");
    content1 += String(WiFiConf.sta_global1);
    content1 += F(" ><br /><br />");

    content1 += F("<li><label for='global2' class=\"req\">Global 2:</label> <input  name='global2' class=\"txt\" id='global2' value=");
    content1 +=  String(WiFiConf.sta_global2);
    content1 += F("><br /><br />");
 
    //content1 += F("><br /><br />");  
    content += F("mHome - Wifi Setup");
    content += F("</title></head><body>");

    content += F("<h1>HC2 Setting</h1>");
    content += F("<p>Wifi conecting : ");
    content += WiFiConf.sta_ssid;
    content += F("</br>IP address: ");  
    content += content1;
    content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Setting ?\");'></form>");
    content += F("</p><form method='get' action='getHC'>");
    content += F("<input type='submit' value='Check'></form>");
    content += F(" </p>");
    content += F("<li>Information HC2");
        content += F("<li>SIM_TK - Tai khoang SIM");
        content += F("<li>Global1 - Trang Thai ON,OFF,OK");
        content += F("<li>SIM_DTMF - Status DTMF");
        content += F("<li>SIM_CALL - Phone number quitclaim calling");
    //content += encoded;
     content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });
  server.on("/set_hc2_conf", []() {
    String new_IPHC = server.arg("iphc2");
    String new_userhc = server.arg("userhc2");
    String new_pwdhc = server.arg("pwdhc2");
    String new_global1 = server.arg("global1");
    String new_global2 = server.arg("global2");
    //String new_global3 = server.arg("global3");
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += F("mHome - Wifi Setup");
    content += F("</title></head><body>");
    content += F("<h1>Lưu Wifi</h1>");
    if (new_IPHC.length() > 0) {
      
      new_IPHC.toCharArray(WiFiConf.sta_iphc2, sizeof(WiFiConf.sta_iphc2));
      new_userhc.toCharArray(WiFiConf.sta_userhc, sizeof(WiFiConf.sta_userhc));
      new_pwdhc.toCharArray(WiFiConf.sta_passhc, sizeof(WiFiConf.sta_passhc));
      new_global1.toCharArray(WiFiConf.sta_global1, sizeof(WiFiConf.sta_global1));
      new_global2.toCharArray(WiFiConf.sta_global2, sizeof(WiFiConf.sta_global2));
      saveWiFiConf();
      content += F("<p>Save '");
      content += WiFiConf.sta_ssid;
      content += F("' ...  device Rebooting !</p>");
      
    } else {
      content += F("<p>Rejected empty SSID.</p>");
     //Serial.println("Rejected empty SSID.");
    }
    content += F("<body></html>");
    server.send(200, F("text/html"), content);
  });
  server.on("/getHC", []() {
    
    String content =  F("<!DOCTYPE HTML>\r\n<html><head><style>body {background-color:lightgrey}h1 {color:green}p {color:black}</style><link rel=\"shortcut icon\" href=\"data:image/x-icon;base64,AAABAAEAEBAAAAEACABoBQAAFgAAACgAAAAQAAAAIAAAAAEACAAAAAAAAAEAAAAAAAAAAAAAAAEAAAAAAADLy8sAHYsbACCNGADK28UADIEHAHOubwAShQoA//7/AObz6ADn8+gA6fPoAOzz6ADGxsYAXa5eAL7gugAghxAAtbyyABFmCAD09vEAN5ctADuPMAA4ly0AxuG9AKvXqQA2lzYAJ4cZADuXNgAikCIAKIwoAHS1cwDs++YAAHgAAKrKpwAAewAAj8WQACCHEQDa6tgAtbyzAMTcwQA4jzEAOpEuAFCkSwAYZgkAa7JiAIK9ggBAmDoAQJY9AA6FDADQ5NAApMalAC2PKQCozagA9PXwAPT48AAvbScAF2YKAGywZgBtsGYA4+3iAMviywA/mDsARpgyAM/jzgBkkGEA0eXLAC6KJAA2dDMAotCgAEWbRAA5iiQAv9+6AKmr3AAQZQgA8vXxAPn48QA+mDMAsdayAJ7DngChxKEANnQ0ACiRKABlkGIAq7iqAAB3AAAAegAAjsSQAHm5eQDKycoAw9vBAGGxYgB/mqMAhJesAPr6+AC21qoA/v/+AJqamgBygMIASpxAAMjIyADv8PYAYLBgANrr1gCUx5EAZ61gAGitYADQy9EAKGpOACOPJAD7+/wAJI8kAPv9+QDo8OUA/v//AI24iQD///8AWaxVAC+MJABerFUAXaleAF6pXgB5tHUA2uvXAPX16wDPys8AxOLGANHL0gAnhB8Agr1+AJqh1AAOfAgAzOHMAIe+gQCLvoEA6O7pAHK0bQCrt6oAMI4iAC6QKAAtlCsAS5s/AF+rXAB4t3YAwdrBAMrIygDv+e8A3O3eAM/K0ADf4uoAdJdxAHWXcQAniBoAbqtuAP3+9QAMhgwAoc6fAHO0bgAvjiMAFYYMAI28jgBGn0MAyMfIAGOrXQB4uHoAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADAyPe4c/NhEqT5QlaaAAX3JyXiITc5oWDmYpS11ycgByXIw8ZXJycnJycnxFg3JiclZESXJycnJycnJyOkEXfSScCXIvG1BtbW1rnYVAPRB4m3JyGgRxMU1OnoFycimVLlhycllTjXJycpAccnJmQpYLcnJkHx8zcnIeAnJyRjcPenJyDVNUBnJjgGBHchZIFDAHIAEoHWxyk1tqWnJDNjmDcm+KdCxycm4FK5hydVE7MhJyl34hGAeff3JyeRVScndoB3JyjpkfTHJyNC1VknIKYWcScnKRA3JyCIuhcFdycgl2iYQ+SjUmhoiiXHIMcnJycoI4JyMZLngkcnJyDAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=\"/><meta charset='UTF-8'><title>");
    content += F("Wait");
    getHC();
    server.send(200, F("text/html"), content);
  });
  server.on("/set_language", []() {
    String new_language = server.arg("language");
    
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += F("mHome - Language");
    content += F("</title></head><body>");
    content += F("<h1>Save</h1>");
    content += F("<p>OK '");
    if (new_language =="Vietnamese") {String lan="1";lan.toCharArray(WiFiConf.sta_language, sizeof(WiFiConf.sta_language));content += "VN";}
    else
      {String lan="0";lan.toCharArray(WiFiConf.sta_language, sizeof(WiFiConf.sta_language));content += "EN";}
      
      saveWiFiConf();
      content += F("' ... Device Reboot !</p>");
      content += F("<body></html>");
    server.send(200, F("text/html"), content);
    ESP.reset();
  });
  server.on("/set_Reset", HTTP_GET, []() {
        ESP.reset();
  });
   server.on("/Reset1", HTTP_GET, []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += F("mHome - Reset");
    content += F("</title></head><body>");
    content += F("<h1>Thông Tin :</h1>");
    content += F("<p>Cty : TNHH Kim Sơn Tiến ");
    content += F("</br>Địa chỉ : 65/39 Đường 339 Phường Tăng Nhơn Phú B,Q9,TP.HCM");
    content += F("</br>Phát Triển : Phạm An Nhàn");
    content += F("</p>");
    content += F("<p>");
    content += F("<form method='get' action='set_Reset1'>");
    content += F("<input type='submit'  id=\"submitbtn\" value='Reset' onclick='return confirm(\"Tiếp Tục ?\");'></form>");
    content += F("</p>");
    content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });
  server.on("/set_Reset1", HTTP_GET, []() {
        String new_IPHC = "192.168.1.10";
        String new_userhc = "admin";
    String new_pwdhc = "admin";
    String new_global1 ="temp1";
    String new_global2 = "temp2";
    String new_ssid = "mhome";
    String new_pwd = "mhome";
    String new_ip = "192.168.1.220";
    String new_gateway = "192.168.1.1";
    String new_subnet = "255.255.255.0";
        String new_sdt1 = "x";
    String new_sdt2 = "x";
    String new_sdt3 = "x";
    String new_sdt4 = "x";
      String new_manap="*100*";
  String new_makttk="*101#";

      new_ssid.toCharArray(WiFiConf.sta_ssid, sizeof(WiFiConf.sta_ssid));
      new_pwd.toCharArray(WiFiConf.sta_pwd, sizeof(WiFiConf.sta_pwd));
      new_ip.toCharArray(WiFiConf.sta_ip, sizeof(WiFiConf.sta_ip));
      new_gateway.toCharArray(WiFiConf.sta_gateway, sizeof(WiFiConf.sta_gateway));
      new_subnet.toCharArray(WiFiConf.sta_subnet, sizeof(WiFiConf.sta_subnet));
      new_IPHC.toCharArray(WiFiConf.sta_iphc2, sizeof(WiFiConf.sta_iphc2));
      new_userhc.toCharArray(WiFiConf.sta_userhc, sizeof(WiFiConf.sta_userhc));
      new_pwdhc.toCharArray(WiFiConf.sta_passhc, sizeof(WiFiConf.sta_passhc));
      new_global1.toCharArray(WiFiConf.sta_global1, sizeof(WiFiConf.sta_global1));
      new_global2.toCharArray(WiFiConf.sta_global2, sizeof(WiFiConf.sta_global2));

      new_sdt1.toCharArray(WiFiConf.sta_SDT1, sizeof(WiFiConf.sta_SDT1));
      new_sdt2.toCharArray(WiFiConf.sta_SDT2, sizeof(WiFiConf.sta_SDT2));
      new_sdt3.toCharArray(WiFiConf.sta_SDT3, sizeof(WiFiConf.sta_SDT3));
      new_sdt4.toCharArray(WiFiConf.sta_SDT4, sizeof(WiFiConf.sta_SDT4));
      new_manap.toCharArray(WiFiConf.sta_manap, sizeof(WiFiConf.sta_manap));
      new_makttk.toCharArray(WiFiConf.sta_makttk, sizeof(WiFiConf.sta_makttk));
      
      String lan="0";
      lan.toCharArray(WiFiConf.sta_language, sizeof(WiFiConf.sta_language));
      resetModuleId();
      saveWiFiConf();
      
       ESP.reset();
  });
}

void setupWeb(void) {
  server.on("/", []() {
    IPAddress ip = WiFi.localIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += F("mHome - Main page");
    content += F("</title></head><body>");
    content += F("<h1>mHome - GSM Modem (SMS) </h1><form method='get' action='set_language'><input type=\"submit\" name=\"language\" value=\"Vietnamese\"> &nbsp &nbsp &nbsp <input type=\"submit\" name=\"language\" value=\"English\"></form>");
    content += F("<p></p>");
  

      content +=F("<fieldset>");
      content +=F("<legend style = \"color: red; font-size : 150%;align: Center\">Configuration</legend>");
      content +=F("<fieldset>");
      content +=F("<legend><a href='/wifi_conf'>Wifi setting</a></legend>");
              if (statusmang==0){ 
                  content += F("<li>Wifi : ");
                  content += F("Not Connected");
                  content += F("</br><li>GSM IP: 192.168.4.1:4999 ");
                  content += F(" ( ");
                  content += WiFiConf.module_id;
                  content += F(" )</p>");
              }
              else{
                  content += F("<li>Connected to : ");
                  content += WiFiConf.sta_ssid;
                  content += F("</br><li>GSM IP: ");
                  content += ipStr;
                  content += F(" ( ");
                  content += WiFiConf.module_id;
                  content += F(" )</p>");
               }
    content +=F("</fieldset>");
    content +=F("<fieldset>");
              content +=F("<legend ><a href='/hc2_conf'>HC2 Setting</a></legend>");
              content +=F("<li>Description:This section is for setting communication between HC2 and GSM controller");
              content +=F("<li>Status : ");
              content +=SerialHC2;
              content +=F("<li>HC2 response :");
              content +=noidung;
    content +=F("</fieldset>");
    content +=F("<fieldset>");
              content +=F("<legend><a href='/sdt_conf'>Phone Number</a></legend>");
              content +=F("<li>Phone Number 1:");
              content +=String(WiFiConf.sta_SDT1);
              content +=F("<li>Phone Number 2:");
              content +=String(WiFiConf.sta_SDT2);
              content +=F("<li>Phone Number 3:");
              content +=String(WiFiConf.sta_SDT3);
             content +=F("<li>Phone Number 4:");
              content +=String(WiFiConf.sta_SDT4);  
   
    content +=F("</fieldset>");
    content +=F("<fieldset>");
              content +=F("<legend><a href='/mang_didong'>Cellular network settings</a></legend>");
              //content +=F("<li>Description:");
                  content += F("<li>USSD for Recharge code :");
              content += String(WiFiConf.sta_manap);
             // content += F(" ><br /><br />");

            content +=F("<li>USSD for Balance Check : ");
            content += String(WiFiConf.sta_makttk) ;
            content +=F("<li>USSD response : ");
            content += noidungkiemtratk ;
    content +=F("</fieldset>");
    content +=F("</fieldset>");
    content +=F("<fieldset>");
    content +=F("<legend style =\"color: red; font-size : 150%;align: Center\">Test Function</legend>");
      content +=F("<fieldset>");
              content +=F("<legend><a href='/tinnhan'>Test SMS function.</a></legend>");
              content +=F("<li>Description: Manually sending SMS to cell phone.");
              content +=F("<li>Using to test sending SMS function.");
    content +=F("</fieldset>");
    content +=F("<fieldset>");
              content +=F("<legend><a href='/cuocgoi'>Test Calling function.</a></legend>");
              content +=F("<li>Description: Manually calling  to cell phone.");
              content +=F("<li>Using to test calling  function.");
    
    content +=F("</fieldset>");
     content +=F("</fieldset>");
    content +=F("<fieldset>");
    content +=F("<legend style =\"color: red; font-size : 150%;align: Center\">Other Function</legend>");
      content +=F("<fieldset>");
              content +=F("<legend><a href='/firmware'>Firmware Update</a></legend>");
              content +=F("<li>Description: This section is for update firmware of GSM controller");
              content +=F("<li>Status : V2.6 - 28/10/2018 -  Button Config, Put Variable to HC2");
    content +=F("</fieldset>");
    content +=F("<fieldset>");
              content +=F("<legend><a href='/Reboot'>Reboot GSM Controller</a></legend>");
              content +=F("<li>Description: Only reboot GSM module, parameter is not changed.");
    content +=F("</fieldset>");
    content +=F("<fieldset>");
            content +=F("<legend><a href='/Reset1'>Factory Reset</a></legend>");
             content +=F("<li>Description: Clear all parameters and restore to original default settings");
    content +=F("</fieldset>");
    content +=F("</fieldset>");
    content +=F("<fieldset>");
    content +=F("<legend style =\"color: red;  font-size : 150%;align: Center\">Information</legend>");
    content += F("<li>Copyright © Kim Son Tien Co.Ltd - mHome");
    content += F("<li>www.nhathongminhmhome.com");
    content += F("<li>Designed by mHome - R&D Department");
    content += F("<li>Made in VietNam");
    
    
    content +=F("</fieldset>");
    content += F("</body></html>");
    server.send(200, "text/html", content);
  });
}
//*******************
// Hafm tach IP.Gateway
//*******************
void parseBytes(const char* str, char sep, byte* bytes, int maxBytes, int base) {
    for (i = 0; i < maxBytes; i++) {
        bytes[i] = strtoul(str, NULL, base);  // Convert byte
        str = strchr(str, sep);               // Find next separator
        if (str == NULL || *str == '\0') {
            break;                            // No more separators, exit
        }
        str++;                                // Point to next character after separator
    }
}
void parseBytes1(const char* str, char sep, int address, int maxBytes, int base) {
  for (int i = 0; i < maxBytes; i++) {
    if (address == 1) ip10[i] = strtoul(str, NULL, base);  // Convert byte
    else if (address == 2) gateway10[i] = strtoul(str, NULL, base);  // Convert byte
    else if (address == 3) subnet10[i] = strtoul(str, NULL, base);  // Convert byte
   // Serial.println(bytes[i]);
    str = strchr(str, sep);               // Find next separator
    if (str == NULL || *str == '\0') {
      break;                            // No more separators, exit
    }
    str++;                                // Point to next character after separator
  }
}

///////////////////////////////////////////
//Set Varuable toi HC2 ////////////////////
//////////////////////////////////////////
void SetVariHC(String vari,String giatri) {
      HTTPClient http;
      http.begin("http://" + String(WiFiConf.sta_iphc2) + "/api/globalVariables/"+vari);
      char* user=WiFiConf.sta_userhc;
      char* pass=WiFiConf.sta_passhc;
      http.setAuthorization(user, pass);
      http.addHeader("Content-Type", "application/json");
      http.PUT("{\"value\":\"" + giatri + "\",\"invokeScenes\":true}");
     // http.writeToStream(&Serial);
      http.end();
}
void SetVariHC2Save(String vari,String giatri) { // Theo ma hoa
      int vitricat=0;    
      char tamchar[128];
      sprintf(tamchar, "%s:%s|", WiFiConf.sta_userhc, WiFiConf.sta_passhc);
      for (byte tam=0;tam<sizeof(tamchar);tam++){
            if (tamchar[tam]=='|'){
                  vitricat=tam;
                  break;
            }
      }
      int encodedLen = base64_enc_len(vitricat-1);
      char encoded[encodedLen];
      base64_encode(encoded, tamchar, vitricat);
      HTTPClient http;
      http.begin("http://" + String(WiFiConf.sta_iphc2) + "/api/globalVariables/"+vari);
      http.setAuthorization(encoded);
      http.addHeader("Content-Type", "application/json");
      http.PUT("{\"value\":\"" + giatri + "\",\"invokeScenes\":true}");
      http.writeToStream(&Serial);
      http.end();
}
////////////////////////////////////
//Get thông số Hc2 /////////
//////////////////////////////////
void getHC() {
  WiFiClient client;
  if (!client.connect(WiFiConf.sta_iphc2,80)) {  
     SerialHC2=F("connection failed");      
    return;
  }
  String url = F("/api/settings/info");
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: "+String(WiFiConf.sta_iphc2)+"\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      //Serial.println(">>> Client Timeout !");
      client.stop();
      SerialHC2=F("HC2 not connected");
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
     if (line.length()>70) {
      line.replace("{","");
      line.replace("}","");
      line.replace("\""," ");
      line.remove(76);
      SerialHC2=line;}
    
  }
}

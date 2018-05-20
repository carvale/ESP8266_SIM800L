void connect_wifi(){
  
  boolean kq=scanWiFireturn();
  if (kq){
   //  WiFi.begin("mHomeRD", "123789456");
          WiFi.begin(WiFiConf.sta_ssid, WiFiConf.sta_pwd);
          parseBytes1(WiFiConf.sta_ip, '.', 1, 4, 10);
          parseBytes1(WiFiConf.sta_gateway, '.', 2, 4, 10);
          parseBytes1(WiFiConf.sta_subnet, '.', 3, 4, 10);
          WiFi.config(ip10,gateway10,subnet10,DNS);
          DEBUG_PRINTLN(WiFiConf.sta_ip);
          DEBUG_PRINTLN(WiFiConf.sta_ssid);
 }

}
boolean scanWiFireturn(void) {
  int founds = WiFi.scanNetworks();
  char ten_wifi[32];
  for (int i = 0; i < founds; ++i)
  {  
    WiFi.SSID(i).toCharArray(ten_wifi, sizeof(ten_wifi));
   if (strstr(ten_wifi,WiFiConf.sta_ssid) != NULL){return true;}   
  }
  return false;
}
long tacksotustring(String kitu){
 // Serial.println(kitu);
  
  char a[12];
  int len=kitu.length();
  char s[len];
  kitu.toCharArray(s, sizeof(s));
  int j,dai; 
    len=strlen(s);
    byte thoat=0;
    j=0;
    for(int i=8;i<len;i++)
    {
        if(s[i]>='0' && s[i]<='9')
        {
           if (thoat == 0 || thoat == 1){
            a[j++]=s[i];
            thoat=1;
           }
        }
        else if (thoat == 1){thoat=2;da_kttk= true;};
    }
    a[j] = 0;
    long trave=atol(a);
   // Serial.println(trave);
    return  trave;
}


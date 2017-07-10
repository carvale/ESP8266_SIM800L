void connect_wifi(){
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(WiFiConf.sta_ssid, WiFiConf.sta_pwd);
  parseBytes1(WiFiConf.sta_ip, '.', 1, 4, 10);
  parseBytes1(WiFiConf.sta_gateway, '.', 2, 4, 10);
  parseBytes1(WiFiConf.sta_subnet, '.', 3, 4, 10);
  WiFi.config(ip10,gateway10,subnet10,DNS);
}
long tacksotustring(String kitu){
  char a[10];
  int len=kitu.length();
  char s[len];
  kitu.toCharArray(s, sizeof(s));
  int i,j,dai; 
    len=strlen(s);
    byte thoat=0;
    j=0;
    for(i=0;i<len;i++)
    {
        if(s[i]>='0' && s[i]<='9')
        {
           if (thoat == 0 || thoat == 1){
            a[j++]=s[i];
            thoat=1;
           }
        }
        else if (thoat == 1)thoat=2;
    }
    a[j] = 0;
    return  atol(a);
}


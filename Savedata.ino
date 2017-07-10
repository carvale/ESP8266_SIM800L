void connect_wifi(){
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(WiFiConf.sta_ssid, WiFiConf.sta_pwd);
  parseBytes1(WiFiConf.sta_ip, '.', 1, 4, 10);
  parseBytes1(WiFiConf.sta_gateway, '.', 2, 4, 10);
  parseBytes1(WiFiConf.sta_subnet, '.', 3, 4, 10);
  WiFi.config(ip10,gateway10,subnet10,DNS);
}


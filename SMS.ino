///////////////////////////////////////////////////////////////////////////
//11/12  Add delay(1) vào hàm sendAT với SendAtString
//12/12 Theme server.handleClient(); vao cac cuoc goi va send AT
//Giamr timeout vaf tang 
//////////////////////////////////////////////////////////////////////
void receive_uart() { 
  
  if(Serial.available()>0) {
   
    uart_tam= Serial.read();
   // Serial.println(c);
    if (xbuff >= buffer_size) {
      xbuff=0;     
    }
    else{ 
      buffer[xbuff++]=uart_tam;
    } 
    if (strstr(buffer, "CMT") != NULL)    
    {      // Serial.println("CMT OK");
            xbuff=0;
            memset(buffer, '\0', buffer_size);      // Initialize the string
            memset(response, '\0', buffer_size); 
            x=0;   
           previous = millis();  
           answer=0;
            do {delay(1);
                if(Serial.available() != 0){
                    response[x] = Serial.read();
                    //if (x>199){x=0;}
                    x++;
                    if (strstr(response, "\n") != NULL)    
                    {
                      answer = 1;
                    }              
                }          
          }
          while((answer == 0) && ((millis() - previous) < 5000));   
          
          char * strtokIndx; // this is used by strtok() as an index
          strtokIndx = strtok(response, "\""); 
          strtokIndx = strtok(NULL, "\""); // step
          String SDT=strtokIndx;
          SDT.toCharArray(phone_number, 13);
          strncpy(phone_number, phone_number + 3,13);
          answer=0;
          x=0;
          memset(response, '\0', buffer_size); 
          previous = millis();  
          do {delay(2);
                if(Serial.available() != 0){                    
                    response[x] = Serial.read();
                    //x++;
                    //if (x>199){x=0;}
                     x++;
                    if (strstr(response, "\n") != NULL){
                      answer = 1;
                    }              
                }
          } while((answer == 0) && ((millis() - previous) < 5000));  
          if ((strstr(WiFiConf.sta_SDT1,phone_number) != NULL) or (strstr( WiFiConf.sta_SDT2,phone_number) != NULL) or (strstr( WiFiConf.sta_SDT3,phone_number) != NULL) or (strstr(WiFiConf.sta_SDT4,phone_number) != NULL)){
                  if (strstr(response,"KTTK") != NULL) {statusnaptk=1;kttkcusd();}
                  else if (strstr(response,"On") != NULL) {
                        digitalWrite(5,HIGH);         
                        String bienvari=String(WiFiConf.sta_global1);
                        bienvari.trim();
                        SetVariHC(bienvari,"1");
                        send_SMS("SDT 0"+ String(phone_number) +" ON ARLAM");
                  }
                  else if (strstr(response,"Off") != NULL) {                 
                        digitalWrite(5,LOW);
                        String bienvari=String(WiFiConf.sta_global1);
                        bienvari.trim();
                        SetVariHC(bienvari,"0");
                        send_SMS("SDT 0"+ String(phone_number) +" OFF ARLAM");
                  }
                 else if (strstr(response,"OK") != NULL) {                 
                        String bienvari=String(WiFiConf.sta_global1);
                        bienvari.trim();
                        SetVariHC(bienvari,"OK");
                        send_SMS("SDT 0"+ String(phone_number) +" Da Kiem Tra : OK");
                  }
                 /* else if (strstr(response,"Tat") != NULL) {                 
                        String bienvari=String(WiFiConf.sta_global2);
                        bienvari.trim();
                        SetVariHC(bienvari,"0");
                        send_SMS("SDT 0"+ String(phone_number) +" OFF TB");
                        
                  }
                 else if (strstr(response,"Mo") != NULL) {                 
                        String bienvari=String(WiFiConf.sta_global2);
                        bienvari.trim();
                        SetVariHC(bienvari,"1");
                        send_SMS("SDT 0"+ String(phone_number) +" ON TB");
                  }*/
                  else if (strstr(response,"NAP") != NULL) {
                                statusnaptk=1;
                                char * strtokIndx; // this is used by strtok() as an index
                                strtokIndx = strtok(response, ":"); 
                                strtokIndx = strtok(NULL, "."); // step
                                String kt=strtokIndx;
                                String manap=WiFiConf.sta_manap;
                                manap.trim();
                                manap=manap+kt;
                                manap=manap+"#";
                                kttk(manap);                  
                  }
          }
          else {
            Serial.println("AT");
          }
   }
   else if(strstr(buffer, "CUSD") != NULL){   
            xbuff=0;    
           x=0;
           memset(buffer, '\0', buffer_size);      // Initialize the string
            memset(response, '\0', buffer_size); 
           i=0;           
           previous = millis(); 
           answer = 0; 
           do {delay(1);
                if(Serial.available() != 0){
                    response[x] = Serial.read();                             
                    if (i>3){x++;}
                    else i++;
                  //  if (x>199){x=0;}
                if (strstr(response, "\n") != NULL){
                  answer = 1;                  
                }              
            }         
          }
          while((answer == 0) && ((millis() - previous) < 5000));  
      
          String manap=response;
          manap.trim();
          manap.replace("\"", "");
          noidungkiemtratk=manap;
          SetVariHC("SIM_TK",manap);
          if (statusnaptk==1){send_SMS(manap);}
  } 
  else if(strstr(buffer, "CLIP") != NULL)    
      { yield();
        xbuff=0;
          x=0;
            memset(buffer, '\0', buffer_size);      // Initialize the string
            memset(response, '\0', buffer_size); 
          previous = millis();  
          answer=0;
               do {delay(2);
                if(Serial.available() != 0){
                    response[x] = Serial.read();
                    x++;
                   // if (x>199){x=0;}
                if (strstr(response, "\n") != NULL){answer = 1;}              
            }          
          }
          while((answer == 0) && ((millis() - previous) < 5000)); 
         
          char * strtokIndx; // this is used by strtok() as an index
          strtokIndx = strtok(response, "\""); 
          strtokIndx = strtok(NULL, "\""); // step
          String SDT=strtokIndx;
          for(i=0;i<13;i++){phone_number[i]=0x00;}
          SDT.toCharArray(phone_number, 13);
          if ((strstr(WiFiConf.sta_SDT1,phone_number) != NULL) or (strstr( WiFiConf.sta_SDT2,phone_number) != NULL) or (strstr( WiFiConf.sta_SDT3,phone_number) != NULL) or (strstr(WiFiConf.sta_SDT4,phone_number) != NULL)){           
            Serial.println(F("ATA"));
          }
          else {
            Serial.println(F("ATH"));
          }
      }
   else if(strstr(buffer,"SMS Ready") != NULL){ guitinnhan=6;
                  memset(buffer, '\0', buffer_size);      // Initialize the string
                  memset(response, '\0', buffer_size); 
                  xbuff=0;
      }
  else if (uart_tam=='+'){xbuff=0; }     
  }
}
void init_SIM900A() { 
 // wdt_disable();
 answer=0;
  int bien=0;
  do { answer = sendAT("ATE0","OK",1000);bien=bien+1; if(bien>5){answer=1;}} while(answer==0); //Tat Echo
  bien=0;
  do { answer = sendAT("AT+IPR=115200","OK",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);
  bien=0;
  do { answer = sendAT("AT+CREG?","+CREG: 0,1",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);     // Checking status of Connecting to the network
  bien=0;
  do { answer = sendAT("AT+CMGF=1", "OK",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);          // Set SMS into text mode
  bien=0;
  answer=0;
  do { answer = sendAT("AT+CNMI=2,2,0,0,0", "OK",300);bien=bien+1; if(bien>10){answer=1;}} while(answer==0);  // Set Message mode when receive new SMS 
  bien=0;
  do { answer = sendAT("AT+CLIP=1", "OK",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);
  bien=0;
  do { answer = sendAT("AT+CSAS", "OK",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);
  bien=0;
  do { answer = sendAT("AT+CSQ", "OK",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);
  answer=0;
  yield();
  }
//******************************************************************************************************
/*void erase_buffer() { 
  memset(buffer, '\0', buffer_size);      // Initialize the string
  memset(response, '\0', buffer_size);
  memset(aux_string, '\0', 150); 
  xbuff=0;
  uart_tam='\0';
 // c1='\0';
  return;
}*/
//******************************************************************************************************
int sendAT(char* ATcommand, char* expected_answer, unsigned int timeout) {
  memset(buffer, '\0', buffer_size);      // Initialize the string
  memset(response, '\0', buffer_size);
   delay(1);
   answer=0;
  Serial.println(ATcommand);
  x = 0;
  previous = millis();
  do {
    if(Serial.available() != 0){    
      response[x] = Serial.read();
      x++;
      if (strstr(response, expected_answer) != NULL){answer=1;return answer;}
    } 
    delay(1);
  }
  while((answer==0)&& (millis() - previous) < timeout);
  return answer;
}
void delay_nhan(unsigned int timeout){
  previous = millis();
  do {
    server.handleClient();
    delay(1);
  }
  while((millis() - previous) < timeout); 
}
//******************************************************************************************************
void power_on(){ 
     if(answer==0)
       {
       int bien=0;
        while(answer==0) {answer = sendAT("AT", "OK", 200); delay(20);bien=bien+1; if(bien>10){answer=1;}} 
       }
       sendAT("AT+CFUN=1","OK",1000);
     }
     //////////////////////////////////////////////
     /////////Hàm gửi tin nhắn  ///////////////////
     ////////////////////////////////////////////////
void send_SMS(String noidungsms) {
  int bien=0;
    answer=0;
    if (WiFiConf.sta_SDT1[0]!='x'){
     sprintf(aux_string,"AT+CMGS=\"%s\"",WiFiConf.sta_SDT1);
     do { if (bien%100==0){answer = sendAT(aux_string,">",1000);} bien=bien+1;delay_nhan(50);if(bien>200){answer=2;}} while(answer==0); 
     if(answer==1){ Serial.print(noidungsms);Serial.write(26);}
     else Serial.write(26);
    }
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT2[0]!='x'){
     delay(2000);
     sprintf(aux_string,"AT+CMGS=\"%s\"",WiFiConf.sta_SDT2);
     do { if (bien%100==0){answer = sendAT(aux_string,">",1000);}bien=bien+1;delay_nhan(50);if(bien>200){answer=2;}} while(answer==0); 
     if(answer==1){ Serial.print(noidungsms);Serial.write(26);}
     else Serial.write(26);
    }
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT3[0]!='x'){
      delay(2000);
     sprintf(aux_string,"AT+CMGS=\"%s\"",WiFiConf.sta_SDT3);
     do { if (bien%100==0){answer = sendAT(aux_string,">",1000);}bien=bien+1;delay_nhan(50);if(bien>200){answer=2;}} while(answer==0); 
     if(answer==1){ Serial.print(noidungsms);Serial.write(26);  }
     else Serial.write(26);
    }
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT4[0]!='x'){
     delay(2000);
     sprintf(aux_string,"AT+CMGS=\"%s\"",WiFiConf.sta_SDT4);
     do { if (bien%100==0){answer = sendAT(aux_string,">",1000);} bien=bien+1;delay_nhan(50);if(bien>200){answer=2;}} while(answer==0); 
     if(answer==1){ Serial.print(noidungsms);Serial.write(26);  }
     else Serial.write(26);
    }
    statusnaptk=0;
    yield();
} 
void send_SMS1(String noidungsms) {
  int bien=0;
    answer=0;
    if (WiFiConf.sta_SDT1[0]!='x'){
      
     sprintf(aux_string,"AT+CMGS=\"%s\"",WiFiConf.sta_SDT1);
     do { if (bien%100==0){answer = sendAT(aux_string,">",1000);}bien=bien+1;delay_nhan(50); if(bien>200){answer=2;}} while(answer==0); 
     if(answer==1){ Serial.print(noidungsms);Serial.write(26);}
     else Serial.write(26);
    }
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT2[0]!='x'){
      delay(2000);
     sprintf(aux_string,"AT+CMGS=\"%s\"",WiFiConf.sta_SDT2);
     do { if (bien%100==0){answer = sendAT(aux_string,">",1000);}bien=bien+1;delay_nhan(50);if(bien>200){answer=2;}} while(answer==0); 
     if(answer==1){ Serial.print(noidungsms);Serial.write(26);}
     else Serial.write(26);
    }
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT3[0]!='x'){
      delay(2000);
     sprintf(aux_string,"AT+CMGS=\"%s\"",WiFiConf.sta_SDT3);
     do { if (bien%100==0){answer = sendAT(aux_string,">",1000);} bien=bien+1;delay_nhan(50);if(bien>200){answer=2;}} while(answer==0); 
     if(answer==1){ Serial.print(noidungsms);Serial.write(26);  }
     else Serial.write(26);
    }
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT4[0]!='x'){
      delay(2000);
     sprintf(aux_string,"AT+CMGS=\"%s\"",WiFiConf.sta_SDT4);
     do { if (bien%100==0){answer = sendAT(aux_string,">",1000);} bien=bien+1;delay_nhan(50);if(bien>200){answer=2;}} while(answer==0); 
     if(answer==1){ Serial.print(noidungsms);Serial.write(26);  }
     else Serial.write(26);
    }
    
    bien=0;
    answer=0;
    if (sdt_new[0]=='0'){
     delay(2000);
     sprintf(aux_string,"AT+CMGS=\"%s\"",sdt_new);
     do { if (bien%100==0){answer = sendAT(aux_string,">",1000);} bien=bien+1;delay_nhan(50);if(bien>200){answer=2;}} while(answer==0); 
     if(answer==1){ Serial.print(noidungsms);Serial.write(26);  }
     else Serial.write(26);
    }
    statusnaptk=0;
    yield();
}   
     //////////////////////////////////////////////
     /////////Hàm gửi tin nhắn gọi điện  ///////////////////
     ////////////////////////////////////////////////
void goidt() {  
  int bien=0;
  answer=0;
  if (WiFiConf.sta_SDT1[0]!='x'){
    //delay(100);
     sprintf(aux_string,"ATD%s;",WiFiConf.sta_SDT1);
     do { if (bien%2500==0){answer = sendAT(aux_string,"OK",500);}bien=bien+1;delay_nhan(10);if(bien>5000){answer=1;break;}} while(answer==0); 
    }
    yield();
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT2[0]!='x'){
      delay_nhan(1000);
     sprintf(aux_string,"ATD%s;",WiFiConf.sta_SDT2);
     do { if (bien%2500==0){answer = sendAT(aux_string,"OK",500);}bien=bien+1;delay_nhan(10);if(bien>5000){answer=1;break;}} while(answer==0); 
    }
    yield();
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT3[0]!='x'){
      delay_nhan(1000);
     sprintf(aux_string,"ATD%s;",WiFiConf.sta_SDT3);
     do { if (bien%2500==0){answer = sendAT(aux_string,"OK",500);}bien=bien+1;delay_nhan(10);if(bien>5000){answer=1;break;}} while(answer==0); 
    }
    yield();
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT4[0]!='x'){
      delay_nhan(1000);
     sprintf(aux_string,"ATD%s;",WiFiConf.sta_SDT4);
     do { if (bien%2500==0){answer = sendAT(aux_string,"OK",500);} bien=bien+1;delay_nhan(10);if(bien>5000){answer=1;break;}} while(answer==0); 
    }
    yield();
    xbuff=0;
} 
void goidt2() {  
  int bien=0;
  answer=0;
  if (WiFiConf.sta_SDT1[0]!='x'){
    //delay(1000);
     sprintf(aux_string,"ATD%s;",WiFiConf.sta_SDT1);
     do { if (bien%250==0){answer = sendAT(aux_string,"OK",500);}bien=bien+1;delay_nhan(50);if(bien>1000){answer=1;break;}} while(answer==0); 
    }
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT2[0]!='x'){
      delay_nhan(1000);
     sprintf(aux_string,"ATD%s;",WiFiConf.sta_SDT2);
     do { if (bien%250==0){answer = sendAT(aux_string,"OK",500);}bien=bien+1;delay_nhan(50);if(bien>1000){answer=1;break;}} while(answer==0); 
    }
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT3[0]!='x'){
      delay_nhan(1000);
     sprintf(aux_string,"ATD%s;",WiFiConf.sta_SDT3);
     do { if (bien%200==0){answer = sendAT(aux_string,"OK",500);}bien=bien+1;delay_nhan(50);if(bien>1000){answer=1;break;}} while(answer==0); 
    }
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT4[0]!='x'){
      delay_nhan(1000);
     sprintf(aux_string,"ATD%s;",WiFiConf.sta_SDT4);
     do { if (bien%200==0){answer = sendAT(aux_string,"OK",500);} bien=bien+1;delay_nhan(50);if(bien>1000){answer=1;break;}} while(answer==0); 
    }
        bien=0;
    answer=0;
    if (sdt_new[0]=='0'){
     delay_nhan(1000);
    sprintf(aux_string,"ATD%s;",sdt_new);
     do { if (bien%200==0){answer = sendAT(aux_string,"OK",500);} bien=bien+1;delay_nhan(50);if(bien>1000){answer=1;break;}} while(answer==0); 
    }
    xbuff=0;
    yield();
} 
     //////////////////////////////////////////////
     /////////Hàm nap tien  ///////////////////
     ////////////////////////////////////////////////
void kttk(String nd) {
 nd="AT+CUSD=1,\""+nd+"\"";
 Serial.println(nd);
 xbuff=0;
}
      //////////////////////////////////////////////
     /////////Hàm KTTK  ///////////////////
     ////////////////////////////////////////////////
void kttkcusd() {int bien=0;
              sprintf(aux_string,"AT+CUSD=1,\"%s\"",WiFiConf.sta_makttk);
              do { answer = sendAT(aux_string,"OK",200);bien=bien+1;delay_nhan(20); if(bien>5){answer=1;}} while(answer==0); 
}


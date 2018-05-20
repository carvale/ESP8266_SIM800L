///////////////////////////////////////////////////////////////////////////
//11/12  Add delay(1) vào hàm sendAT với SendAtString
//12/12 Theme server.handleClient(); vao cac cuoc goi va send AT
//Giamr timeout vaf tang 
//////////////////////////////////////////////////////////////////////
void receive_uart() { 
#ifdef USING_SIM
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
                      //  digitalWrite(OUT3,HIGH);         
                        String bienvari=String(WiFiConf.sta_global1);
                        bienvari.trim();
                        SetVariHC(bienvari,"1");
                        send_SMS("SDT 0"+ String(phone_number) +" ON ARLAM");
                  }
                  else if (strstr(response,"Off") != NULL) {                 
                       // digitalWrite(OUT3,LOW);
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
          while((answer == 0) && ((millis() - previous) < 2000));  
      
          String manap=response;
          manap.trim();
          manap.replace("\"", "");
          noidungkiemtratk=manap;
          sotien=tacksotustring(noidungkiemtratk);
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
          while((answer == 0) && ((millis() - previous) < 1000)); 
         
          char * strtokIndx; // this is used by strtok() as an index
          strtokIndx = strtok(response, "\""); 
          strtokIndx = strtok(NULL, "\""); // step
          String SDT=strtokIndx;
          for(int i=0;i<13;i++){phone_number[i]=0x00;}
          SDT.toCharArray(phone_number, 13);
          if ((strstr(WiFiConf.sta_SDT1,phone_number) != NULL) or (strstr( WiFiConf.sta_SDT2,phone_number) != NULL) or (strstr( WiFiConf.sta_SDT3,phone_number) != NULL) or (strstr(WiFiConf.sta_SDT4,phone_number) != NULL)){           
            Serial.println(F("ATA"));
          }
          else {
            Serial.println(F("ATH"));
          }
      }
   else if(strstr(buffer, "DTMF:") != NULL)    
      { yield();
        xbuff=0;
          x=0;
            memset(buffer, '\0', buffer_size);      // Initialize the string
            memset(response, '\0', buffer_size); 
          previous = millis();  
          answer=0;
          do {
                if(Serial.available() != 0){
                    response[x] = Serial.read();
                    x++;
                if (strstr(response, "\n") != NULL){answer = 1;}              
            }          
          }
          while((answer == 0) && ((millis() - previous) < 1000)); 
         if (strstr(response,"1") != NULL) {
          dtmf_sim=1;
         }
         else if (strstr(response,"2") != NULL) {
          dtmf_sim=2;
         }
                  else if (strstr(response,"3") != NULL) {
                    dtmf_sim=3;
         }
                  else if (strstr(response,"4") != NULL) {
                    dtmf_sim=4;
         }
                  else if (strstr(response,"5") != NULL) {
                    dtmf_sim=5;
         }
                  else if (strstr(response,"6") != NULL) {
                    dtmf_sim=6;
         }
                  else if (strstr(response,"7") != NULL) {
                    dtmf_sim=7;
         }
                  else if (strstr(response,"8") != NULL) {
                    dtmf_sim=8;
         }
                  else if (strstr(response,"9") != NULL) {
                    dtmf_sim=9;
         }
                  else if (strstr(response,"0") != NULL) {
                    dtmf_sim=0;
         }
         
      }
   else if(strstr(buffer,"SMS Ready") != NULL){ guitinnhan=0;
                  memset(buffer, '\0', buffer_size);      // Initialize the string
                  memset(response, '\0', buffer_size); 
                  xbuff=0;
  }
  else if (uart_tam=='+'){xbuff=0;
                  memset(buffer, '\0', buffer_size);      // Initialize the string
                  memset(response, '\0', buffer_size);  
 }     
  }
#endif
}
void init_SIM900A() { 
 // wdt_disable();
 answer=0;
 #ifdef USING_SIM
  int bien=0;
  do { answer = sendAT("ATE0","OK",1000);bien=bien+1; if(bien>5){answer=1;}} while(answer==0); //Tat Echo
  bien=0;
  do { answer = sendAT("AT+IPR=0","OK",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);
  bien=0;
  do { answer = sendAT("AT+CREG?","+CREG: 0,1",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);     // Checking status of Connecting to the network
  bien=0;
  do { answer = sendAT("AT+CMGF=1", "OK",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);          // Set SMS into text mode
  bien=0;
  do { answer = sendAT("AT+CNMI=2,2,0,0,0", "OK",300);bien=bien+1; if(bien>10){answer=1;}} while(answer==0);  // Set Message mode when receive new SMS 
  bien=0;
  do { answer = sendAT("AT+CLIP=1", "OK",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);
  bien=0;
  do { answer = sendAT("AT+CSAS", "OK",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);
  bien=0;
  do { answer = sendAT("AT+CSQ", "OK",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);
  bien=0;
  do { answer = sendAT("AT + DDET = 1,1000,0,0", "OK",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);
    bien=0;
  do { answer = sendAT("AT + CMGDA =\"DEL ALL\"", "OK",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);
  answer=0;
  yield();
  #endif
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
  #ifdef USING_SIM
  memset(buffer, '\0', buffer_size);      // Initialize the string
  memset(response, '\0', buffer_size);
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
#endif
  return answer;
}
int sendATSMS(char* ATcommand, char* expected_answer, unsigned int timeout) {

  memset(buffer, '\0', buffer_size);      // Initialize the string
  memset(response, '\0', buffer_size);
   answer=0;
#ifdef USING_SIM
  Serial.print(ATcommand);
  Serial.print("\r");
  x = 0;
  previous = millis();
  do {
    if(Serial.available() != 0){    
      response[x] = Serial.read();
      x++;
      if (strstr(response, expected_answer) != NULL){answer=1;return answer;}
    } 
    delay(1);
    yield();
  }
  while((answer==0)&& (millis() - previous) < timeout);
#endif
  return answer;
}
void delay_nhan(unsigned int timeout){
  previous = millis();
  do {
    server.handleClient();
    delay(1);
    yield();
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

boolean send_end() {
#ifdef USING_SIM
  memset(buffer, '\0', buffer_size);      // Initialize the string
  memset(response, '\0', buffer_size);
   answer=0;
  Serial.write(0x1A);
  x = 0;
  previous = millis();
  do {
    if(Serial.available() != 0){    
      response[x] = Serial.read();
      x++;
      if (strstr(response, "+CMGS") != NULL){answer=1;return true;}
    } 
    delay(1);
  }
  while((answer==0)&& (millis() - previous) < 2000);
#endif
  return false;
}
     //////////////////////////////////////////////
     /////////Hàm gửi tin nhắn  //////////////////
     ////////////////////////////////////////////
void send_SMS(String noidungsms) {
#ifdef USING_SIM
  int bien=0;
    answer=0;
     if (WiFiConf.sta_SDT1[0]!='x'){
      sendAT("AT+CMGF=1", "OK",200);
      delay(500);
     sprintf(aux_string,"AT+CMGS=\"%s\"",WiFiConf.sta_SDT1);
     do { if (bien%100==0){answer = sendATSMS(aux_string,">",1000);} bien=bien+1;delay_nhan(50);if(bien>200){answer=2;}} while(answer==0); 
     if(answer==1){ Serial.print(noidungsms);delay(100);send_end();}
     else {delay(100);Serial.write((char)27);}
    }
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT2[0]!='x'){
      delay(1000);
      sendAT("AT+CMGF=1", "OK",200);
     delay(500);
     sprintf(aux_string,"AT+CMGS=\"%s\"",WiFiConf.sta_SDT2);
     do { if (bien%50==0){answer = sendATSMS(aux_string,">",1000);}bien=bien+1;delay_nhan(50);if(bien>200){answer=2;}} while(answer==0); 
     if(answer==1){ Serial.print(noidungsms);delay(100);send_end();}
     else {delay(100);Serial.write((char)27);}
    }
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT3[0]!='x'){
      delay(1000);
      sendAT("AT+CMGF=1", "OK",200);
     delay(500);
     sprintf(aux_string,"AT+CMGS=\"%s\"",WiFiConf.sta_SDT3);
     do { if (bien%50==0){answer = sendATSMS(aux_string,">",1000);}bien=bien+1;delay_nhan(50);if(bien>200){answer=2;}} while(answer==0); 
     if(answer==1){ Serial.print(noidungsms);delay(100);send_end();}
     else {delay(100);Serial.write((char)27);}
    }
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT4[0]!='x'){
      delay(1000);
      sendAT("AT+CMGF=1", "OK",200);
    delay(500);
     sprintf(aux_string,"AT+CMGS=\"%s\"",WiFiConf.sta_SDT4);
     do { if (bien%50==0){answer = sendATSMS(aux_string,">",1000);} bien=bien+1;delay_nhan(50);if(bien>200){answer=2;}} while(answer==0); 
     if(answer==1){ Serial.print(noidungsms);delay(100);send_end();}
     else {delay(100);Serial.write((char)27);}
    }
    statusnaptk=0;
    yield();
#endif
} 
void send_SMS1(String noidungsms) {
#ifdef USING_SIM
  int bien=0;
    answer=0;
    if (WiFiConf.sta_SDT1[0]!='x'){
      sendAT("AT+CMGF=1", "OK",200);
      delay(500);
     sprintf(aux_string,"AT+CMGS=\"%s\"",WiFiConf.sta_SDT1);
     do { if (bien%100==0){answer = sendATSMS(aux_string,">",1000);} bien=bien+1;delay_nhan(50);if(bien>200){answer=2;}} while(answer==0); 
     if(answer==1){ Serial.print(noidungsms);delay(100);send_end();}
     else {delay(100);Serial.write((char)27);}
    }
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT2[0]!='x'){
      delay(1000);
      sendAT("AT+CMGF=1", "OK",200);
     delay(500);
     sprintf(aux_string,"AT+CMGS=\"%s\"",WiFiConf.sta_SDT2);
     do { if (bien%50==0){answer = sendATSMS(aux_string,">",1000);}bien=bien+1;delay_nhan(50);if(bien>200){answer=2;}} while(answer==0); 
     if(answer==1){ Serial.print(noidungsms);delay(100);send_end();}
     else {delay(100);Serial.write((char)27);}
    }
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT3[0]!='x'){
      delay(1000);
      sendAT("AT+CMGF=1", "OK",200);
     delay(500);
     sprintf(aux_string,"AT+CMGS=\"%s\"",WiFiConf.sta_SDT3);
     do { if (bien%50==0){answer = sendATSMS(aux_string,">",1000);}bien=bien+1;delay_nhan(50);if(bien>200){answer=2;}} while(answer==0); 
     if(answer==1){ Serial.print(noidungsms);delay(100);send_end();}
     else {delay(100);Serial.write((char)27);}
    }
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT4[0]!='x'){
      delay(1000);
      sendAT("AT+CMGF=1", "OK",200);
    delay(500);
     sprintf(aux_string,"AT+CMGS=\"%s\"",WiFiConf.sta_SDT4);
     do { if (bien%50==0){answer = sendATSMS(aux_string,">",1000);} bien=bien+1;delay_nhan(50);if(bien>200){answer=2;}} while(answer==0); 
     if(answer==1){ Serial.print(noidungsms);delay(100);send_end();}
     else {delay(100);Serial.write((char)27);}
    }
    
    bien=0;
    answer=0;
    if (sdt_new[0]=='0'){
      delay(1000);
      sendAT("AT+CMGF=1", "OK",200);
    delay(500);
     sprintf(aux_string,"AT+CMGS=\"%s\"",sdt_new);
     do { if (bien%50==0){answer = sendATSMS(aux_string,">",1000);} bien=bien+1;delay_nhan(50);if(bien>200){answer=2;}} while(answer==0); 
     if(answer==1){ Serial.print(noidungsms);delay(100);send_end();}
     else {delay(100);Serial.write((char)27);}
    }
    statusnaptk=0;
    yield();
 #endif
}   
     //////////////////////////////////////////////
     /////////Hàm gửi tin nhắn gọi điện  /////////
     ////////////////////////////////////////////
boolean end_cuocgoi(){
#ifdef USING_SIM
    memset(buffer, '\0', buffer_size);      // Initialize the string
  memset(response, '\0', buffer_size);
   answer=0;
  x = 0;
  previous = millis();
  do {
    if(Serial.available() != 0){    
      response[x] = Serial.read();
      x++;
      if (strstr(response, "NO ANSWER") != NULL){answer=1;return false;} //Khong Bat MAy
      else if (strstr(response, "BUSY") != NULL){answer=1;return true;} // Tu Choi Cuoc Goi
      else if (strstr(response, "NO CARRIER") != NULL){answer=1;return true;} // Bat MAy 
      else if(strstr(response, "DTMF:") != NULL){
          x=0;
          memset(response, '\0', buffer_size); 
          previous = millis();  
          answer=0;
          do {
                if(Serial.available() != 0){
                    response[x] = Serial.read();
                    x++;
                if (strstr(response, "\n") != NULL){answer = 1;}              
            }          
          }
         while((answer == 0) && ((millis() - previous) < 1000)); 
         if      (strstr(response,"1") != NULL) dtmf_sim=1;
         else if (strstr(response,"2") != NULL) dtmf_sim=2;
         else if (strstr(response,"3") != NULL) dtmf_sim=3;
         else if (strstr(response,"4") != NULL) dtmf_sim=4;
         else if (strstr(response,"5") != NULL) dtmf_sim=5;
         else if (strstr(response,"6") != NULL) dtmf_sim=6;
         else if (strstr(response,"7") != NULL) dtmf_sim=7;
         else if (strstr(response,"8") != NULL) dtmf_sim=8;
         else if (strstr(response,"9") != NULL) dtmf_sim=9;
         else if (strstr(response,"0") != NULL) dtmf_sim=0;
         if (dtmf_sim != 10){SetVariHC("SIM_DTMF",String(dtmf_sim));
                    dtmf_sim=10;
         }
      }
    } 
    delay(10);
    yield();
  }
  while((answer==0)&& (millis() - previous) < 60000);
#endif
  return false;
}
void goidt(byte khancap) {  
#ifdef USING_SIM
  int bien=0;
  answer=0;
  boolean end_cc=false;
  if (WiFiConf.sta_SDT1[0]!='x'){
    //delay(100);
    delay(1000);
     sprintf(aux_string,"ATD%s;",WiFiConf.sta_SDT1);
     do { if (bien%2500==0){answer = sendAT(aux_string,"OK",500);}bien=bien+1;delay_nhan(10);if(bien>5000){answer=1;break;}} while(answer==0); 
         end_cc=end_cuocgoi();
    if (end_cc) {sprintf(aux_string,"%s - Return",WiFiConf.sta_SDT1);SetVariHC("SIM_CALL",aux_string);return;}
    }
    yield();
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT2[0]!='x'){
      delay_nhan(1000);
     sprintf(aux_string,"ATD%s;",WiFiConf.sta_SDT2);
     do { if (bien%2500==0){answer = sendAT(aux_string,"OK",500);}bien=bien+1;delay_nhan(10);if(bien>5000){answer=1;break;}} while(answer==0); 
         end_cc=end_cuocgoi();
        if (end_cc) {sprintf(aux_string,"%s - Return",WiFiConf.sta_SDT1);SetVariHC("SIM_CALL",aux_string);return;}
    }    
    yield();
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT3[0]!='x'){
      delay_nhan(1000);
     sprintf(aux_string,"ATD%s;",WiFiConf.sta_SDT3);
     do { if (bien%2500==0){answer = sendAT(aux_string,"OK",500);}bien=bien+1;delay_nhan(10);if(bien>5000){answer=1;break;}} while(answer==0); 
         end_cc=end_cuocgoi();
    if (end_cc) {sprintf(aux_string,"%s - Return",WiFiConf.sta_SDT1);SetVariHC("SIM_CALL",aux_string);return;}
    }

    yield();
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT4[0]!='x'){
      delay_nhan(1000);
     sprintf(aux_string,"ATD%s;",WiFiConf.sta_SDT4);
     do { if (bien%2500==0){answer = sendAT(aux_string,"OK",500);}bien=bien+1;delay_nhan(10);if(bien>5000){answer=1;break;}} while(answer==0); 
         end_cc=end_cuocgoi();
    if (end_cc) {sprintf(aux_string,"%s - Return",WiFiConf.sta_SDT1);SetVariHC("SIM_CALL",aux_string);return;}
    }
    yield();
    xbuff=0;
    if (khancap == 1){
    bien=0;
    answer=0;
    if (sdt_new[0]=='0'){
     delay_nhan(1000);
    sprintf(aux_string,"ATD%s;",sdt_new);
    do { if (bien%2500==0){answer = sendAT(aux_string,"OK",500);}bien=bien+1;delay_nhan(10);if(bien>5000){answer=1;break;}} while(answer==0); 
    end_cc=end_cuocgoi();
    if (end_cc) {sprintf(aux_string,"%s - Return",sdt_new);SetVariHC("SIM_CALL",aux_string);return;} 
    }
    }
    SetVariHC("SIM_CALL","NO ANSWER");
#endif
} 
/*void goidt2() {  
#ifdef USING_SIM
  int bien=0;
  answer=0;
  if (WiFiConf.sta_SDT1[0]!='x'){
    delay(500);
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
#endif
} */
//////////////////////////////////////////////
/////////Hàm nap tien  //////////////////////
////////////////////////////////////////////
void kttk(String nd) {
#ifdef USING_SIM
 nd="AT+CUSD=1,\""+nd+"\"";
 Serial.println(nd);
 xbuff=0;
#endif
}
//////////////////////////////////////////////
/////////Hàm KTTK  //////////////////////////
////////////////////////////////////////////
void kttkcusd() {
#ifdef USING_SIM
        int bien=0;
              sprintf(aux_string,"AT+CUSD=1,\"%s\"",WiFiConf.sta_makttk);
              do { answer = sendAT(aux_string,"OK",200);bien=bien+1;delay_nhan(2000); if(bien>5){answer=1;}} while(answer==0); 
#endif
}


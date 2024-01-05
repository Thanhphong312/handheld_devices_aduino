#include <ArduinoJson.h>
#include "Esp8266.h"
#include "Kpad.h"
#include "TFT.h"
#include "SDCard.h"
#include "RTCDS3231.h"
#include "SensorFactory.h"
#include "Sirens.h"
#include "Battery.h"
#include <string>
extern unsigned short iconwifi[256];
extern unsigned short iconpassword[256];
extern unsigned short stwifi1[2500];
extern unsigned short stwifi2[2500];
extern unsigned short connectwifi[0x8F];
extern unsigned short noconnectwifi[0x8F];
extern unsigned short upArrow[0x100];
extern unsigned short deleteArrow[0x100];
extern unsigned short changeNumber[0x100];
extern unsigned short rtArrow[100];
extern unsigned short sensor_temp[1024];
extern unsigned short sensor_do[1024];
extern unsigned short sensor_nh4[1024];
extern unsigned short sensor_salt[1024];
extern unsigned short sensor_ph[1024];


extern uint8_t BigFont[];
extern uint8_t SmallFont[];
extern uint8_t TinyFont[];
extern uint8_t iconshort[];
extern uint8_t iconoperation_16x16[];
extern uint8_t BigFont_DK_NO[];
extern uint8_t SevenSegNumFontPlusPlusMinus[];



class Board{
  private:
    KPad mykeypad;
    Esp8266 esp8266;
    TFT mylcd;
    SDCard mysd;
    RTCDS3231 myrtc;
    SensorFactory sensorfactory;
    Sirens sirens;
    Battery battery;
    String configusernamewifi[3] = {"","selected","unsuccess"};
    String configpasswordwifi[3] = {"","unselected","unsuccess"};  
    String confighosthttp[3] = {"","selected","unsuccess"};
    String configporthttp[3] = {"","unselected","unsuccess"};
    String configbrokermqtt[3] = {"","selected","unsuccess"};
    String configportmqtt[3] = {"","unselected","unsuccess"};
    String configusernamemqtt[3] = {"","unselected","unsuccess"};
    String configpasswordmqtt[3] = {"","unselected","unsuccess"};
    String configslopePH[3] = {"","selected","unsuccess"};
    String configinterceptPH[3] = {"","unselected","unsuccess"};
    String configslopeSAL[3] = {"","selected","unsuccess"};
    String configinterceptSAL[3] = {"","unselected","unsuccess"};
    String tempConfigusernamewifi = "";
    String tempConfigpasswordwifi = "";  
    String tempConfighosthttp = "";
    String tempConfigporthttp = "";
    String tempConfigbrokermqtt = "";
    String tempConfigportmqtt = "";
    String tempConfigusernamemqtt = "";
    String tempConfigpasswordmqtt = "";
    int percentbattery = 0;
    char timeString[30] = "Wed, 06 Dec 2023 10:15:00 GMT";
  public:
    Board(){
      mykeypad.setKeypad();
      esp8266.setESP();
      mylcd.setTFT();
      mysd.setSDCard();
      myrtc.setRTC();
      percentbattery = battery.getBattery();
      uint8_t *font[] = {
          BigFont,
          SmallFont,
          TinyFont,
          iconshort,
          BigFont_DK_NO,
          iconoperation_16x16,
          SevenSegNumFontPlusPlusMinus
        };
        mylcd.setFont(sizeof(font) / sizeof(font[0]) ,font);
        unsigned short *icon[] = {
          iconwifi,
          iconpassword,
          connectwifi,
          noconnectwifi,
          upArrow,
          deleteArrow,
          changeNumber,
          rtArrow,
          stwifi1,
          stwifi2,
          sensor_temp,
          sensor_ph,
          sensor_salt,
          sensor_nh4,
          sensor_do
        };
        mylcd.setIcon(sizeof(icon) / sizeof(icon[0]), icon);
        mylcd.setPage();
        mylcd.drawStatus(percentbattery);
        mylcd.drawIndex("index"); 
        // mylcd.drawIndex("history"); 
        // String history = mysd.getHistorySensor("NH4");
        // mylcd.drawHistory(0,0,history);
        myrtc.setArrayTime();
        Serial.println(myrtc.getRTCTimeStr());
        this->configTime(myrtc.getHour(),myrtc.getMinute(),myrtc.getSecond(),myrtc.getDay(),myrtc.getMonth(),myrtc.getYear());
        String cfwifi = mysd.getConfig();
        Serial.println(cfwifi);

        if(cfwifi!=""){
          Serial.println(cfwifi);

          // Calculate the capacity for the JSON data
          const size_t capacity = cfwifi.length() + JSON_OBJECT_SIZE(10) + JSON_ARRAY_SIZE(1);
          DynamicJsonDocument doc(capacity);

          // Parse the JSON data
          DeserializationError error = deserializeJson(doc, cfwifi);

          if (error) {
            Serial.print("JSON parsing failed: ");
            Serial.println(error.c_str());
            return;
          }

          // Now you can work with the parsed JSON array
          if (doc.is<JsonObject>()) {
            Serial.println("Json parsing done");
            JsonObject objConfig = doc.as<JsonObject>();
            const char* usernamewifi = "";
            if (objConfig.containsKey("usernamewifi")) {
              usernamewifi = objConfig["usernamewifi"];
            }
            const char* passwordwifi = "";
            if (objConfig.containsKey("passwordwifi")) {
              passwordwifi = objConfig["passwordwifi"];
            }
            const char* hosthttp = "";
            if (objConfig.containsKey("hosthttp")) {
              hosthttp = objConfig["hosthttp"];
            }
            const char* porthttp = "";
            if (objConfig.containsKey("porthttp")) {
              porthttp = objConfig["porthttp"];
            }
            const char* brokermqtt = "";
            if (objConfig.containsKey("brokermqtt")) {
              brokermqtt = objConfig["brokermqtt"];
            }
            const char* portmqtt = "";
            if (objConfig.containsKey("portmqtt")) {
              portmqtt = objConfig["portmqtt"];
            }
            const char* usernamemqtt = "";
            if (objConfig.containsKey("usernamemqtt")) {
              usernamemqtt = objConfig["usernamemqtt"];
            }
            const char* passwordmqtt = "";
            if (objConfig.containsKey("passwordmqtt")) {
              passwordmqtt = objConfig["passwordmqtt"];
            }
            this->tempConfigusernamewifi = usernamewifi;
            this->tempConfigpasswordwifi = passwordwifi;
            this->tempConfighosthttp = hosthttp;
            this->tempConfigporthttp = porthttp;
            this->tempConfigbrokermqtt = brokermqtt;
            this->tempConfigportmqtt = portmqtt;
            this->tempConfigusernamemqtt = usernamemqtt;
            this->tempConfigpasswordmqtt = passwordmqtt;
            this->configJson();
            this->esp8266.setConfigWifi(this->tempConfigusernamewifi.c_str(), this->tempConfigpasswordwifi.c_str());
            doc.clear();
          }
        }else{
          this->esp8266.setConfigWifi(this->esp8266.getSsid().c_str(), this->esp8266.getPass().c_str());
        }
        this->getCalibrations();
        // Serial.println("Hour");
        // Serial.println(myrtc.getHour());
        // Serial.println("Second");
        // Serial.println(myrtc.getSecond());
        // Serial.println("Minute");
        // Serial.println(myrtc.getMinute());
        // Serial.println("Day");
        // Serial.println(myrtc.getDay());
        // Serial.println("Month");
        // Serial.println(myrtc.getMonth());
        // Serial.println("Year");
        // Serial.println(myrtc.getYear());
        // mylcd.configTime();
        // mylcd.drawConnectWifi();

    }
    ~Board(){

    }
    void startStatus(){
      mylcd.drawStatus(percentbattery);
    }
    void start(){
      // Serial.println("getttt");
      if(mylcd.getStatus()=="change"){
        // Serial.println("-----------");
        // Serial.println(mylcd.getpage("index"));
        // Serial.println(mylcd.getpage("configwifi"));
        // Serial.println(mylcd.getpage("start"));
        // Serial.println(mylcd.getpage("history"));
        // Serial.println(mylcd.getpage("calibrations"));
        // Serial.println(mylcd.getpage("connectwifi"));
        // Serial.println(mylcd.getpage("detailhistory"));
        // Serial.println(mylcd.getpage("detailpost"));
        // Serial.println(mylcd.getpage("confighttp"));
        // Serial.println(mylcd.getpage("configmqtt"));
        // Serial.println(mylcd.getpage("chooseconfig"));
        // Serial.println(mylcd.getpage("connecthttp"));
        // Serial.println(mylcd.getpage("connectmqtt"));
        // Serial.println(mylcd.getpage("detailCalibrations"));
        // Serial.println(mylcd.getpage("configtime"));
        // Serial.println(mylcd.getpage("postServer"));       
        // Serial.println(mylcd.getStatus());
        // Serial.println("-----------"); 
      }
      if(mylcd.getpage("index")=="show"){
        if(mylcd.getStatus()=="change"){
          Serial.println("Change");
          mylcd.drawStatus(percentbattery);
          mylcd.fillScreen("black");
          mylcd.drawIndex("index");
          mylcd.setStatus("no change");
        }
        if(mylcd.getdataAvailable()){
          Serial.println("data");
          int x = mylcd.getXTouch(); 
          int y = mylcd.getYTouch();
          if ((x != -1 && x != -1)){
            mylcd.checkindex(x, y);
            // if()
            Serial.println("-----------");
            Serial.println("show index");
            Serial.println("-----------");
          }
        }
      }else if(mylcd.getpage("configwifi")=="show"){
        char key = this->getKeyPad();
        // Serial.println(key);
        if(key=='U'||key=='D'||key=='L'||key=='R'||key=='*'||key=='#'||key=='S'){
          Serial.println(key);
          mylcd.setStatus("no change");
          mylcd.drawKeyboardWifi(key, configusernamewifi, configpasswordwifi);
        }
        if(mylcd.getStatus()=="change"){
          if(this->getShowKeyBoard()==1){
            Serial.println("showwwwww");
            mylcd.setStatus("no change");
            this->setShowKeyBoard(0);
            this->configJson();
            mylcd.fillScreen("white");
            this->configusernamewifi[1] = "selected";
            this->configusernamewifi[2] = "unsuccess";
            this->configpasswordwifi[1] = "unselected";
            this->configpasswordwifi[2] = "unsuccess";
            mylcd.drawKeyboardWifi(0, configusernamewifi, configpasswordwifi);
            mylcd.drawIndex("configwifi");
          }
        } 
        if(mylcd.getChangeConfigWifi()==1){
          mylcd.setChangeConfigWifi(0);
          mylcd.drawConfigWifi(configusernamewifi, configpasswordwifi);
        }
        if(mylcd.getdataAvailable()){
          int x = mylcd.getXTouch(); 
          int y = mylcd.getYTouch();
          if ((x != -1 && x != -1)){
            mylcd.drawKeyboardWifi(0, configusernamewifi, configpasswordwifi, x, y);
            // if()
            Serial.println("-----------");
            Serial.println(x);
            Serial.println(" - ");
            Serial.println(y);
            Serial.println("-----------");
          }
        }
            // mylcd.setStatus("no change");
      }else if(mylcd.getpage("confighttp")=="show"){
        char key = this->getKeyPad();
        // Serial.println(key);
        if(key=='U'||key=='D'||key=='L'||key=='R'||key=='*'||key=='#'||key=='S'){
          Serial.println(key);
          mylcd.setStatus("no change");
          mylcd.drawKeyboardHttp(key, confighosthttp, configporthttp);

        }
        if(mylcd.getStatus()=="change"){
          if(this->getShowKeyBoard()==1){
            Serial.println("showwwwww");
            mylcd.setStatus("no change");
            this->setShowKeyBoard(0);
            mylcd.fillScreen("white");
            this->confighosthttp[1] = "selected";
            this->confighosthttp[2] = "unsuccess";
            this->configporthttp[1] = "unselected";
            this->configporthttp[2] = "unsuccess";
            mylcd.drawKeyboardHttp(0, confighosthttp, configporthttp);
            mylcd.drawIndex("confighttp");
          }
        } 
        if(mylcd.getChangeConfigWifi()==1){
          mylcd.setChangeConfigWifi(0);
          mylcd.drawConfigServerHttp(confighosthttp, configporthttp);
        }
        if(mylcd.getdataAvailable()){
          int x = mylcd.getXTouch(); 
          int y = mylcd.getYTouch();
          if ((x != -1 && x != -1)){
            mylcd.drawKeyboardHttp(0, configusernamewifi, configpasswordwifi, x, y);
            // if()
            Serial.println("-----------");
            Serial.println(x);
            Serial.println(" - ");
            Serial.println(y);
            Serial.println("-----------");
          }
        }
        // mylcd.setStatus("no change");
      }else if(mylcd.getpage("configmqtt")=="show"){
        char key = this->getKeyPad();
        // Serial.println(key);
        if(key=='U'||key=='D'||key=='L'||key=='R'||key=='*'||key=='#'||key=='S'){
          Serial.println(key);
          mylcd.setStatus("no change");
          mylcd.drawKeyboardMqtt(key, configbrokermqtt, configportmqtt, configusernamemqtt, configpasswordmqtt);

        }
        if(mylcd.getStatus()=="change"){
          if(this->getShowKeyBoard()==1){
            Serial.println("showwwwww");
            mylcd.setStatus("no change");
            this->setShowKeyBoard(0);
            mylcd.fillScreen("white");
            this->configbrokermqtt[1] = "selected";
            this->configbrokermqtt[2] = "unsuccess";
            this->configportmqtt[1] = "unselected";
            this->configportmqtt[2] = "unsuccess";
            this->configusernamemqtt[1] = "unselected";
            this->configusernamemqtt[2] = "unsuccess";
            this->configpasswordmqtt[1] = "unselected";
            this->configpasswordmqtt[2] = "unsuccess";
            mylcd.drawKeyboardMqtt(0, configbrokermqtt, configportmqtt, configusernamemqtt, configpasswordmqtt);
            mylcd.drawIndex("configmqtt");
          }
        } 
        if(mylcd.getChangeConfigWifi()==1){
          mylcd.setChangeConfigWifi(0);
          mylcd.drawConfigServerMqtt(configbrokermqtt, configportmqtt, configusernamemqtt, configpasswordmqtt);
        }
        if(mylcd.getdataAvailable()){
          int x = mylcd.getXTouch(); 
          int y = mylcd.getYTouch();
          if ((x != -1 && x != -1)){
            mylcd.drawKeyboardMqtt(0, configbrokermqtt, configportmqtt, configusernamemqtt, configpasswordmqtt, x, y);
            // if()
            Serial.println("-----------");
            Serial.println(x);
            Serial.println(" - ");
            Serial.println(y);
            Serial.println("-----------");
          }
        }
            // mylcd.setStatus("no change");
      }else if(mylcd.getpage("configtime")=="show"){
        if(mylcd.getStatus()=="change"){
            Serial.println("showwwwww");
            mylcd.drawStatus(percentbattery);
            mylcd.setStatus("no change");
            mylcd.fillScreen("black");
            mylcd.drawIndex("configtime");
            int response = this->configTimeFromApi();
            Serial.println(response);
            mylcd.drawConfigTime(response);
        } 
        // if(mylcd.getChangeConfigWifi()==1){
        //   mylcd.setChangeConfigWifi(0);
        //   mylcd.drawConfigWifi(configusernamewifi, configpasswordwifi);
        // }
      }else if(mylcd.getpage("chooseconfig")=="show"){
        if(mylcd.getStatus()=="change"){
          Serial.println("Change");
          mylcd.drawStatus(percentbattery);
          mylcd.fillScreen("black");
          this->configJson();
          mylcd.drawIndex("chooseconfig");
          mylcd.setStatus("no change");
        }
        if(mylcd.getdataAvailable()){
          int x = mylcd.getXTouch(); 
          int y = mylcd.getYTouch();
          if ((x != -1 && x != -1)){
            mylcd.checkChooseConfig(x, y);
            // if()
            Serial.println("-----------");
            Serial.println("show index");
            Serial.println("-----------");
          }
        }
            // mylcd.setStatus("no change");
      }else if(mylcd.getpage("connectwifi")=="show"){
        if(mylcd.getStatus()=="change"){
          mylcd.setStatus("no change");
          mylcd.fillScreen("black");
          mylcd.drawStatus(percentbattery);
          mylcd.drawIndex("connectwifi");
          this->setConfigJson("wifi");
          this->esp8266.setStatus("disconnect");
          this->mylcd.setCheckConnectWifi(0);
          Serial.println(this->configusernamewifi[0]);
          Serial.println(this->configpasswordwifi[0]);
          mylcd.drawStatusConnectWifi("before");
          this->connectWifiEsp8266();
          mylcd.drawStatusConnectWifi("after");
        }
      }else if(mylcd.getpage("history")=="show"){
        if(mylcd.getStatus()=="change"){
          Serial.println("Change");
          mylcd.drawStatus(100);
          mylcd.setStatus("no change");
          mylcd.drawIndex("history"); 
        }
        if(mylcd.getdataAvailable()){
          int x = mylcd.getXTouch(); 
          int y = mylcd.getYTouch();
          if ((x != -1 && x != -1)){
            mylcd.checkChooseHistory(x,y);   
            Serial.print("----------------------");
            Serial.println(x);
            Serial.print(y);
            Serial.print("----------------------");  
          }
        }
      }else if(mylcd.getpage("detailhistory")=="show"){
        if(mylcd.getStatus()=="change"){
          Serial.println("Change");
          mylcd.drawStatus(percentbattery);
          mylcd.fillScreen("black");
          mylcd.setStatus("no change");
          String namefile = mylcd.getNameFileHistory() + ".txt";
          mylcd.setLengthScoll(mysd.lengthHistory(namefile));
          Serial.println("mysd.lengthHistory(namefile)");
          Serial.println(mysd.lengthHistory(namefile));
          mylcd.drawIndex("detailhistory"); 
        }
        if(mylcd.getChangedetailhistory()==1){
          mylcd.setChangedetailhistory(0);
          String namefile = mylcd.getNameFileHistory() + ".txt";         
          if(mysd.lengthHistory(namefile)>0){
            mylcd.drawUpDown();
            int start = mylcd.getStartHis();
            int end = mylcd.getEndHis();
            if(mysd.lengthHistory(namefile)<end){
              end = mysd.lengthHistory(namefile);
            }
            Serial.println(start);
            Serial.println(end);
            int widthline = 0;
            for (int i = start; i < end; i++) {
              int linex1 = 70+widthline;
              int linex2 = 70+widthline+20;
              String history = mysd.getHistorySensor(namefile, i);
              Serial.println("history");
              Serial.println(history);
              
              int fileid;
              static char value[10], datetime[20], status[2];
              sscanf(history.c_str(), "{\"id\":%d,\"value\":\"%9[^\"]\",\"datetime\":\"%19[^\"]\",\"status\":\"%1[^\"]\"}", &fileid, value, datetime, status);
                // String id = String(fileid);
              mylcd.drawHistory(String(fileid).c_str(),String(value),String(datetime),String(status), linex1, linex2);
              Serial.print("ID: ");
              Serial.println(fileid);
              Serial.print("Value: ");
              Serial.println(String(value));
              Serial.print("Datetime: ");
              Serial.println(datetime);
              Serial.print("Status: ");
              Serial.println(status);
              widthline+=20;
            }
             Serial.println("-------------");
              Serial.println(mylcd.getLenChoosePost());
              for(int i=0;i<mylcd.getLenChoosePost();i++){
                if (mylcd.getlistchooseidpost()[i] != nullptr) {
                  Serial.print(mylcd.getlistchooseidpost()[i]);
                  Serial.print(" ");
                } else {
                  Serial.print("null ");
                }
              }
              Serial.println("-------------");
          }
        }
        if(mylcd.getdataAvailable()){
          int x = mylcd.getXTouch(); 
          int y = mylcd.getYTouch();
          if ((x != -1 && x != -1)){
            mylcd.CheckChooseSelect(x, y);
            Serial.println("------");
            Serial.println(x);
            Serial.println(y);
            Serial.println("------");
          }
        }
      }else if(mylcd.getpage("detailpost")=="show"){
        if(mylcd.getStatus()=="change"){
          Serial.println("Change");
          mylcd.drawStatus(percentbattery);
          mylcd.fillScreen("black");
          mylcd.setStatus("no change");
          mylcd.drawIndex("detailpost"); 
          Serial.println(mylcd.getChangedetailposthistory());
        }
        if(mylcd.getChangedetailposthistory()==1){
          mylcd.setChangedetailposthistory(0);

          String namefile = mylcd.getNameFileHistory() + ".txt";
          char** listpostsv = mylcd.getlistchooseidpost();
          int lenlistpostsv = mylcd.getLenChoosePost();
          Serial.println("lenlistpostsv");
          Serial.println(lenlistpostsv);
          Serial.println(mylcd.getEndHisPost());
          if(lenlistpostsv>0){
            if(lenlistpostsv<mylcd.getEndHisPost()){
              mylcd.setEndHisPost(lenlistpostsv);
            }
            int start = mylcd.getStartHisPost();
            int end = mylcd.getEndHisPost();
            int widthline = 0;
            int linex1 = 0;
            int linex2 = 0;
            for(int i= start ; i < end; i++){
              linex1 = 70+widthline;
              linex2 = 70+widthline+20;
              Serial.println("listpostsv[i]");
              Serial.println(listpostsv[i]);
              int id = atoi(listpostsv[i]);
              Serial.println(id);
              Serial.println("id");
              Serial.println("namefile");
              Serial.println(namefile);
              String history = mysd.getHistoryByID(namefile, id);
              Serial.println(history);
              int fileid;
              static char value[10], datetime[20], status[2];
              sscanf(history.c_str(), "{\"id\":%d,\"value\":\"%9[^\"]\",\"datetime\":\"%19[^\"]\",\"status\":\"%1[^\"]\"}", &fileid, value, datetime, status);
                // String id = String(fileid);
                mylcd.drawPostServer(String(id),String(value),String(datetime),String(status), linex1, linex2);
                Serial.print("ID: ");
                Serial.println(id);
                Serial.print("Value: ");
                Serial.println(value);
                Serial.print("Datetime: ");
                Serial.println(datetime);
                Serial.print("Status: ");
                Serial.println(status);
              widthline+=20;
            }
          }
        }
        if(mylcd.getdataAvailable()){
          int x = mylcd.getXTouch(); 
          int y = mylcd.getYTouch();
          if ((x != -1 && x != -1)){
            mylcd.checkDrawPostServer(x, y);
          }
        }
      }else if(mylcd.getpage("connecthttp")=="show"){
        if(mylcd.getStatus()=="change"){
          mylcd.setStatus("no change");
          mylcd.fillScreen("black");
          mylcd.drawStatus(percentbattery);
          mylcd.drawIndex("connecthttp");
          this->setConfigJson("http");
          int res = this->connectWifiProtocol("HTTP");
          delay(2000);
          mylcd.drawStatusConfigHttp(res);
          mylcd.clearScreen();

        }
      }else if(mylcd.getpage("connectmqtt")=="show"){
        if(mylcd.getStatus()=="change"){
          mylcd.setStatus("no change");
          mylcd.fillScreen("black");
          mylcd.drawStatus(percentbattery);
          mylcd.drawIndex("connectmqtt");
          this->setConfigJson("mqtt");
          int res = this->connectWifiProtocol("MQTT");
          // delay(2000);
          mylcd.drawStatusConfigMqtt(res);
        }
      }else if(mylcd.getpage("calibrations")=="show"){
        if(mylcd.getStatus()=="change"){
          mylcd.setStatus("no change");
          mylcd.fillScreen("black");
          mylcd.drawStatus(percentbattery);
          mylcd.drawIndex("calibrations");
        }
        if(mylcd.getdataAvailable()){
          int x = mylcd.getXTouch(); 
          int y = mylcd.getYTouch();
          if ((x != -1 && x != -1)){
            mylcd.checkChooseCalibrations(x, y);
          }
        }
      }else if(mylcd.getpage("detailCalibrations")=="show"){
        if(mylcd.getStatus()=="change"){
          mylcd.setStatus("no change");
          mylcd.fillScreen("black");
          mylcd.drawStatus(percentbattery);
          mylcd.drawIndex("detailCalibrations");
          this->configslopePH[1] = "selected";
          this->configslopePH[2] = "unsuccess";
          this->configinterceptPH[1] = "unselected";
          this->configinterceptPH[2] = "unsuccess";
          this->configslopeSAL[1] = "selected";
          this->configslopeSAL[2] = "unsuccess";
          this->configinterceptSAL[1] = "unselected";
          this->configinterceptSAL[2] = "unsuccess";
          if(mylcd.getPageCalibrations()=="PH"){
            mylcd.drawKeyboardCalibrations(0, configslopePH, configinterceptPH);
          }else{
            mylcd.drawKeyboardCalibrations(0, configslopeSAL, configinterceptSAL);
          }
        }
        if(mylcd.getShowPageCalibrations()==1){
          mylcd.setShowPageCalibrations(0);
          if(mylcd.getPageCalibrations()=="PH"){
            mylcd.drawConfigpageCalibrations(configslopePH, configinterceptPH);
          }else{
            mylcd.drawConfigpageCalibrations(configslopeSAL, configinterceptSAL);
          }
        }
        char key = this->getKeyPad();
        // Serial.println(key);
        if(key=='U'||key=='D'||key=='L'||key=='R'||key=='*'||key=='#'||key=='S'){
          Serial.println(key);
          mylcd.setStatus("no change");
          if(mylcd.getPageCalibrations()=="PH"){
            mylcd.drawKeyboardCalibrations(key, configslopePH, configinterceptPH);
          }else{
            mylcd.drawKeyboardCalibrations(key, configslopeSAL, configinterceptSAL);
          }
        }
        if(mylcd.getdataAvailable()){
          int x = mylcd.getXTouch(); 
          int y = mylcd.getYTouch();
          if ((x != -1 && x != -1)){
            if(mylcd.getPageCalibrations()=="PH"){
              mylcd.drawKeyboardCalibrations(0, configslopePH, configinterceptPH, x, y);
            }else{
              mylcd.drawKeyboardCalibrations(0, configslopeSAL, configinterceptSAL, x, y);
            }            
            Serial.println("-----------");
            Serial.println("show index");
            Serial.println("-----------");
          }
        }
      }else if(mylcd.getpage("configCalibrations")=="show"){
        if(mylcd.getStatus()=="change"){
          mylcd.setStatus("no change");
          mylcd.fillScreen("black");
          mylcd.drawStatus(percentbattery);
          mylcd.drawIndex("configCalibrations");
          int res = this->setCalibrations();
          mylcd.drawStatusConfigCalibrations(res);
        }
      }else if(mylcd.getpage("start")=="show"){
        if(mylcd.getStatus()=="change"){
            Serial.println("showwwwww");
            mylcd.setStatus("no change");
            mylcd.fillScreen("white");
            mylcd.setStatusSensorMeasuring("stop");
            sensorfactory.setSensorFactory(A0, A1);
            mylcd.drawIndex("start");
        } 

        if(mylcd.getStatusSensorMeasuring()=="start"){
          Serial.println("mylcd.getSirens()");

          sensorfactory.checkSensor(mylcd.getSensorMeasuring(), this->configslopePH[0].toDouble(), configinterceptPH[0].toFloat(), configslopeSAL[0].toFloat(), configinterceptSAL[0].toFloat());

          Serial.println("mylcd.getSensorMeasuring()");
          Serial.println(mylcd.getSensorMeasuring());
            float voltageID = sensorfactory.getVoltageID(); // Fetch the voltage ID once

            Serial.print("Voltage ID: ");
            Serial.println(voltageID, 20);
            float maxVoltageID = 4.80;
            bool isGreaterThan5 = (voltageID<maxVoltageID); // Compare the fetched voltage ID with 5.0

            Serial.print("Is voltage ID greater than 5? ");
            Serial.println(isGreaterThan5);
            if(isGreaterThan5){
              if(mylcd.getSirens()==1){
                Serial.println("reoooooooooooooooooooooooo");
                Serial.println(sensorfactory.getVoltageID());
                sirens.setPin(12);
                sirens.ring();
                mylcd.setSirens(0);
                delay(200);
                Serial.println("stoppppppppppppppppp");
                sirens.stop();
              }
            }else{
              mylcd.setSirens(1);
            }
          String name = sensorfactory.getSensorName();
          String unit = sensorfactory.getDonVi();
          float value = sensorfactory.get();
          mylcd.drawMeasuring(name, unit ,String(value));
        }
        if(mylcd.getStartPost()==1){
          mylcd.setStartPost(0);
          mylcd.drawStatusPostSaveMeasuring(0, "P");
          String shortname = sensorfactory.getShortName();
          shortname.trim();
          int dataStreamId = "";
          if(shortname=="TEMP"){
            dataStreamId = 1;
          }else if(shortname=="PH"){
            dataStreamId = 2;
          }else if(shortname=="SAL"){
            dataStreamId = 3;
          }
          Serial.println("shortname");
          Serial.println(shortname);
          float value = mylcd.getValueMeasuring();
          char valueStr[10]; // Adjust the size according to your value
          dtostrf(value, sizeof(value), 2, valueStr);
          Serial.println(valueStr);
          myrtc.setArrayTime();
          int year = myrtc.getYear();
          int month = myrtc.getMonth();
          int day = myrtc.getDay();
          int hour = myrtc.getHour();
          int second = myrtc.getSecond();
          int minute = myrtc.getMinute();
          char datapost[85]; 
          sprintf(datapost, "{\"dataStreamId\": %d,\"result\": \"%s\",\"resultTime\": \"%04d-%02d-%02dT%02d:%02d:%02d\"}",dataStreamId, valueStr, year, month, day, hour, minute, second);

          Serial.println(datapost);
          int response = this->postServer("HTTP", datapost);
          Serial.println("response");
          Serial.println(response);
          if(response==1){
            mylcd.drawStatusPostSaveMeasuring(1);
          }else{
            mylcd.drawStatusPostSaveMeasuring(2);
          }
          delay(1000);
          mylcd.drawStatusPostSaveMeasuring(0);
        }
        if(mylcd.getStartSave()==1){
          mylcd.setStartSave(0);
          mylcd.drawStatusPostSaveMeasuring(0, "S");
          String shortname = sensorfactory.getShortName();
          shortname.trim();
          String namefile = shortname+".txt";
          float value = mylcd.getValueMeasuring();
          myrtc.setArrayTime();
          int year = myrtc.getYear();
          int month = myrtc.getMonth();
          int day = myrtc.getDay();
          int hour = myrtc.getHour();
          int second = myrtc.getSecond();
          int minute = myrtc.getMinute();
          char datetime[19]; 
          sprintf(datetime, "%04d-%02d-%02dT%02d:%02d:%02d", year, month, day, hour, minute, second);

          // String datetime = "2023-11-30T22:46:00";
          int id = random(1, 10)*random(1, 10)*random(1, 9)+second;
          Serial.println("id");
          Serial.println(id);
          Serial.println("namefile");
          Serial.println(namefile);
          Serial.println("value");
          Serial.println(String(value));
          int response = mysd.setHistorySensor(namefile, String(id), String(value), String(datetime), "0");
          if(response==1){
            mylcd.drawStatusPostSaveMeasuring(1);
          }else{
            mylcd.drawStatusPostSaveMeasuring(2);
          }
          delay(1000);
          mylcd.drawStatusPostSaveMeasuring(0);
        }
        if(mylcd.getdataAvailable()){
          int x = mylcd.getXTouch(); 
          int y = mylcd.getYTouch();
          if ((x != -1 && x != -1)){
            mylcd.drawChooseMeasuring(x, y);
            Serial.println("-----------");
            Serial.println(x);
            Serial.println(" - ");
            Serial.println(y);
            Serial.println("-----------");
          }
        }
      }else if(mylcd.getpage("postServer")=="show"){
        if(mylcd.getStatus()=="change"){
          Serial.println("Change");
          mylcd.drawStatus(percentbattery);
          mylcd.fillScreen("black");
          mylcd.drawIndex("postServer");
          mylcd.setStatus("no change");
          Serial.println(mylcd.getChooseMethod());
          String namefile = mylcd.getNameFileHistory() + ".txt";
          char** listpostsv = mylcd.getlistchooseidpost();
          int lenlistpostsv = mylcd.getLenChoosePost();
          Serial.println("lenlistpostsv");
          Serial.println(lenlistpostsv);
          int hightLine = 0;
          for(int i=0; i <lenlistpostsv; i++){
            int id = atoi(listpostsv[i]);
            String history = mysd.getHistoryByID(namefile, id);
            Serial.println(history);
            int fileid;
            static char value[10], datetime[20], status[2];
            sscanf(history.c_str(), "{\"id\":%d,\"value\":\"%9[^\"]\",\"datetime\":\"%19[^\"]\",\"status\":\"%1[^\"]\"}", &fileid, value, datetime, status);
            String shortname = mylcd.getNameFileHistory();
            String dataStreamId = "";
            if(shortname=="TEMP"){
              dataStreamId = 1;
            }else if(shortname=="PH"){
              dataStreamId = 2;
            }else if(shortname=="SAL"){
              dataStreamId = 3;
            }
            String result = String(value);
            String resultTime = String(datetime);
            String data = "{\"dataStreamId\": "+dataStreamId+",\"result\": \""+result+"\",\"resultTime\": \""+resultTime+"\"}";
            Serial.println("data");
            Serial.println(data);
            int response = this->postServer(mylcd.getChooseMethod(), data);
            Serial.println(response);
            String statuscheck = "done";
            if (response) {
                statuscheck =  "done";
                // delay(1000);
                // this->mysd.changeStatus(namefile, id, "1");
                delay(1000);
            } else {
                statuscheck = "fail";
            }
            mylcd.drawStatusPostServer(result, resultTime, statuscheck, hightLine);
            hightLine+=20;
          }
          delay(2000);
          mylcd.succesPostServer();
          // Serial.println("hihi");
          //response:New observation with dataStreamId 2 was added
        }
      }        
    }
    int postServer(String ptc , String data){
      this->esp8266.setConfigWifi(tempConfigusernamewifi.c_str(),tempConfigpasswordwifi.c_str());
      Serial.println(tempConfigusernamewifi);
      Serial.println(tempConfigpasswordwifi);
      Serial.println(tempConfighosthttp);
      Serial.println(tempConfigporthttp);
      int request = 0;
      if(this->esp8266.connectWifi()){
        if(ptc=="HTTP"){
          Serial.println(data);
          this->esp8266.setHTTP(tempConfighosthttp.c_str(), tempConfigporthttp.toInt());
          Serial.println("-----------------------");
          // Serial.println(this->esp8266->connectWifi());
          char* path = "/ctu/geo/observations";
          request = this->esp8266.post(path, data.c_str());
          Serial.println(request);
          Serial.println("-----------------------");
        }else if(ptc=="MQTT"){
          Serial.println("mqtt");
          this->esp8266.setMQTT(this->configbrokermqtt[0].c_str(), this->configportmqtt[0].toInt(), this->configusernamemqtt[0].c_str(), this->configpasswordmqtt[0].c_str());
          // delay(4000);
          Serial.println("-----------------------");
          // response = this->esp8266.connectMQTT();

          Serial.println(request);
          Serial.println("-----------------------");
        }
      }        
      return request;    // Parse the history string into a JsonObject  
    }
    bool checkConnectWifi(){
     return this->esp8266.getStatus();
    }
    char getKeyPad(){
       return this->mykeypad.getKeyPad();
    }
    int getShowKeyBoard(){
      return this->mylcd.getShow();
    }
    void setShowKeyBoard(int value){
      this->mylcd.setShow(value);
    }
    void connectWifiEsp8266(){
      // this->esp8266.setDebug(true);
      this->mysd.setConfig(this->configusernamewifi[0],this->configpasswordwifi[0], this->confighosthttp[0], this->configporthttp[0], this->configbrokermqtt[0], this->configportmqtt[0],configusernamemqtt[0],this->configpasswordmqtt[0]);
      this->esp8266.setConfigWifi(this->configusernamewifi[0].c_str(),this->configpasswordwifi[0].c_str());
      Serial.println(this->esp8266.getSsid());
      Serial.println(this->esp8266.getPass());
      
      Serial.println("-----------------------");
      // Serial.println(this->esp8266->connectWifi());
      int response = this->esp8266.connectWifi();
      Serial.println(response);
      Serial.println("-----------------------");
      
      if(response==1){  
        mylcd.setCheckConnectWifi(1);
      }else{
        mylcd.setCheckConnectWifi(0);
      }
    }
    int connectWifiProtocol(String ptc){
      this->esp8266.setConfigWifi(this->configusernamewifi[0].c_str(),this->configpasswordwifi[0].c_str());
      this->esp8266.connectWifi();
        int response = 0;
        this->mysd.setConfig(this->configusernamewifi[0], this->configpasswordwifi[0], this->confighosthttp[0], this->configporthttp[0], this->configbrokermqtt[0], this->configportmqtt[0],configusernamemqtt[0],this->configpasswordmqtt[0]);
        if(ptc=="HTTP"){
          Serial.println("http");
          this->esp8266.setHTTP(this->confighosthttp[0].c_str(), this->configporthttp[0].toInt());
          Serial.println(this->confighosthttp[0]);
          Serial.println(this->configporthttp[0].toInt());
          Serial.println("-----------------------");
          // Serial.println(this->esp8266->connectWifi());
          response = this->esp8266.connectHTTP();
          Serial.println(response);
          Serial.println("-----------------------");
        }else if(ptc=="MQTT"){
          Serial.println("mqtt");
          // this->esp8266.setMQTT(this->configbrokermqtt[0].c_str(), this->configportmqtt[0].toInt(), this->configusernamemqtt[0].c_str(), this->configpasswordmqtt[0].c_str());
          // Serial.println("-----------------------");
          // response = this->esp8266.connectMQTT();
          response = 1;
          Serial.println(response);
          Serial.println("-----------------------");
        }
        // delay(5000);
        return response;
      // }else{
      //   return 0;
      // } 
    }
    void configTime(int hour, int minute, int second, int day, int month, int year ){
      mylcd.setTimeOut(hour, minute, second, day, month, year);
    }
    int configTimeFromApi(){
      this->esp8266.setConfigWifi(this->tempConfigusernamewifi.c_str(),this->tempConfigpasswordwifi.c_str());
      Serial.println(this->esp8266.getSsid());
      Serial.println(this->esp8266.getPass());
      
      Serial.println("-----------------------");
      // Serial.println(this->esp8266->connectWifi());
      int response = this->esp8266.connectWifi();
      // Serial.println(response);
      Serial.println("-----------------------");
      
      if(response==1){  
        Serial.println("setup server");
        Serial.println("get api");
        // this->esp8266.setHTTP("timeapi.io", 80);
        this->esp8266.setHTTP("bookkkk.shop", 80);

        //"/v2.1/get-time-zone?key=9X1N2H0EY3ZJ&format=json&by=position&lat=10.7769&lng=106.7009"
        // /api/Time/current/zone?timeZone=Asia/Ho_Chi_Minh
        String responsetime = this->esp8266.get("/");
        Serial.println("responsetime");
        Serial.println(responsetime.c_str());
        char* dateStart = strstr(responsetime.c_str(), "Date: ");
        if (dateStart != NULL) {
          dateStart += 6; // Di chuyển con trỏ đến phần bắt đầu của thời gian (6 ký tự sau "Date: ")
          // char* dateEnd = strstr(dateStart, "\r\n"); // Tìm vị trí kết thúc của chuỗi thời gian
          if (1) {
            // *dateEnd = '\0'; // Kết thúc chuỗi tại vị trí kết thúc của thời gian
            // Serial.println("Date: ");
            // Serial.println(dateStart); // In ra thời gian từ con trỏ char*
            strcpy(this->timeString, dateStart);
          }
          Serial.println("timeString");
          Serial.println(timeString);
          int day, year, hour, minute, second;
          char month[4];
          char dayOfWeek[4];
          char timeZone[4];
          sscanf(timeString, "%3s, %2d %3s %4d %2d:%2d:%2d %3s", dayOfWeek, &day, month, &year, &hour, &minute, &second, timeZone);
          Serial.println(day);
          Serial.println(hour);
          Serial.println(minute);
          Serial.println(second);
          int monthNumber = getMonthNumber(month);
          char* getdayOfWeek = getDayofWeek(dayOfWeek);
          mylcd.setTimeOut(hour+7, minute, second, day, monthNumber, year);
          myrtc.setDefaultTime(getdayOfWeek, hour+7, minute, second, day, monthNumber, year);
          Serial.println(myrtc.getRTCTimeStr());
          Serial.println(myrtc.getRTCDateStr());
          return 1;
        }else{
          return 0;
        }
      }else{
        return 0;
      }
    }
    char* getDayofWeek(const char* dayOfWeek) {
      if (strcmp(dayOfWeek, "Sun") == 0) {
          return "SUNDAY";
      } else if (strcmp(dayOfWeek, "Mon") == 0) {
          return "MONDAY";
      } else if (strcmp(dayOfWeek, "Tue") == 0) {
          return "TUESDAY";
      } else if (strcmp(dayOfWeek, "Wed") == 0) {
          return "WEDNESDAY";
      } else if (strcmp(dayOfWeek, "Thu") == 0) {
          return "THURSDAY";
      } else if (strcmp(dayOfWeek, "Fri") == 0) {
          return "FRIDAY";
      } else if (strcmp(dayOfWeek, "Sat") == 0) {
          return "SATURDAY";
      }
      return -1; // Trả về -1 nếu không tìm thấy chuỗi phù hợp
    }
    int getMonthNumber(const char* monthAbbreviation) {
      const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
      for (int i = 0; i < 12; i++) {
        if (strcmp(monthAbbreviation, months[i]) == 0) {
          return i + 1; // Months are 1-based, so January is 1, February is 2, etc.
        }
      }
      return 0; // Invalid month abbreviation
    }
    void configJson(){
      this->configusernamewifi[0] = this->tempConfigusernamewifi;
      this->configpasswordwifi[0] = this->tempConfigpasswordwifi;
      this->confighosthttp[0] = this->tempConfighosthttp;
      this->configporthttp[0] = this->tempConfigporthttp;
      this->configbrokermqtt[0] = this->tempConfigbrokermqtt;
      this->configportmqtt[0] = this->tempConfigportmqtt;
      this->configusernamemqtt[0] = this->tempConfigusernamemqtt;
      this->configpasswordmqtt[0] = this->tempConfigpasswordmqtt;
    }
    void getCalibrations(){
      String calibrations = mysd.getCalibrations();
      Serial.println(calibrations);
      if(calibrations!=""){
          // Calculate the capacity for the JSON data
          const size_t capacity = calibrations.length() + JSON_OBJECT_SIZE(10) + JSON_ARRAY_SIZE(1);
          DynamicJsonDocument doc(capacity);

          // Parse the JSON data
          DeserializationError error = deserializeJson(doc, calibrations);

          if (error) {
            Serial.print("JSON parsing failed: ");
            Serial.println(error.c_str());
            return;
          }

          // Now you can work with the parsed JSON array
          if (doc.is<JsonObject>()) {
            Serial.println("Json parsing done");
            JsonObject objConfig = doc.as<JsonObject>();
            const char* slopePH = "";
            if (objConfig.containsKey("slopePH")) {
              slopePH = objConfig["slopePH"];
            }
            const char* interceptPH = "";
            if (objConfig.containsKey("interceptPH")) {
              interceptPH = objConfig["interceptPH"];
            }
            const char* slopeSAL = "";
            if (objConfig.containsKey("slopeSAL")) {
              slopeSAL = objConfig["slopeSAL"];
            }
            const char* interceptSAL = "";
            if (objConfig.containsKey("interceptSAL")) {
              interceptSAL = objConfig["interceptSAL"];
            }
            
            this->configslopePH[0] = (slopePH=="")?"0.0":slopePH;
            this->configinterceptPH[0] = (interceptPH=="")?"0.0":interceptPH;
            this->configslopeSAL[0] = (slopeSAL=="")?"0.0":slopeSAL;
            this->configinterceptSAL[0] = (interceptSAL=="")?"0.0":interceptSAL;
          }
          doc.clear();
      }
    }
    int setCalibrations(){
      this->mysd.setCalibrations(this->configslopePH[0], this->configinterceptPH[0], this->configslopeSAL[0], this->configinterceptSAL[0]);
      return 1;
    }
    void setConfigJson(String type){
      if(type=="wifi"){
        this->tempConfigusernamewifi = this->configusernamewifi[0];
        this->tempConfigpasswordwifi = this->configpasswordwifi[0];
      }else if(type=="http"){
        this->tempConfighosthttp = this->confighosthttp[0];
        this->tempConfigporthttp = this->configporthttp[0];
      }else if(type=="mqtt"){
        this->tempConfigbrokermqtt = this->configbrokermqtt[0];
        this->tempConfigportmqtt = this->configportmqtt[0]; 
        this->tempConfigusernamemqtt = this->configusernamemqtt[0];
        this->tempConfigpasswordmqtt = this->configpasswordmqtt[0];
      }
    }
};
Board *board;
void setup()
{

  Serial.begin(9600);
  board = new Board();
}

void loop()
{
  board->startStatus();
  board->start();
}

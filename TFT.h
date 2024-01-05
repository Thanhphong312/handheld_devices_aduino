#include "Print.h"
#include <stdlib.h>
#include <URTouch.h>
#include <stdio.h>
#include <UTFT.h>
#include <TimeLib.h>
#include <KPad.h>
#include "Esp8266.h"
#include <ArduinoJson.h>
#define CHAR_MIN_CHAR '\0'  // Minimum value for char
#define CHAR_MAX_CHAR 127   // Maximum value for char
// DynamicJsonDocument doc(400);
//max width 318
//max heigh 240
using namespace std;

class TFT{
  private:
    int x, y;
    URTouch myTouch{ 6, 5, 4, 3, 2};
    UTFT myGLCD{ILI9341_16, 38, 39, 40, 41};
    char *status;
    int show = 1;
    int checkConnectWifi = 0;
    int checkConfigWifi = 1;
    int KEY_WIDTH = 28; //width of keyboard
    int KEY_HEIGHT = 28; //heigh of keyboard
    int KEY_MARGIN_X = 3; //margin left right of keyboard
    int KEY_MARGIN_Y = 3; //margin top bottom of keyboard
    int NUMBER_ROWS = 4; //number row
    int MAX_NUMBER_COLS = 10;
    int STARTROW = 0; //start in index x
    int STARTCOL = 0; //start in index y 
    int statuskeyboard = 0;
    int chooseDelete = 0;
    char keyAlphabetUpperCase[4][10] = {
            {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'},
            {'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ' '},
            {'↑' , 'Z', 'X', 'C', 'V', 'B', 'N', 'M'},
            {'∞', ',' , ' ', '.'},
          };
    char keyAlphabetLowerCase[4][10] = {
              {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p'},
              {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ' '},
              {'↑', 'z', 'x', 'c', 'v', 'b', 'n', 'm'},
              {'∞', ',' , ' ', '.'},
          };
    char keyAlphabetSpecialKey[4][10] = {
            {'1', '2', '3', '4', '5' , '6', '7', '8', '9', '0'},
            {'!', '@', '#', '$', '%', '^', '&', '*', '(', ')'},
            {'↑' , '-', '\'', '"',  ':', ';', ',', '?'},
            {'∞', ',' , ' ', '.' },
          };
    char keyAlphabet[4][10] = {
              {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p'},
              {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ' '},
              {'↑', 'z', 'x', 'c', 'v', 'b', 'n', 'm'},
              {'∞', ',' , ' ', '.'},
          };
    uint8_t **font;
    unsigned short **icons;
    time_t currentTime;
    const char ***page;
    int numpage = 0;
    int startrow = 0;
    int startcol = 0;  
    char* listchooseidpost[20] = { nullptr }; 
    int numPosts = 0;
    int indexScoll = 0;
    int starthis = 0;
    int endhis = 8;
    int starthispost = 0;
    int endhispost = 8;
    int choosex = 0;
    int choosey = 0;
    int arrhisstatus = 0;
    int lengthScoll = 0;
    int lengthChoose = 0;
    char choosekeyboard = 'q';
    String detailHistory[2] = {"DO","DO"};
    String statusPostServer = "Start send";
    String pageCalibrations = "PH";
    int showPageCalibrations = 1;
    char* statusSensorMeasuring = "no";
    int changedetailhistory = 0;
    int changedetailposthistory = 0;
    String methodPost = "HTTP";
    int sensorMeasuring = 0;
    int startPost = 0 ;
    int startSave = 0 ;
    float valueMeasuring = 0.0;
    int sirens = 0;
  public: 
    TFT() {
          
        }
    ~TFT() {
        
    }
    void setTFT(){
      this->myGLCD.InitLCD();
      this->myGLCD.fillScr(0, 0, 0);
      this->myTouch.InitTouch();
      this->myTouch.setPrecision(PREC_MEDIUM);
      this->status = "no change";
      currentTime = now();
    }
    void setPage() {
      this->numpage = 17;
      this->page = new char**[numpage];

        const char *config[][2] = {
            {"index", "show"}, //0
            {"configwifi", "hide"},//1
            {"start", "hide"},//2
            {"history", "hide"},//3
            {"calibrations", "hide"},//4
            {"connectwifi", "hide"},//5
            {"detailhistory","hide"},//6
            {"detailpost","hide"},//7
            {"confighttp","hide"},//8
            {"configmqtt","hide"},//9
            {"chooseconfig","hide"},//10
            {"connecthttp","hide"},//11
            {"connectmqtt","hide"},//12
            {"detailCalibrations","hide"},//13
            {"configCalibrations","hide"},//14
            {"postServer","hide"},//15
            {"configtime","hihe"}//16
        };

        for (int i = 0; i < this->numpage; i++) {
            page[i] = new const char*[2];
            page[i][0] = config[i][0];
            page[i][1] = config[i][1];
        }
        Serial.println(page[1][1]);
    }

    String getpage(char* getpage){
      for (int i = 0; i < this->numpage; i++) {
          if(this->page[i][0] == getpage){
            return this->page[i][1];
          }
      }
      return "hide";
    }
    int getSensorMeasuring(){
      return this->sensorMeasuring;
    }
    int getStartPost(){
      return this->startPost;
    }
    void setSirens(int sirens){
      this->sirens = sirens;
    }
    int getSirens(){
      return this->sirens;
    }
    void setStartPost(int post){
      this->startPost = post;
    }
    int getStartSave(){
      return this->startSave;
    }
    void setStartSave(int save){
      this->startSave = save;
    }
    int getShowPageCalibrations(){
      return showPageCalibrations;
    }
    void setStatusSensorMeasuring(char* statusSensorMeasuring){
      this->statusSensorMeasuring = statusSensorMeasuring;
    }
    char* getStatusSensorMeasuring(){
      return this->statusSensorMeasuring;
    }
    void setShowPageCalibrations(int showPageCalibrations){
      this->showPageCalibrations = showPageCalibrations;
    }
    void setPageCalibrations(String pageCalibrations){
      this->pageCalibrations = pageCalibrations;
    }
    String getPageCalibrations(){
      return this->pageCalibrations;
    }
    void setCheckConnectWifi(int checkConnectWifi){
      this->checkConnectWifi = checkConnectWifi;
    }
    int getCheckConnectWifi(){
      return this->checkConnectWifi;
    }
    void setTimeOut(int hour, int minute, int second, int day, int month, int year){

      // Parse the timeString        
      setTime(hour, minute, second, day, month, year);
      Serial.println(weekday(currentTime));
    }
    
    char* getWeekday(){
      // Print the day of the week
      int dayOfWeek = weekday(currentTime);
      char* dfw = "";
      switch (dayOfWeek) {
        case 1:
          dfw = "Sunday";
          break;
        case 2:
          dfw = "Monday";
          break;
        case 3:
          dfw = "Tuesday";
          break;
        case 4:
          dfw = "Wednesday";
          break;
        case 5:
          dfw = "Thursday";
          break;
        case 6:
          dfw = "Friday";
          break;
        case 7:
          dfw = "Saturday";
          break;
        default:
          dfw = "Sunday";
      }
      return dfw;
    }
    void setStatusKeyboard(int statuskeyboard){
      this->statuskeyboard = statuskeyboard;
    }
    int getStatusKeyboard(){
      return statuskeyboard;
    }
    char* getStatus(){
      return this->status;
    }
    void setStatus(char *status){
      this->status = status;
    }
    void clearScreen(){
      this->myGLCD.clrScr();
    }
    void fillScreen(String color){
      if(color=="white"){
        this->myGLCD.fillScr(255, 255, 255);
      }else if(color=="black"){
        this->myGLCD.fillScr(0, 0, 0);
      }
    }
    // void configpage()
    void setFont(int numfont, uint8_t **listfont) {
        this->font = new uint8_t *[numfont];
        for (int i = 0; i < numfont; i++) {
            this->font[i] = listfont[i];
        }
    }
    void setIcon(int numIcons, unsigned short **listIcons) {
      Serial.println(numIcons);
      
      this->icons = new unsigned short *[numIcons];
      for (int i = 0; i < numIcons; i++) {  
        this->icons[i] = listIcons[i]; // copy the icon data
      }
    }
    void setkeyAlphabet( char newkeyAlphabet[4][10] = nullptr, int numrow = 4, int maxnumcol = 10, int width = 28, int heigh = 28, int margin_x = 3, int margin_y = 3){
      this->NUMBER_ROWS  = numrow;
      this->MAX_NUMBER_COLS = maxnumcol;
      this->KEY_WIDTH = width;
      this->KEY_HEIGHT = heigh;
      this->KEY_MARGIN_X = margin_x;
      this->KEY_MARGIN_Y = margin_y;
      for(int i = 0 ; i < this->NUMBER_ROWS ; i++){
            int NUMBER_COLS = sizeof(newkeyAlphabet[i]);
            // this->keyAlphabet[i] = new char[NUMBER_COLS];
            for(int j = 0 ; j < NUMBER_COLS ; j++){
              Serial.println(j);
              this->keyAlphabet[i][j] = newkeyAlphabet[i][j];
            }
          }
    }  
    bool getdataAvailable(){
      return this->myTouch.dataAvailable();
    }
    int getXTouch(){
      if (this->myTouch.dataAvailable()){
        this->myTouch.read();
        // Serial.println(this->myTouch.getX());
        // Serial.println("-----------");
        return this->myTouch.getX();
      }
      return -1;
    }
    int getYTouch(){
      if (this->myTouch.dataAvailable()){
        this->myTouch.read();
        // Serial.println(this->myTouch.getY());
        // Serial.println("-----------");
        return this->myTouch.getY();
      }
      return -1;
    }
    void drawConnectWifi(int checkConnectWifi){

      this->myGLCD.clrScr();
      // this->myGLCD.fillScr(0, 0, 0);
      //draw box menu
      this->myGLCD.setBackColor(255, 255, 255);
      this->myGLCD.fillRect(1, 15, 317, 17);
      this->myGLCD.fillRect(6, 25, 311, 234);
      /////
    }
    void drawIndex(String content){
      
      if(content=="connectwifi"){
        //print Config Wifi
        this->myGLCD.setBackColor(255, 255, 255);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRect(1, 20, 316, 234);        // this->font[0] = BigFont;
        this->myGLCD.setFont(this->font[2]); // Set a large font for displaying the time
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.print("Connect wifi", 115, 55);
      }else if(content=="index"){
        //print Menu
        this->myGLCD.setBackColor(255, 255, 255);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRect(1, 20, 316, 234);
        this->myGLCD.setFont(this->font[4]); // Set a large font for displaying the time
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.print("Menu", 125, 45);

        this->myGLCD.setFont(this->font[2]); // Set a large font for displaying the time

        this->myGLCD.setBackColor(0, 123, 255);
        //print button config internet
        this->myGLCD.setColor(0, 123, 255);
        this->myGLCD.fillRoundRect (15, 90, 150, 140);
        this->myGLCD.setColor(0, 220, 255);
        this->myGLCD.drawRoundRect (15, 90, 150, 140);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.print("Config", 60, 112);
        
        //print button start
        this->myGLCD.setColor(0, 123, 255);
        this->myGLCD.fillRoundRect (165, 90, 300, 140);
        this->myGLCD.setColor(0, 220, 255);
        this->myGLCD.drawRoundRect (165, 90, 300, 140);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.print("Start", 215, 112); 

        //print button History
        this->myGLCD.setColor(0, 123, 255);
        this->myGLCD.fillRoundRect (15, 155, 150, 205);
        this->myGLCD.setColor(0, 220, 255);
        this->myGLCD.drawRoundRect (15, 155, 150, 205);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.print("History", 55, 175); 

        //print button History
        this->myGLCD.setColor(0, 123, 255);
        this->myGLCD.fillRoundRect (165, 155, 300, 205);
        this->myGLCD.setColor(0, 220, 255);
        this->myGLCD.drawRoundRect (165, 155, 300, 205);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.print("Calibrations", 185, 175);
      }else if(content=="history"){
        this->myGLCD.setBackColor(255, 255, 255);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRect(1, 20, 316, 240);
        this->myGLCD.setFont(this->font[4]);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.print("History", 105, 45);
        this->myGLCD.setBackColor(0, 123, 255);

        //print button PH
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRoundRect (15, 90, 100, 140);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.drawRoundRect (15, 90, 100, 140);
        this->myGLCD.drawBitmap(42,100,32,32,this->icons[11],0,32,32);
        
        //print button SAL
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRoundRect (215, 90, 300, 140);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.drawRoundRect (215, 90, 300, 140);
        this->myGLCD.drawBitmap(243,100,32,32,this->icons[12],0,32,32);

        //print button TEMP
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRoundRect (115, 90, 200, 140);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.drawRoundRect (115, 90, 200, 140);
        this->myGLCD.drawBitmap(145,100,32,32,this->icons[10],0,32,32);

        //print button return
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (7, 25, 37, 55);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (7, 25, 37, 55);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.setFont(this->font[5]);
        this->myGLCD.printNumI(3, 15, 33);
      }else if(content=="detailhistory"){
        this->indexScoll = 0;
        this->starthis = 0;
        this->endhis = 8;
        this->clearList();
        this->setchooseSelect(0);
        this->myGLCD.setBackColor(255, 255, 255);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRect(1, 20, 316, 240);
        // this->myGLCD.setFont(this->font[3]); // Set a large font for displaying the time
        this->myGLCD.setFont(this->font[4]);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.print(this->getNameHistory(), 60, 32);
        this->myGLCD.setFont(this->font[2]);
        this->myGLCD.print("value", 60, 58);
        this->myGLCD.print("date time", 125, 58);
        this->myGLCD.print("("+String(lengthScoll)+")", 200, 58);
        //print button return
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (7, 25, 37, 55);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (7, 25, 37, 55);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.setFont(this->font[5]);
        this->myGLCD.printNumI(3, 15, 33);
        // //print button chooseall
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (7, 75, 37, 105);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (7, 75, 37, 105);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.setFont(this->font[5]);
        this->myGLCD.printNumI(1, 15, 83);
        
        //print button post to server
        this->myGLCD.setColor(13, 110, 253);
        this->myGLCD.fillRoundRect (280, 25, 310, 55);
        this->myGLCD.setColor(13, 110, 253);
        this->myGLCD.drawRoundRect (280, 25, 310, 55);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.setBackColor(13, 110, 253);
        this->myGLCD.setFont(this->font[5]);
        this->myGLCD.printNumI(2, 288, 32);  
        //draw button Up Down
        this->drawChooseUpDown();
      }else if(content=="detailpost"){
        
        this->myGLCD.setBackColor(255, 255, 255);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRect(1, 20, 316, 240);
        // this->myGLCD.setFont(this->font[3]); // Set a large font for displaying the time
        //draw status post
        this->myGLCD.setFont(this->font[4]);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.print(this->statusPostServer, 60, 32);
        this->myGLCD.setFont(this->font[2]);
        this->myGLCD.print("value", 60, 58);
        this->myGLCD.print("date time", 125, 58);
        //print button return
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (7, 25, 37, 55);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (7, 25, 37, 55);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.setFont(this->font[5]);
        this->myGLCD.printNumI(3, 15, 33);
        //draw method
        this->drawChooseMethod(1, 0);
        //print button post to server
        this->myGLCD.setColor(13, 110, 253);
        this->myGLCD.fillRoundRect (280, 25, 310, 55);
        this->myGLCD.setColor(13, 110, 253);
        this->myGLCD.drawRoundRect (280, 25, 310, 55);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.setBackColor(13, 110, 253);
        this->myGLCD.setFont(this->font[5]);
        this->myGLCD.printNumI(4, 288, 34); 
        //draw button Up Down
        this->drawChooseUpDown();
        this->setChangedetailposthistory(1);
      }else if(content=="chooseconfig"){ 
        //print Menu
        this->myGLCD.setBackColor(255, 255, 255);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRect(1, 20, 316, 234);
        this->myGLCD.setFont(this->font[4]); // Set a large font for displaying the time
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.print("Config", 112, 45);

        this->myGLCD.setFont(this->font[2]); // Set a large font for displaying the time
        this->myGLCD.setBackColor(0, 123, 255);

        //print button config wifi
        this->myGLCD.setColor(0, 123, 255);
        this->myGLCD.fillRoundRect (13, 90, 103, 140);
        this->myGLCD.setColor(0, 220, 255);
        this->myGLCD.drawRoundRect (13, 90, 103, 140);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.print("WIFI", 43, 112);
        
        //print button config http
        this->myGLCD.setColor(0, 123, 255);
        this->myGLCD.fillRoundRect (113, 90, 203, 140);
        this->myGLCD.setColor(0, 220, 255);
        this->myGLCD.drawRoundRect (113, 90, 203, 140);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.print("HTTP", 143, 112); 

        //print button config mqtt
        this->myGLCD.setColor(0, 123, 255);
        this->myGLCD.fillRoundRect (213, 90, 303, 140);
        this->myGLCD.setColor(0, 220, 255);
        this->myGLCD.drawRoundRect (213, 90, 303, 140);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.print("MQTT", 243, 112); 

        //print button config timezone
        this->myGLCD.setColor(0, 123, 255);
        this->myGLCD.fillRoundRect (113, 150, 203, 200);
        this->myGLCD.setColor(0, 220, 255);
        this->myGLCD.drawRoundRect (113, 150, 203, 200);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.print("TIME", 143, 172); 

        //print button return
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (7, 25, 37, 55);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (7, 25, 37, 55);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.setFont(this->font[5]);
        this->myGLCD.printNumI(3, 15, 33);
      }else if(content=="confighttp"){ 
        //print button return
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (7, 5, 37, 35);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (7, 5, 37, 35);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.setFont(this->font[5]);
        this->myGLCD.printNumI(3, 15, 13);
        //print button connect
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (280, 5, 310, 35);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (280, 5, 310, 35);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.setFont(this->font[5]);
        this->myGLCD.printNumI(5, 288, 13);
      }else if(content=="configmqtt"){ 
        //print button return
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (7, 5, 37, 35);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (7, 5, 37, 35);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.setFont(this->font[5]);
        this->myGLCD.printNumI(3, 15, 13);
        //print button connect
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (280, 5, 310, 35);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (280, 5, 310, 35);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.setFont(this->font[5]);
        this->myGLCD.printNumI(5, 288, 13);
      }else if(content=="configwifi"){ 
        //print button return
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (7, 5, 37, 35);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (7, 5, 37, 35);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.setFont(this->font[5]);
        this->myGLCD.printNumI(3, 15, 13);
        //print button connect
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (280, 5, 310, 35);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (280, 5, 310, 35);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.setFont(this->font[5]);
        this->myGLCD.printNumI(5, 288, 13);
      }else if(content=="configtime"){ 
        this->myGLCD.setBackColor(255, 255, 255);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRect(1, 20, 316, 234);
        this->myGLCD.setFont(this->font[4]); // Set a large font for displaying the time
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.print("Config time", 70, 45);

        this->myGLCD.setFont(this->font[1]); // Set a large font for displaying the time
        this->myGLCD.setColor(255,255,255);  
        this->myGLCD.fillRect(105, 65, 250, 90);
        this->myGLCD.setColor(0,0,0);
        this->myGLCD.print("Get time from api timezone...", 50, 80);
        delay(1000);
        this->myGLCD.setColor(255,255,255);  
        this->myGLCD.fillRect(90, 92, 150, 150);
        this->myGLCD.setColor(0,0,0);
        this->myGLCD.print("connected...", 110, 100);

      }else if(content=="connecthttp"){
        //print Config Wifi
        this->myGLCD.setBackColor(255, 255, 255);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRect(1, 20, 316, 234);        // this->font[0] = BigFont;
        this->myGLCD.setFont(this->font[2]); // Set a large font for displaying the time
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.print("Connect http", 115, 55);
        this->myGLCD.setColor(255,255,255);
        this->myGLCD.fillRect(105, 65, 250, 90);
        this->myGLCD.setColor(0,0,0);
        this->myGLCD.print("Connected...",115, 75);
      }else if(content=="connectmqtt"){
        //print Config Wifi
        this->myGLCD.setBackColor(255, 255, 255);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRect(1, 20, 316, 234);        // this->font[0] = BigFont;
        this->myGLCD.setFont(this->font[2]); // Set a large font for displaying the time
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.print("Connect mqtt", 115, 55);
        this->myGLCD.setColor(255,255,255);
        this->myGLCD.fillRect(105, 65, 250, 90);
        this->myGLCD.setColor(0,0,0);
        this->myGLCD.print("Connected...",115, 75);
      }else if(content=="calibrations"){
        this->myGLCD.setBackColor(255, 255, 255);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRect(1, 20, 316, 240);
        this->myGLCD.setFont(this->font[4]);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.print("Celibrations", 75, 45);
        this->myGLCD.setBackColor(0, 123, 255);

        //print button PH
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRoundRect (50, 90, 135, 140);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.drawRoundRect (50, 90, 135, 140);
        this->myGLCD.drawBitmap(78,100,32,32,this->icons[11],0,32,32);
        
        //print button Sal
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRoundRect (185, 90, 270, 140);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.drawRoundRect (185, 90, 270, 140);
        this->myGLCD.drawBitmap(213,100,32,32,this->icons[12],0,32,32);

        //print button return
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (7, 25, 37, 55);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (7, 25, 37, 55);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.setFont(this->font[5]);
        this->myGLCD.printNumI(3, 15, 33);
      }else if(content=="detailCalibrations"){
        this->myGLCD.setBackColor(255, 255, 255);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRect(1, 1, 316, 240);
        this->myGLCD.setFont(this->font[4]);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.print(this->pageCalibrations, 8, 50);
        //print button return
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (7, 5, 37, 35);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (7, 5, 37, 35);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.setFont(this->font[5]);
        this->myGLCD.printNumI(3, 15, 13);
        //print button config
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (280, 5, 310, 35);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (280, 5, 310, 35);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.setFont(this->font[5]);
        this->myGLCD.printNumI(6, 288, 13);
        this->setkeyAlphabet(this->keyAlphabetSpecialKey);
      }else if(content=="configCalibrations"){
        //print Config Wifi
        this->myGLCD.setBackColor(255, 255, 255);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRect(1, 20, 316, 234);        // this->font[0] = BigFont;
        this->myGLCD.setFont(this->font[2]); // Set a large font for displaying the time
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.print("Config "+this->pageCalibrations+" calibration", 80, 55);
        this->myGLCD.setColor(255,255,255);
        this->myGLCD.fillRect(105, 65, 250, 90);
        this->myGLCD.setColor(0,0,0);
        this->myGLCD.print("Setup...",115, 75);
      }else if(content=="start"){
         //print Menu
        this->myGLCD.setBackColor(255, 255, 255);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRect(1, 1, 316, 234);
        this->myGLCD.setFont(this->font[4]);
        this->myGLCD.setColor(0, 0, 0);
        int widthcontent = 12;
        const char* title = "Start measuring";
        int titleLength = strlen(title);
        for (int i = 0; i < titleLength; i++) {
            this->myGLCD.printChar(title[i], 65 + i * widthcontent, 22);
        }
        //print window measuring
        this->myGLCD.setColor(0, 128, 255);
        this->myGLCD.setBackColor(0, 128, 255);
        this->myGLCD.fillRoundRect (50, 70, 270, 170);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.drawRoundRect (50, 70, 270, 170);
        //print value measuring
        this->drawMeasuring("None","none","000.00");
        //print button return
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (7, 5, 37, 35);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (7, 5, 37, 35);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.setFont(this->font[5]);
        this->myGLCD.printNumI(3, 15, 13);

        //savepost
        this->drawPostSaveMeasuring(0, 0);

        //print button start measuring
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (50, 180, 150, 210);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (50, 180, 150, 210);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setFont(this->font[2]);
        this->myGLCD.print("start", 82, 192);

        //print button stop measuring
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (170, 180, 270, 210);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (170, 180, 270, 210);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setFont(this->font[2]);
        this->myGLCD.print("stop", 205, 192);

        this->checkSensor(1, 0, 0);
        
        // //print button none
        // this->myGLCD.setColor(192, 192, 192);
        // this->myGLCD.fillRoundRect (7, 5, 37, 35);
        // this->myGLCD.setColor(192, 192, 192);
        // this->myGLCD.drawRoundRect (7, 5, 37, 35);
        // this->myGLCD.setColor(0, 0, 0);
        // this->myGLCD.setBackColor(192, 192, 192);
        // this->myGLCD.setFont(this->font[5]);
        // this->myGLCD.printNumI(3, 15, 13);
      }else if(content=="postServer"){
        //print Config Wifi
        this->myGLCD.setBackColor(255, 255, 255);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRect(1, 20, 316, 234);        // this->font[0] = BigFont;
        this->myGLCD.setFont(this->font[2]); // Set a large font for displaying the time
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.print("Post server http", 100, 35);
        this->myGLCD.setColor(255,255,255);
        this->myGLCD.fillRect(105, 65, 250, 90);
        this->myGLCD.setColor(0,0,0);
        this->myGLCD.print("start post", 120, 55);
      }
    }
    void drawChoosePost(int choosePostSV = 0){
      if(choosePostSV==1){
        this->myGLCD.setColor(11, 98, 227);
        this->myGLCD.fillRoundRect (290, 25, 320, 55);
        this->myGLCD.setColor(13, 110, 253);
        this->myGLCD.drawRoundRect (290, 25, 320, 55);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.setBackColor(13, 110, 253);
        this->myGLCD.setFont(this->font[5]);
        this->myGLCD.printNumI(2, 277, 32);
      }
      this->myGLCD.setColor(13, 110, 253);
      this->myGLCD.fillRoundRect (270, 25, 300, 55);
      this->myGLCD.setColor(13, 110, 253);
      this->myGLCD.drawRoundRect (270, 25, 300, 55);
      this->myGLCD.setColor(255, 255, 255);
      this->myGLCD.setBackColor(13, 110, 253);
      this->myGLCD.setFont(this->font[5]);
      this->myGLCD.printNumI(2, 277, 32);
    }
    void setchooseSelect(int chooseSelectAll){
      if(chooseSelectAll==1){
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (7, 75, 37, 105);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (7, 75, 37, 105);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.setFont(this->font[5]);
        this->myGLCD.printNumI(0, 15, 83);
        this->setChangedetailhistory(1);
      }else{
        Serial.println("delete");
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (7, 75, 37, 105);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (7, 75, 37, 105);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.setFont(this->font[5]);
        this->myGLCD.printNumI(1, 15, 83);
        this->clearList();
        this->setChangedetailhistory(1);
      }
    }
    void CheckChooseSelect(int x, int y){
      if(y>=25&&y<=55){
        if(x>=7&&x<=37){
          this->myGLCD.setColor(220, 2, 2);
          this->myGLCD.drawRoundRect (7, 25, 37, 55);
          this->clearScreen();
          this->setStatus("change");
          this->page[6][1] = "hide";
          this->page[3][1] = "show"; 
        }else if(x>=280&&x<=310){
          this->myGLCD.setColor(255, 255, 255);
          this->myGLCD.fillRoundRect (280, 25, 310, 55);
          this->myGLCD.setColor(13, 110, 253);
          this->myGLCD.drawRoundRect (280, 25, 310, 55);
          this->myGLCD.setColor(0, 0, 0);
          this->myGLCD.setBackColor(255, 255, 255);
          this->myGLCD.setFont(this->font[5]);
          this->myGLCD.printNumI(2, 288, 32);  
          delay(1000);
          this->clearScreen();
          this->setStatus("change");
          this->page[6][1] = "hide";
          this->page[7][1] = "show";
        }
      }else if(y>=69&&y<=231){
        Serial.println("ok1");
        if(x>=7&&x<=37){
          if(y>=75&&y<=105){
            this->choosex = 0;
            this->choosey = 0;
            this->setchooseSelect(0);
          }
        }else if(x>=55&&x<=264){
          this->choosex = x;
          this->choosey = y;
          this->setchooseSelect(1);
          this->setChangedetailhistory(1);
        }else if(x>=265&&x<=285){
            this->choosex = 0;
            this->choosey = 0;
            if(y>=70&&y<=90){
              //increate scoll
              //print choose button up
              this->drawChooseUpDown(1,0);
              if(lengthScoll>8){
                indexScoll--;
                starthis--;
                endhis--;
                if(starthis<0){
                  starthis = 0;
                  endhis = 8;
                }
                this->setChangedetailhistory(1);
              }
              
            }
            if(y>=210&&y<=230){
              //print button down
              this->drawChooseUpDown(0,1);
              if(lengthScoll>8){
                indexScoll++;
                starthis++;
                endhis++;
                if(endhis>lengthScoll){
                  starthis=lengthScoll-8;
                  endhis=lengthScoll;
                }
                this->setChangedetailhistory(1);
              }
            }
            if(indexScoll<=0){
              indexScoll=0;
            }
            if(indexScoll>=lengthScoll-8){
              indexScoll=lengthScoll-8;
            }
        }
      }
        Serial.println("-------------");
        // Serial.println(strlen(this->listidpost));     
    }
    void checkSensor(int sensorTemp = 0, int sensorSal = 0, int sensorPH = 0){
      
      //print button temp
      if(sensorTemp==0){
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (7, 55, 44, 92);    
      }else{
        this->myGLCD.setColor(0,255,0);
        this->myGLCD.fillRoundRect (7, 55, 44, 92);    
      }   
      this->myGLCD.drawBitmap(10, 58, 32, 32,this->icons[10],0,32,32);
      //print button sal
      if(sensorSal==0){
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (7, 100, 44, 137);
      }else{
        this->myGLCD.setColor(0,255,0);
        this->myGLCD.fillRoundRect (7, 100, 44, 137);
      }   
      this->myGLCD.drawBitmap(10, 103, 32,32,this->icons[12],0,32,32);

      //print button ph
      if(sensorPH==0){
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (7, 145, 44, 182);
      }else{
        this->myGLCD.setColor(0,255,0);
        this->myGLCD.fillRoundRect (7, 145, 44, 182);
      }   
      this->myGLCD.drawBitmap(10,148,32,32,this->icons[11],0,32,32);

    }
    void drawChooseUpDown(int chooseup = 0, int choosedown = 0){
      //print button up
      if(chooseup==1){
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRoundRect (266, 70, 286, 90);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (266, 70, 286, 90);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(255, 255, 255);
        this->myGLCD.setFont(this->font[3]);
        this->myGLCD.printNumI(8, 268, 76);
      }else{
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (266, 70, 286, 90);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (266, 70, 286, 90);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.setFont(this->font[3]);
        this->myGLCD.printNumI(8, 268, 76);
      }
      if(choosedown==1){
        //print button down
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRoundRect (266, 210, 286, 230);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (266, 210, 286, 230);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(255, 255, 255);
        this->myGLCD.setFont(this->font[3]);
        this->myGLCD.printNumI(9, 268, 217);
      }else{
        //print button down
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (266, 210, 286, 230);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (266, 210, 286, 230);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.setFont(this->font[3]);
        this->myGLCD.printNumI(9, 268, 217);
      } 
    }
    void setChangedetailhistory(int changedetailhistory){
      this->changedetailhistory = changedetailhistory;
    }
    int getChangedetailhistory(){
      return this->changedetailhistory;
    }
    int getChangedetailposthistory(){
      return this->changedetailposthistory;
    }
    void setChangedetailposthistory(int changedetailposthistory){
      this->changedetailposthistory = changedetailposthistory;
    }
    void setLengthScoll(int lengthScoll){
      this->lengthScoll = lengthScoll;
    }
    
    int getStartHis(){
      return this->starthis;
    }
    int getEndHis(){
      return this->endhis;
    }
    void setLengthScollPost(int lengthChoose){
      this->lengthChoose = lengthChoose;
    }
    int getStartHisPost(){
      return this->starthispost;
    }
   
    void setEndHisPost(int endhispost){
      this->endhispost = endhispost;
    }
    int getEndHisPost(){
      return this->endhispost;
    }
    void setDetailHistory(String namehis, String namefile){
      this->detailHistory[0] = namehis;
      this->detailHistory[1] = namefile;
    }
    String getNameHistory(){
      return this->detailHistory[0];
    }
    String getNameFileHistory(){
      return this->detailHistory[1];
    }
    void drawStatusConnectWifi(String status){
        this->myGLCD.setFont(this->font[1]);
        if(this->getCheckConnectWifi()==1){
          // this->esp8266->setStatus("Connect");
          this->myGLCD.drawBitmap(135, 70, 50, 50, this->icons[9], 0, 50, 50);
          this->myGLCD.setColor(0,0,0);
          this->myGLCD.print("Ok", 145, 145);
          delay(3000);
          this->page[5][1] = "hide";
          this->page[0][1] = "show"; 
          this->status = "change";
          this->clearScreen();
        }else{
          if(status=="before"){
            this->myGLCD.drawBitmap(135, 70, 50, 50, this->icons[8], 0, 50, 50);
          }else{  
            this->myGLCD.setColor(0,0,0);
            this->myGLCD.print("Fail", 145, 145);
            delay(3000);
            this->page[5][1] = "hide";
            this->page[0][1] = "show"; 
            this->status = "change";
            this->clearScreen();
          }
        }      
    }
    void drawStatusConfigHttp(int statusconnect){
        if(statusconnect==1){
          this->myGLCD.setColor(255,255,255);
          this->myGLCD.fillRect(105, 65, 250, 90);
          this->myGLCD.setColor(0,0,0);
          this->myGLCD.print("Success", 135, 75);
          delay(2000);
          this->page[11][1] = "hide"; 
          this->page[0][1] = "show";
          this->status = "change";
          this->clearScreen();
        }else{
          this->myGLCD.setColor(255,255,255);
          this->myGLCD.fillRect(105, 65, 250, 90);
          this->myGLCD.setColor(0,0,0);
          this->myGLCD.print("Failure",135, 75);
          delay(2000);
          this->page[11][1] = "hide";
          this->page[0][1] = "show"; 
          this->status = "change";
          this->clearScreen();
        }       
    }
    void drawStatusConfigMqtt(int statusconnect){
        if(statusconnect==1){
          this->myGLCD.setColor(255,255,255);
          this->myGLCD.fillRect(105, 65, 250, 90);
          this->myGLCD.setColor(0,0,0);
          this->myGLCD.print("Success", 135, 75);
          delay(3000);
          this->clearScreen();
          this->page[12][1] = "hide"; 
          this->page[0][1] = "show";
          this->status = "change";
        }else{
          this->myGLCD.setColor(255,255,255);
          this->myGLCD.fillRect(105, 65, 250, 90);
          this->myGLCD.setColor(0,0,0);
          this->myGLCD.print("Failure",135, 75);
          delay(3000);
          this->clearScreen();
          this->page[12][1] = "hide";
          this->page[0][1] = "show"; 
          this->status = "change";
        }          
    }
    void drawStatus(int percentbattery){
      if(this->getpage("index")=="show"||this->getpage("connectwifi")=="show"||this->getpage("history")=="show"||this->getpage("chooseconfig")=="show"||this->getpage("configtime")=="show"){
        int currentHour = hour();
        int currentMinute = minute();
        int currentSecond = second();
        int statuspin = 6;
        if(percentbattery<=10){
          statuspin = 0;
        }else if(percentbattery>10&&percentbattery<=20){
          statuspin = 1;
        }else if(percentbattery>20&&percentbattery<=40){
          statuspin = 2;
        }else if(percentbattery>40&&percentbattery<=60){
          statuspin = 3;
        }else if(percentbattery>60&&percentbattery<=80){
          statuspin = 4;
        }else if(percentbattery>80&&percentbattery<100){
          statuspin = 5;
        }else if(percentbattery==100){
          statuspin = 6;
        }
        if(this->getCheckConnectWifi()==0){
          this->myGLCD.drawBitmap(271, 4, 13, 11, this->icons[3], 0, 13, 11);
        }else{
          this->myGLCD.drawBitmap(271, 4, 13, 11, this->icons[2], 0, 13, 11);
        }
        if(statuspin==0) this->myGLCD.setColor(255, 0, 0);
        this->myGLCD.setFont(this->font[3]);
        this->myGLCD.printNumI(statuspin, 292, 6);

        this->myGLCD.setColor(255, 255, 255);
        char timeStr[9];
        sprintf(timeStr, "%02d:%02d:%02d", currentHour, currentMinute, currentSecond);
        myGLCD.setFont(this->font[2]); // Set a large font for displaying the time
        this->myGLCD.setBackColor(0, 0, 0);
        this->myGLCD.print(timeStr, 200, 6); // Display the current time in the center of the screen
      }
    }
    void drawPower(){
      int x = 10; // Adjust the x-coordinate according to your preference
      int y = 50; // Adjust the y-coordinate according to your preference
      this->myGLCD.drawBitmap(x, y, this->icons[2], 40, 40); 
    }
    void drawButtons(){
      // Draw the start of buttons
         this->myGLCD.setColor(0, 0, 255);
         this->myGLCD.fillRoundRect (10, 100, 150, 150);
         this->myGLCD.setColor(255, 255, 255);
         this->myGLCD.drawRoundRect (10, 100, 150, 150);
         this->myGLCD.print("Start", 40, 117);
        // Draw the stop of buttons
         this->myGLCD.setColor(0, 0, 255);
         this->myGLCD.fillRoundRect (160, 100, 300, 150);
         this->myGLCD.setColor(255, 255, 255);
         this->myGLCD.drawRoundRect (160, 100, 300, 150);
         this->myGLCD.print("Stop", 195, 117);
    }
    void checkindex(int x, int y){
      // int index = {1,2,3,4};
      //  {"index", "show"},
      //  {"configwifi", "hide"},
      //  {"start", "hide"},
      //  {"history", "hide"},
      //  {"index", "hide"}
      //option config this->myGLCD.fillRoundRect (15, 90, 150, 140);
      //option start  this->myGLCD.fillRoundRect (165, 90, 300, 140);
      //option History this->myGLCD.fillRoundRect (15, 155, 150, 205);
      //option new  this->myGLCD.fillRoundRect (165, 155, 300, 205);
      int check[4] = {0,0,0,0};
      if(x>=15&&x<=150){//configwifi
        if(y>=90&&y<=140){
          this->myGLCD.setColor(0, 0, 0);
          this->myGLCD.drawRoundRect (15, 90, 150, 140);
          check[0] = 1;
          this->clearScreen();
          this->page[10][1] = "show";
          this->page[0][1] = "hide";
          this->setStatus("change");          
        }else if(y>=155&&y<=205){//start
          this->myGLCD.setColor(0, 0, 0);
          this->myGLCD.drawRoundRect (15, 155, 150, 205);
          check[1] = 1;
          this->clearScreen();
          this->page[0][1] = "hide"; 
          this->page[3][1] = "show"; 
          this->setStatus("change");
        }
      }else if(x>=165&&x<=300){
        if(y>=90&&y<=140){//history
          this->myGLCD.setColor(0, 0, 0);
          this->myGLCD.drawRoundRect (165, 90, 300, 140);
          check[2] = 1;
          this->clearScreen();
          this->page[0][1] = "hide"; 
          this->page[2][1] = "show";
          this->setStatus("change");
        }else if(y>=155&&y<=205){//...
          this->myGLCD.setColor(0, 0, 0);
          this->myGLCD.drawRoundRect (165, 155, 300, 205);
          check[3] = 1;
          this->clearScreen();
          this->page[0][1] = "hide"; 
          this->page[4][1] = "show"; 
          this->setStatus("change");
        }
      }
        if(check[0]==0){
          // this->myGLCD.setColor(0, 220, 255);
          // this->myGLCD.drawRoundRect (15, 90, 150, 140);
          this->page[1][1] = "hide"; 
        }
        if(check[1]==0){
          // this->myGLCD.setColor(0, 220, 255);
          // this->myGLCD.drawRoundRect (15, 155, 150, 205);
          this->page[3][1] = "hide"; 
        }
        if(check[2]==0){
          // this->myGLCD.setColor(0, 220, 255);
          // this->myGLCD.drawRoundRect (165, 90, 300, 140);
          this->page[2][1] = "hide"; 
        }
        if(check[3]==0){
          // this->myGLCD.setColor(0, 220, 255);
          // this->myGLCD.drawRoundRect (165, 155, 300, 205);
          this->page[4][1] = "hide"; 
        }
    }
    void checkChooseConfig(int x, int y){ 
      //this->myGLCD.fillRoundRect (13, 90, 103, 140);
      //this->myGLCD.fillRoundRect (113, 90, 203, 140);
      //this->myGLCD.fillRoundRect (213, 90, 303, 140);
      int check[4] = {0,0,0};
      if(y>=90&&y<=140){
        if(x>=13&&x<=103){
          this->myGLCD.setColor(0, 0, 0);
          this->myGLCD.drawRoundRect (13, 90, 103, 140);
          this->clearScreen();
          this->page[1][1] = "show";
          this->page[10][1] = "hide";
          check[0] = 1;
          this->setShow(1);
          this->setChangeConfigWifi(1); 
          this->setStatus("change");
        }else if(x>=113&&x<=203){
          this->myGLCD.setColor(0, 0, 0);
          this->myGLCD.drawRoundRect (113, 90, 203, 140);
          check[1] = 1;
          this->clearScreen();
          this->page[8][1] = "show";
          this->page[10][1] = "hide"; 
          this->setShow(1);
          this->setChangeConfigWifi(1); 
          this->setStatus("change");
        }else if(x>=213&&x<=303){
          this->myGLCD.setColor(0, 0, 0);
          this->myGLCD.drawRoundRect (213, 90, 303, 140);
          check[2] = 1;
          this->clearScreen();
          this->page[9][1] = "show";
          this->page[10][1] = "hide"; 
          this->setShow(1);
          this->setChangeConfigWifi(1); 
          this->setStatus("change");
        }
      }else if(y>=25&&y<=55){
        if(x>=7&&x<=37){
          this->myGLCD.setColor(220, 2, 2);
          this->myGLCD.drawRoundRect (7, 25, 37, 55);
          this->clearScreen();
          this->setStatus("change");
          this->page[0][1] = "show";
          this->page[10][1] = "hide"; 
        }
      }else if(y>=150&&y<=200){
        if(x>=113&&x<=203){
          this->myGLCD.setColor(220, 2, 2);
          this->myGLCD.drawRoundRect (113, 150, 203, 200);
          this->clearScreen();
          this->setStatus("change");
          this->page[16][1] = "show";
          this->page[10][1] = "hide";
        }
      }
        if(check[0]==0){
          // if(this->status=="change"){

          // }
          // this->myGLCD.setColor(0, 220, 255);
          // this->myGLCD.drawRoundRect (13, 90, 103, 140);
          this->page[1][1] = "hide"; 
        }
        if(check[1]==0){
          // this->myGLCD.setColor(0, 220, 255);
          // this->myGLCD.drawRoundRect (113, 90, 203, 140);
          this->page[8][1] = "hide"; 
        }
        if(check[2]==0){
          // this->myGLCD.setColor(0, 220, 255);
          // this->myGLCD.drawRoundRect (213, 90, 303, 140);
          this->page[9][1] = "hide"; 
        }
    }
    void checkChooseCalibrations(int x, int y){
      if(y>=25&&y<=55){
          if(x>=7&&x<=35){
            this->myGLCD.setColor(220, 2, 2);
            this->myGLCD.drawRoundRect (7, 25, 37, 55);
            this->clearScreen();
            this->page[0][1] = "show";
            this->page[4][1] = "hide";
            this->setStatus("change");     
          }
      }else if(y>=90&&y<=140){
        if(x>=50&&x<=135){
          this->myGLCD.setColor(70, 144, 255);
          this->myGLCD.drawRoundRect (50, 90, 135, 140);
          this->clearScreen();
          this->pageCalibrations = "PH";
          this->page[13][1] = "show";
          this->page[4][1] = "hide";
          this->setShowPageCalibrations(1);
          this->setStatus("change");
        }else if(x>=185&&x<=270){
          this->myGLCD.setColor(70, 144, 255);
          this->myGLCD.drawRoundRect (185, 90, 270, 140);
          this->clearScreen();
          this->pageCalibrations = "SAL";
          this->page[13][1] = "show";
          this->page[4][1] = "hide";
          this->setShowPageCalibrations(1);
          this->setStatus("change");
        }
      }
    }
    int getChangeConfigWifi(){
      return this->checkConfigWifi;
    }
    void setChangeConfigWifi(int checkConfigWifi){
      this->checkConfigWifi = checkConfigWifi;
    }
    void setShow(int show){
      this->show = show;
    }
    int getShow(){
      return this->show;
    }
    void setChooseKeyPad(char key){
      this->choosekeyboard = key;
    }
    void drawConfigServerHttp(String host[], String port[]){
      this->myGLCD.setFont(this->font[2]);
      this->myGLCD.setBackColor(255, 255, 255);
      this->myGLCD.setColor(192, 192, 192);
      //draw host
      this->myGLCD.fillRoundRect(60, 5, 260, 35);
      this->myGLCD.setColor(0, 0, 0);
      this->myGLCD.setBackColor(192, 192, 192);
      this->myGLCD.print("host:", 68, 17);

      //draw port
      this->myGLCD.setColor(192, 192, 192);
      this->myGLCD.fillRoundRect(60, 40, 260, 70);
      this->myGLCD.setColor(0, 0, 0);
      this->myGLCD.setBackColor(192, 192, 192);
      this->myGLCD.print("port:", 68, 52);

      if(host[2]=="success"&&port[2]=="success"){
        this->myGLCD.setColor(0, 255, 100);
        this->myGLCD.drawRoundRect(60, 5, 260, 35);
        this->myGLCD.drawRoundRect(60, 40, 260, 70);
      }else{
        if(host[1]=="selected"){
          this->myGLCD.setColor(0, 255, 100);
          this->myGLCD.drawRoundRect(60, 5, 260, 35);
        }else if(host[1]=="unselected"){
          this->myGLCD.setColor(0, 0, 0);
          this->myGLCD.drawRoundRect(60, 5, 260, 35);
        }
        if(port[1]=="selected"){
          this->myGLCD.setColor(0, 255, 100);
          this->myGLCD.drawRoundRect(60, 40, 260, 70);
        }else if(port[1]=="unselected"){
          this->myGLCD.setColor(0, 0, 0);
          this->myGLCD.drawRoundRect(60, 40, 260, 70);
        }
      }
      this->myGLCD.setColor(0, 0, 0);
      this->myGLCD.setBackColor(192, 192, 192);
      this->myGLCD.print(host[0],110,17);
      this->myGLCD.print(port[0],110,52);
    }
    void drawConfigServerMqtt(String broker[], String port[], String username[], String password[] ){
      this->myGLCD.setFont(this->font[2]);
      this->myGLCD.setBackColor(255, 255, 255);
      this->myGLCD.setColor(192, 192, 192);
      //draw host
      this->myGLCD.fillRoundRect(60, 5, 260, 35);
      this->myGLCD.setColor(0, 0, 0);
      this->myGLCD.setBackColor(192, 192, 192);
      this->myGLCD.print("broker:", 68, 17);

      //draw port
      this->myGLCD.setColor(192, 192, 192);
      this->myGLCD.fillRoundRect(60, 40, 260, 70);
      this->myGLCD.setColor(0, 0, 0);
      this->myGLCD.setBackColor(192, 192, 192);
      this->myGLCD.print("port:", 68, 52);

      //draw username
      this->myGLCD.setColor(192, 192, 192);
      this->myGLCD.fillRoundRect(10, 75, 160, 105);
      this->myGLCD.setColor(0, 0, 0);
      this->myGLCD.setBackColor(192, 192, 192);

      //draw password
      this->myGLCD.setColor(192, 192, 192);
      this->myGLCD.fillRoundRect(170, 75, 310, 105);
      this->myGLCD.setColor(0, 0, 0);
      this->myGLCD.setBackColor(192, 192, 192);

      if(broker[2]=="success"&&port[2]=="success"&&username[2]=="success"&&password[2]=="success"){
        this->myGLCD.setColor(0, 255, 100);
        this->myGLCD.drawRoundRect(60, 5, 260, 35);
        this->myGLCD.drawRoundRect(60, 40, 260, 70);
        this->myGLCD.drawRoundRect(10, 75, 160, 105);
        this->myGLCD.drawRoundRect(170, 75, 310, 105);
      }else{
        if(broker[1]=="selected"){
          this->myGLCD.setColor(0, 255, 100);
          this->myGLCD.drawRoundRect(60, 5, 260, 35);
        }else if(broker[1]=="unselected"){
          this->myGLCD.setColor(0, 0, 0);
          this->myGLCD.drawRoundRect(60, 5, 260, 35);
        }
        if(port[1]=="selected"){
          this->myGLCD.setColor(0, 255, 100);
          this->myGLCD.drawRoundRect(60, 40, 260, 70);
        }else if(port[1]=="unselected"){
          this->myGLCD.setColor(0, 0, 0);
          this->myGLCD.drawRoundRect(60, 40, 260, 70);
        }
        if(username[1]=="selected"){
          this->myGLCD.setColor(0, 255, 100);
          this->myGLCD.drawRoundRect(10, 75, 160, 105);
        }else if(port[1]=="unselected"){
          this->myGLCD.setColor(0, 0, 0);
          this->myGLCD.drawRoundRect(10, 75, 160, 105);
        }
        if(password[1]=="selected"){
          this->myGLCD.setColor(0, 255, 100);
          this->myGLCD.drawRoundRect(170, 75, 310, 105);
        }else if(port[1]=="unselected"){
          this->myGLCD.setColor(0, 0, 0);
          this->myGLCD.drawRoundRect(170, 75, 310, 105);
        }
      }
      this->myGLCD.setColor(0, 0, 0);
      this->myGLCD.setBackColor(192, 192, 192);
      int minWidthContentBroker = 125;
      int maxWidthContentBroker = 250;
      int startContenBroker = 125;
      char* textBroker = broker[0].c_str();
      int lengthContentBroker = strlen(textBroker);
      if(broker[1]=="selected"){
        if(maxWidthContentBroker<startContenBroker+(lengthContentBroker-1)*7){
          startContenBroker = startContenBroker - ((startContenBroker+(lengthContentBroker-1)*7) - maxWidthContentBroker)  ;
        }
      }
      
      for(int i = 0; i<lengthContentBroker ; i++){
        int indexContent = startContenBroker+i*7;
        if(indexContent>=minWidthContentBroker&&indexContent<=maxWidthContentBroker){
          this->myGLCD.printChar(textBroker[i],indexContent,17);
        }
      }
      int minWidthContentUsername = 15;
      int maxWidthContentUsername = 150; 
      int startContenUsername = 15;
      char* textUsername = username[0].c_str();
      int lengthContentUsername = strlen(textUsername);
      if(username[1]=="selected"){
        if(maxWidthContentUsername<startContenUsername+(lengthContentUsername-1)*7){
          startContenUsername = startContenUsername - ((startContenUsername+(lengthContentUsername-1)*7) - maxWidthContentUsername)  ;
        }
      }
      for(int i = 0; i<lengthContentUsername ; i++){
        int indexContent = startContenUsername+i*7;
        if(indexContent>=minWidthContentUsername&&indexContent<=maxWidthContentUsername){
          this->myGLCD.printChar(textUsername[i],indexContent,87);
        }
      }
      int minWidthContentPassword = 175;
      int maxWidthContentPassword = 300; 
      int startContentPassword = 175;
      char* textPassword = password[0].c_str();
      int lengthContentPassword = strlen(textPassword);
      if(password[1]=="selected"){
        if(maxWidthContentPassword<startContentPassword+(lengthContentPassword-1)*7){
          startContentPassword = startContentPassword - ((startContentPassword+(lengthContentPassword-1)*7) - maxWidthContentPassword)  ;
        }
      }
      for(int i = 0; i<lengthContentPassword ; i++){
        int indexContent = startContentPassword+i*7;
        if(indexContent>=minWidthContentPassword&&indexContent<=maxWidthContentPassword){
          this->myGLCD.printChar(textPassword[i],indexContent,87);
        }
      }
      this->myGLCD.print(port[0],110,52);
    }
    void drawConfigWifi(String configname[], String configpassword[]){
      this->myGLCD.setFont(this->font[2]);
      this->myGLCD.setBackColor(255, 255, 255);
      this->myGLCD.setColor(192, 192, 192);
      //draw name
      this->myGLCD.fillRoundRect(60, 5, 260, 35);
      this->myGLCD.drawBitmap(70, 13, 16, 16, this->icons[0], 0, 16, 16);

      //draw password
      this->myGLCD.fillRoundRect(60, 40, 260, 70);
      this->myGLCD.drawBitmap(68, 47, 16, 16, this->icons[1], 0, 16, 16);

      if(configname[2]=="success"&&configname[2]=="success"){
        this->myGLCD.setColor(0, 255, 100);
        this->myGLCD.drawRoundRect(60, 5, 260, 35);
        this->myGLCD.drawRoundRect(60, 40, 260, 70);
      }else{
        if(configname[1]=="selected"){
          this->myGLCD.setColor(0, 255, 100);
          this->myGLCD.drawRoundRect(60, 5, 260, 35);
        }else if(configname[1]=="unselected"){
          this->myGLCD.setColor(0, 0, 0);
          this->myGLCD.drawRoundRect(60, 5, 260, 35);
        }
        if(configpassword[1]=="selected"){
          this->myGLCD.setColor(0, 255, 100);
          this->myGLCD.drawRoundRect(60, 40, 260, 70);
        }else if(configpassword[1]=="unselected"){
          this->myGLCD.setColor(0, 0, 0);
          this->myGLCD.drawRoundRect(60, 40, 260, 70);
        }
      }
      this->myGLCD.setColor(0, 0, 0);
      this->myGLCD.setBackColor(192, 192, 192);
      this->myGLCD.print(configname[0],90,17);
      this->myGLCD.print(configpassword[0],90,52);
    }
    void drawConfigpageCalibrations(String slope[], String intercept[]){

      this->myGLCD.setFont(this->font[2]);
      this->myGLCD.setBackColor(255, 255, 255);
      this->myGLCD.setColor(192, 192, 192);

      //draw slope
      this->myGLCD.fillRoundRect(60, 15, 260, 45);
      this->myGLCD.setColor(0, 0, 0);
      this->myGLCD.setBackColor(255, 255, 255);
      this->myGLCD.print("slope:", 68, 5);

      //draw intercept
      this->myGLCD.setColor(192, 192, 192);
      this->myGLCD.setBackColor(192, 192, 192);
      this->myGLCD.fillRoundRect(60, 65, 260, 95);
      this->myGLCD.setColor(0, 0, 0);
      this->myGLCD.setBackColor(255, 255, 255);
      this->myGLCD.print("intercept:", 68, 55);

      if(slope[2]=="success"&&intercept[2]=="success"){
        this->myGLCD.setColor(0, 255, 100);
        this->myGLCD.drawRoundRect(60, 15, 260, 45);
        this->myGLCD.drawRoundRect(60, 65, 260, 95);
      }else{
        if(slope[1]=="selected"){
          this->myGLCD.setColor(0, 255, 100);
          this->myGLCD.drawRoundRect(60, 15, 260, 45);
        }else if(slope[1]=="unselected"){
          this->myGLCD.setColor(0, 0, 0);
          this->myGLCD.drawRoundRect(60, 15, 260, 45);
        }
        if(intercept[1]=="selected"){
          this->myGLCD.setColor(0, 255, 100);
          this->myGLCD.drawRoundRect(60, 65, 260, 95);
        }else if(intercept[1]=="unselected"){
          this->myGLCD.setColor(0, 0, 0);
          this->myGLCD.drawRoundRect(60, 65, 260, 95);
        }
      }
      this->myGLCD.setColor(0, 0, 0);
      this->myGLCD.setBackColor(192, 192, 192);
      this->myGLCD.print(slope[0],70,27);
      this->myGLCD.print(intercept[0],70, 76);
      Serial.println(slope[0]);
      Serial.println(intercept[0]);
    }
    void drawKeyboardWifi(char key, String configname[], String configpassword[], int x = 0 , int y = 0) {
      int startX = KEY_MARGIN_X;
      int startY = KEY_MARGIN_Y+15;
      this->myGLCD.setBackColor(192, 192, 192);
      if(x==0&&y==0){
        if(key=='U'){
          if(this->startrow==0){
            if(this->startcol==0||this->startcol==1){
              this->startcol=0;
            }else if(this->startcol==2){
              this->startcol=1;
            }else if(this->startcol>=3&&this->startcol<=6){
              this->startcol=2;
            }else if(this->startcol==7){
              this->startcol=3;
            }else if(this->startcol==8||this->startcol==9){
              this->startcol=3;
            }
            this->startrow=3;
          }else if(this->startrow==3){
            if(this->startcol==2){
              this->startcol=2;
            }else if(this->startcol==3){
              this->startcol=8;
            }
            this->startrow--;
          }else{
            this->startrow--;
          }
        }else if(key=='D'){
          if(this->startrow==0){
            if(this->startcol==9){
              if(keyAlphabet[1][9]==' '){
                this->startcol=8;
              }
              this->startrow++;
            }else{
              this->startrow++;
            }
          }else if(this->startrow==3){
            if(this->startcol==3){
              this->startcol=7;
            }else if(this->startrow==1){
              this->startcol=1;
            }
            this->startrow=0;
          }else if(this->startrow==2){
            if(this->startcol>=2&&this->startcol<=6){
              this->startcol=2;
            }else if(this->startcol==7){
              this->startcol=3;
            }else if(this->startcol==8){
              this->startcol=3;
            }
            this->startrow++;
          }else if(this->startrow==1){
            if(this->startcol==9){
              this->startcol=8;
            }
            this->startrow++;
          }else{
            this->startrow++;
          }
        }else if(key=='L'){
          if(this->startrow==0){
            if(this->startcol==0){
              this->startcol=9;
            }else{
              this->startcol--;
            }
          }else if(this->startrow==1){
            if(this->keyAlphabet[1][9]==' '){
              if(this->startcol==0){
                this->startcol=8;
              }else{
                this->startcol--;
              }
            }else{
              if(this->startcol==0){
                this->startcol=9;
              }else{
                this->startcol--;
              }
            }
          }else if(this->startrow==2){
            if(this->startcol==0){
              this->startcol=8;
            }else{
              this->startcol--;
            }
          }else if(this->startrow==3){
            if(this->startcol==0){
              this->startcol=3;
            }else{
              this->startcol--;
            }
          }
        }else if(key=='R'){
          if(this->startrow==0){
            if(this->startcol==9){
              this->startcol=0;
            }else{
              this->startcol++;
            }
          }else if(this->startrow==1){
            if(this->keyAlphabet[1][9]==' '){
              if(this->startcol==8){
                this->startcol=0;
              }else{
                this->startcol++;
              }
            }else{
              if(this->startcol==9){
                this->startcol=0;
              }else{
                this->startcol++;
              }
            }
          }else if(this->startrow==2){
            if(this->startcol==8){
              this->startcol=0;
            }else{
              this->startcol++;
            }
          }else if(this->startrow==3){
            if(this->startcol==3){
              this->startcol=0;
            }else{
              this->startcol++;
            }
          }
        }else if(key=='S'){
          if(this->startrow==2&&this->startcol==0){
            if(getStatusKeyboard()==0){
              this->setkeyAlphabet(this->keyAlphabetUpperCase);
              this->setStatusKeyboard(1);
            }else if(getStatusKeyboard()==1){
              this->setkeyAlphabet(this->keyAlphabetLowerCase);
              this->setStatusKeyboard(0);
            }else if(getStatusKeyboard()==2){
              this->setkeyAlphabet(this->keyAlphabetLowerCase);
              this->setStatusKeyboard(0);
              this->myGLCD.setColor(255, 255, 255);
              this->myGLCD.fillRect(6, 95, 317 ,234 );
            }
          }else if(this->startrow==3&&this->startcol==0){
            this->myGLCD.setColor(255, 255, 255);
            this->myGLCD.fillRect(6, 95, 317 ,234 );
            if(getStatusKeyboard()==0||getStatusKeyboard()==1){
              this->setkeyAlphabet(this->keyAlphabetSpecialKey);
              this->setStatusKeyboard(2);
            }else if(getStatusKeyboard()==2){
              this->setkeyAlphabet(this->keyAlphabetLowerCase);
              this->setStatusKeyboard(0);
            }
          }else if(startrow==3&&startcol==2){
            if(configname[2]=="unsuccess"&&configname[1]=="selected"){
              configname[0]=configname[0]+' ';
              this->setChangeConfigWifi(1);
            }
            if(configpassword[2]=="unsuccess"&&configpassword[1]=="selected"){
              configpassword[0]=configpassword[0]+' ';
              this->setChangeConfigWifi(1);
            }
            this->myGLCD.setColor(255, 255, 255);
            this->myGLCD.fillRect(6, 95, 317 ,234 );
          }else if(this->startrow==2&&this->startcol==8){
            if(configname[2]=="unsuccess"&&configname[1]=="selected"){
              configname[0]=configname[0].substring(0,configname[0].length()-1);
              this->setChangeConfigWifi(1);
            }
            if(configpassword[2]=="unsuccess"&&configpassword[1]=="selected"){
              configpassword[0]=configpassword[0].substring(0,configpassword[0].length()-1);
              this->setChangeConfigWifi(1);
            }
          }else{
            if(configname[2]=="unsuccess"&&configname[1]=="selected"){
              configname[0]=configname[0]+keyAlphabet[this->startrow][this->startcol];
              this->setChangeConfigWifi(1);
            }
            if(configpassword[2]=="unsuccess"&&configpassword[1]=="selected"){
              configpassword[0]=configpassword[0]+this->keyAlphabet[this->startrow][this->startcol];
              this->setChangeConfigWifi(1);
            }
          }
          
          // Serial.println(configname[1]);
          // Serial.println(configname[2]);

          // Serial.println(configpassword[1]);
          // Serial.println(configpassword[2]);
        }else if(key=='#'){
          if(configname[1]=="selected"){
            configname[2] = "success";
            configname[1] = "unselected";
            configpassword[1] = "selected";
            this->setChangeConfigWifi(1);
          }else if(configpassword[1]=="selected"){
            configpassword[2] = "success";
            configpassword[1] = "unselected";
            this->setChangeConfigWifi(1);
          }else{
            if(configname[2]=="success"&&configpassword[2]=="success"){
              this->page[1][1] = "hide";
              this->page[5][1] = "show"; 
              this->setStatus("change");
              this->clearScreen();       
            }
          }
        }else if(key=='*'){
          if(configname[2] == "success"){
            if(configpassword[2] == "success"){
              configpassword[2] = "unsuccess";
              configpassword[1] = "selected";
              this->setChangeConfigWifi(1);
            }else{
              configname[2] = "unsuccess";
              configname[1] = "selected";
              configpassword[2] = "unsuccess";
              configpassword[1] = "unselected";
              this->setChangeConfigWifi(1);
            }
          }else{
            this->setShow(0);
            this->setChangeConfigWifi(0);
            this->page[1][1] = "hide";
            this->page[10][1] = "show"; 
            this->setStatus("change");
            // this->clearScreen();
          }
        }else{
          
        }
        choosekeyboard = this->keyAlphabet[this->startrow][this->startcol];
        for (int row = 0; row < NUMBER_ROWS; row++) {
            int NUMBER_COLS = strlen(this->keyAlphabet[row])-1;
            Serial.println("startrow");
            Serial.println(this->startrow);
            Serial.println("startcol");
            Serial.println(this->startcol);
            Serial.print(choosekeyboard);
            for (int col = 0; col < 10; col++) {
              int keyX = startX + col * (KEY_WIDTH + KEY_MARGIN_X);
              int keyY = startY + row * (KEY_HEIGHT + KEY_MARGIN_Y);
              // Draw the key label (character)
               // Set the text color (black)
              this->myGLCD.setFont(this->font[2]); // Choose a font (you can change the font if needed)
              if(row==2){
                if(col>=0&&col<=8){
                  if(col==0){
                     // Draw the key button
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );                    
                    this->myGLCD.drawBitmap(keyX + 25, keyY + 101, 16, 16, this->icons[4], 0, 16, 16);

                    // this->myGLCD.printChar(keyAlphabetUpperCase[row][col], keyX + 18, keyY + 105);
                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }else if(col==8){
                     // Draw the key button
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );                    
                    this->myGLCD.drawBitmap(keyX + 25, keyY + 101, 16, 16, this->icons[5], 0, 16, 16);

                    // this->myGLCD.printChar(keyAlphabetUpperCase[row][col], keyX + 18, keyY + 105);
                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }else{
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );

                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 30, keyY + 106);
                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  } 
                }
              }else if(row==3){
                if(col>=0&&col<5){
                  if(col==0){
                    // Draw the key button
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );                    
                    this->myGLCD.drawBitmap(keyX + 25, keyY + 101, 16, 16, this->icons[6], 0, 16, 16);

                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }else if(col==1){
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );

                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 30, keyY + 103);

                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }else if(col==2){
                    // Draw the key button
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );                    
                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.print("Space", keyX + 75, keyY + 106);

                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }else if(col==3){
                    // Draw the key button
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 142, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );                    
                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 155, keyY + 106);

                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 142, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 142, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }
                }
              }else if(row==1){
                if(this->keyAlphabet[row][9]!=' '){
                  if(col>=0&&col<=9){
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );

                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 15, keyY + 106);
                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }
                }else{
                  if(col>=0&&col<=8){
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );

                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 30, keyY + 106);
                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }
                }

                
              }else{ 
                this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                this->myGLCD.fillRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );

                this->myGLCD.setColor(0, 0, 0);
                this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 15, keyY + 106);
                if(this->keyAlphabet[row][col]==choosekeyboard){
                  this->myGLCD.setColor(0, 255, 100);
                  this->myGLCD.drawRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                }else{
                  this->myGLCD.setColor(0, 0, 0);
                  this->myGLCD.drawRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                }
              }
            }
          // }
        }    
      }else if(x>=7&&x<=37){
        if(y>=5&&y<=35){
          this->myGLCD.setColor(220, 2, 2);
          this->myGLCD.drawRoundRect (7, 5, 37, 35);
          delay(1000);
          this->clearScreen();
          this->setStatus("change");
          this->page[1][1] = "hide";
          this->page[10][1] = "show";
        }
      }else if(x>=280&&x<=310){
        if(y>=5&&y<=35){
          if(configname[2]=="success"&&configpassword[2]=="success"||configname[0]!=""&&configpassword[0]!=""){
            this->myGLCD.setColor(220, 2, 2);
            this->myGLCD.drawRoundRect (280, 5, 310, 35);
            delay(1000);
            this->clearScreen();
            this->setStatus("change");
            this->page[1][1] = "hide";
            this->page[5][1] = "show";
          }
        }
      }
          
    }  
    void drawKeyboardHttp(char key, String host[], String port[], int x = 0 , int y = 0) {
      int startX = KEY_MARGIN_X;
      int startY = KEY_MARGIN_Y+15;
      this->myGLCD.setBackColor(192, 192, 192);
      if(x==0&&y==0){
        if(key=='U'){
          if(this->startrow==0){
            if(this->startcol==0||this->startcol==1){
              this->startcol=0;
            }else if(this->startcol==2){
              this->startcol=1;
            }else if(this->startcol>=3&&this->startcol<=6){
              this->startcol=2;
            }else if(this->startcol==7){
              this->startcol=3;
            }else if(this->startcol==8||this->startcol==9){
              this->startcol=3;
            }
            this->startrow=3;
          }else if(this->startrow==3){
            if(this->startcol==2){
              this->startcol=2;
            }else if(this->startcol==3){
              this->startcol=8;
            }
            this->startrow--;
          }else{
            this->startrow--;
          }
        }else if(key=='D'){
          if(this->startrow==0){
            if(this->startcol==9){
              if(keyAlphabet[1][9]==' '){
                this->startcol=8;
              }
              this->startrow++;
            }else{
              this->startrow++;
            }
          }else if(this->startrow==3){
            if(this->startcol==3){
              this->startcol=7;
            }else if(this->startrow==1){
              this->startcol=1;
            }
            this->startrow=0;
          }else if(this->startrow==2){
            if(this->startcol>=2&&this->startcol<=6){
              this->startcol=2;
            }else if(this->startcol==7){
              this->startcol=3;
            }else if(this->startcol==8){
              this->startcol=3;
            }
            this->startrow++;
          }else if(this->startrow==1){
            if(this->startcol==9){
              this->startcol=8;
            }
            this->startrow++;
          }else{
            this->startrow++;
          }
        }else if(key=='L'){
          if(this->startrow==0){
            if(this->startcol==0){
              this->startcol=9;
            }else{
              this->startcol--;
            }
          }else if(this->startrow==1){
            if(this->keyAlphabet[1][9]==' '){
              if(this->startcol==0){
                this->startcol=8;
              }else{
                this->startcol--;
              }
            }else{
              if(this->startcol==0){
                this->startcol=9;
              }else{
                this->startcol--;
              }
            }
          }else if(this->startrow==2){
            if(this->startcol==0){
              this->startcol=8;
            }else{
              this->startcol--;
            }
          }else if(this->startrow==3){
            if(this->startcol==0){
              this->startcol=3;
            }else{
              this->startcol--;
            }
          }
        }else if(key=='R'){
          if(this->startrow==0){
            if(this->startcol==9){
              this->startcol=0;
            }else{
              this->startcol++;
            }
          }else if(this->startrow==1){
            if(this->keyAlphabet[1][9]==' '){
              if(this->startcol==8){
                this->startcol=0;
              }else{
                this->startcol++;
              }
            }else{
              if(this->startcol==9){
                this->startcol=0;
              }else{
                this->startcol++;
              }
            }
          }else if(this->startrow==2){
            if(this->startcol==8){
              this->startcol=0;
            }else{
              this->startcol++;
            }
          }else if(this->startrow==3){
            if(this->startcol==3){
              this->startcol=0;
            }else{
              this->startcol++;
            }
          }
        }else if(key=='S'){
          if(this->startrow==2&&this->startcol==0){
            if(getStatusKeyboard()==0){
              this->setkeyAlphabet(this->keyAlphabetUpperCase);
              this->setStatusKeyboard(1);
            }else if(getStatusKeyboard()==1){
              this->setkeyAlphabet(this->keyAlphabetLowerCase);
              this->setStatusKeyboard(0);
            }else if(getStatusKeyboard()==2){
              this->setkeyAlphabet(this->keyAlphabetLowerCase);
              this->setStatusKeyboard(0);
              this->myGLCD.setColor(255, 255, 255);
              this->myGLCD.fillRect(6, 95, 317 ,234 );
            }
          }else if(this->startrow==3&&this->startcol==0){
            this->myGLCD.setColor(255, 255, 255);
            this->myGLCD.fillRect(6, 95, 317 ,234 );
            if(getStatusKeyboard()==0||getStatusKeyboard()==1){
              this->setkeyAlphabet(this->keyAlphabetSpecialKey);
              this->setStatusKeyboard(2);
            }else if(getStatusKeyboard()==2){
              this->setkeyAlphabet(this->keyAlphabetLowerCase);
              this->setStatusKeyboard(0);
            }
          }else if(startrow==3&&startcol==2){
            if(host[2]=="unsuccess"&&host[1]=="selected"){
              host[0]=host[0]+' ';
              this->setChangeConfigWifi(1);
            }
            if(port[2]=="unsuccess"&&port[1]=="selected"){
              port[0]=port[0]+' ';
              this->setChangeConfigWifi(1);
            }
            this->myGLCD.setColor(255, 255, 255);
            this->myGLCD.fillRect(6, 95, 317 ,234 );
          }else if(this->startrow==2&&this->startcol==8){
            if(host[2]=="unsuccess"&&host[1]=="selected"){
              host[0]=host[0].substring(0,host[0].length()-1);
              this->setChangeConfigWifi(1);
            }
            if(port[2]=="unsuccess"&&port[1]=="selected"){
              port[0]=port[0].substring(0,port[0].length()-1);
              this->setChangeConfigWifi(1);
            }
          }else{
            if(host[2]=="unsuccess"&&host[1]=="selected"){
              host[0]=host[0]+keyAlphabet[this->startrow][this->startcol];
              this->setChangeConfigWifi(1);
            }
            if(port[2]=="unsuccess"&&port[1]=="selected"){
              port[0]=port[0]+this->keyAlphabet[this->startrow][this->startcol];
              this->setChangeConfigWifi(1);
            }
          }
          
          // Serial.println(configname[1]);
          // Serial.println(configname[2]);

          // Serial.println(configpassword[1]);
          // Serial.println(configpassword[2]);
        }else if(key=='#'){
          if(host[1]=="selected"){
            host[2] = "success";
            host[1] = "unselected";
            port[1] = "selected";
            this->setChangeConfigWifi(1);
          }else if(port[1]=="selected"){
            port[2] = "success";
            port[1] = "unselected";
            this->setChangeConfigWifi(1);
          }else{
            if(host[2]=="success"&&port[2]=="success"){
              this->page[8][1] = "hide";
              this->page[11][1] = "show"; 
              this->setStatus("change");
              this->clearScreen();       
            }
          }
        }else if(key=='*'){
          if(host[2] == "success"){
            if(port[2] == "success"){
              port[2] = "unsuccess";
              port[1] = "selected";
              this->setChangeConfigWifi(1);
            }else{
              host[2] = "unsuccess";
              host[1] = "selected";
              port[2] = "unsuccess";
              port[1] = "unselected";
              this->setChangeConfigWifi(1);
            }
          }else{
            this->setShow(0);
            this->setChangeConfigWifi(0);
            this->page[1][1] = "hide";
            this->page[10][1] = "show"; 
            this->setStatus("change");
            this->clearScreen();
          }
        }else{
          
        }
        choosekeyboard = this->keyAlphabet[this->startrow][this->startcol];
        for (int row = 0; row < NUMBER_ROWS; row++) {
            int NUMBER_COLS = strlen(this->keyAlphabet[row])-1;
            Serial.println("startrow");
            Serial.println(this->startrow);
            Serial.println("startcol");
            Serial.println(this->startcol);
            Serial.print(choosekeyboard);
            for (int col = 0; col < 10; col++) {
              int keyX = startX + col * (KEY_WIDTH + KEY_MARGIN_X);
              int keyY = startY + row * (KEY_HEIGHT + KEY_MARGIN_Y);
              // Draw the key label (character)
               // Set the text color (black)
              this->myGLCD.setFont(this->font[2]); // Choose a font (you can change the font if needed)
              if(row==2){
                if(col>=0&&col<=8){
                  if(col==0){
                     // Draw the key button
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );                    
                    this->myGLCD.drawBitmap(keyX + 25, keyY + 101, 16, 16, this->icons[4], 0, 16, 16);

                    // this->myGLCD.printChar(keyAlphabetUpperCase[row][col], keyX + 18, keyY + 105);
                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }else if(col==8){
                     // Draw the key button
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );                    
                    this->myGLCD.drawBitmap(keyX + 25, keyY + 101, 16, 16, this->icons[5], 0, 16, 16);

                    // this->myGLCD.printChar(keyAlphabetUpperCase[row][col], keyX + 18, keyY + 105);
                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }else{
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );

                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 30, keyY + 106);
                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  } 
                }
              }else if(row==3){
                if(col>=0&&col<5){
                  if(col==0){
                    // Draw the key button
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );                    
                    this->myGLCD.drawBitmap(keyX + 25, keyY + 101, 16, 16, this->icons[6], 0, 16, 16);

                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }else if(col==1){
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );

                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 30, keyY + 103);

                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }else if(col==2){
                    // Draw the key button
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );                    
                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.print("Space", keyX + 75, keyY + 106);

                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }else if(col==3){
                    // Draw the key button
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 142, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );                    
                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 155, keyY + 106);

                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 142, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 142, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }
                }
              }else if(row==1){
                if(this->keyAlphabet[row][9]!=' '){
                  if(col>=0&&col<=9){
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );

                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 15, keyY + 106);
                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }
                }else{
                  if(col>=0&&col<=8){
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );

                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 30, keyY + 106);
                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }
                }

                
              }else{ 
                this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                this->myGLCD.fillRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );

                this->myGLCD.setColor(0, 0, 0);
                this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 15, keyY + 106);
                if(this->keyAlphabet[row][col]==choosekeyboard){
                  this->myGLCD.setColor(0, 255, 100);
                  this->myGLCD.drawRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                }else{
                  this->myGLCD.setColor(0, 0, 0);
                  this->myGLCD.drawRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                }
              }
            }
          // }
        }
      }else if(x>=7&&x<=37){
        if(y>=5&&y<=35){
          this->myGLCD.setColor(220, 2, 2);
          this->myGLCD.drawRoundRect (7, 5, 37, 35);
          delay(1000);
          this->clearScreen();
          this->setStatus("change");
          this->page[8][1] = "hide";
          this->page[10][1] = "show";
        }
      }else if(x>=280&&x<=310){
        if(y>=5&&y<=35){
          if(host[2]=="success"&&port[2]=="success"||host[0]!=""&&port[0]!=""){
            this->myGLCD.setColor(220, 2, 2);
            this->myGLCD.drawRoundRect (280, 5, 310, 35);
            delay(1000);
            this->clearScreen();
            this->setStatus("change");
            this->page[8][1] = "hide";
            this->page[11][1] = "show";
          }
        }
      }
      
    }  
    void drawKeyboardMqtt(char key, String configbrokerhttp[], String configportmqtt[],String configusernamemqtt[],String configpasswordmqtt[], int x = 0 , int y = 0) {
      int startX = KEY_MARGIN_X;
      int startY = KEY_MARGIN_Y+15;
      this->myGLCD.setBackColor(192, 192, 192);
      if(x==0&&y==0){
        if(key=='U'){
          if(this->startrow==0){
            if(this->startcol==0||this->startcol==1){
              this->startcol=0;
            }else if(this->startcol==2){
              this->startcol=1;
            }else if(this->startcol>=3&&this->startcol<=6){
              this->startcol=2;
            }else if(this->startcol==7){
              this->startcol=3;
            }else if(this->startcol==8||this->startcol==9){
              this->startcol=3;
            }
            this->startrow=3;
          }else if(this->startrow==3){
            if(this->startcol==2){
              this->startcol=2;
            }else if(this->startcol==3){
              this->startcol=8;
            }
            this->startrow--;
          }else{
            this->startrow--;
          }
        }else if(key=='D'){
          if(this->startrow==0){
            if(this->startcol==9){
              if(keyAlphabet[1][9]==' '){
                this->startcol=8;
              }
              this->startrow++;
            }else{
              this->startrow++;
            }
          }else if(this->startrow==3){
            if(this->startcol==3){
              this->startcol=7;
            }else if(this->startrow==1){
              this->startcol=1;
            }
            this->startrow=0;
          }else if(this->startrow==2){
            if(this->startcol>=2&&this->startcol<=6){
              this->startcol=2;
            }else if(this->startcol==7){
              this->startcol=3;
            }else if(this->startcol==8){
              this->startcol=3;
            }
            this->startrow++;
          }else if(this->startrow==1){
            if(this->startcol==9){
              this->startcol=8;
            }
            this->startrow++;
          }else{
            this->startrow++;
          }
        }else if(key=='L'){
          if(this->startrow==0){
            if(this->startcol==0){
              this->startcol=9;
            }else{
              this->startcol--;
            }
          }else if(this->startrow==1){
            if(this->keyAlphabet[1][9]==' '){
              if(this->startcol==0){
                this->startcol=8;
              }else{
                this->startcol--;
              }
            }else{
              if(this->startcol==0){
                this->startcol=9;
              }else{
                this->startcol--;
              }
            }
          }else if(this->startrow==2){
            if(this->startcol==0){
              this->startcol=8;
            }else{
              this->startcol--;
            }
          }else if(this->startrow==3){
            if(this->startcol==0){
              this->startcol=3;
            }else{
              this->startcol--;
            }
          }
        }else if(key=='R'){
          if(this->startrow==0){
            if(this->startcol==9){
              this->startcol=0;
            }else{
              this->startcol++;
            }
          }else if(this->startrow==1){
            if(this->keyAlphabet[1][9]==' '){
              if(this->startcol==8){
                this->startcol=0;
              }else{
                this->startcol++;
              }
            }else{
              if(this->startcol==9){
                this->startcol=0;
              }else{
                this->startcol++;
              }
            }
          }else if(this->startrow==2){
            if(this->startcol==8){
              this->startcol=0;
            }else{
              this->startcol++;
            }
          }else if(this->startrow==3){
            if(this->startcol==3){
              this->startcol=0;
            }else{
              this->startcol++;
            }
          }
        }else if(key=='S'){
          if(this->startrow==2&&this->startcol==0){
            if(getStatusKeyboard()==0){
              this->setkeyAlphabet(this->keyAlphabetUpperCase);
              this->setStatusKeyboard(1);
            }else if(getStatusKeyboard()==1){
              this->setkeyAlphabet(this->keyAlphabetLowerCase);
              this->setStatusKeyboard(0);
            }else if(getStatusKeyboard()==2){
              this->setkeyAlphabet(this->keyAlphabetLowerCase);
              this->setStatusKeyboard(0);
              this->myGLCD.setColor(255, 255, 255);
              this->myGLCD.fillRect(6, 95, 317 ,234 );
            }
          }else if(this->startrow==3&&this->startcol==0){
            this->myGLCD.setColor(255, 255, 255);
            this->myGLCD.fillRect(6, 95, 317 ,234 );
            if(getStatusKeyboard()==0||getStatusKeyboard()==1){
              this->setkeyAlphabet(this->keyAlphabetSpecialKey);
              this->setStatusKeyboard(2);
            }else if(getStatusKeyboard()==2){
              this->setkeyAlphabet(this->keyAlphabetLowerCase);
              this->setStatusKeyboard(0);
            }
          }else if(startrow==3&&startcol==2){
            if(configbrokerhttp[2]=="unsuccess"&&configbrokerhttp[1]=="selected"){
              configbrokerhttp[0]=configbrokerhttp[0]+' ';
              this->setChangeConfigWifi(1);
            }
            if(configportmqtt[2]=="unsuccess"&&configportmqtt[1]=="selected"){
              configportmqtt[0]=configportmqtt[0]+' ';
              this->setChangeConfigWifi(1);
            }
            if(configusernamemqtt[2]=="unsuccess"&&configusernamemqtt[1]=="selected"){
              configusernamemqtt[0]=configusernamemqtt[0]+' ';
              this->setChangeConfigWifi(1);
            }
            if(configpasswordmqtt[2]=="unsuccess"&&configpasswordmqtt[1]=="selected"){
              configpasswordmqtt[0]=configpasswordmqtt[0]+' ';
              this->setChangeConfigWifi(1);
            }
            this->myGLCD.setColor(255, 255, 255);
            this->myGLCD.fillRect(6, 95, 317 ,234 );
          }else if(this->startrow==2&&this->startcol==8){
            if(configbrokerhttp[2]=="unsuccess"&&configbrokerhttp[1]=="selected"){
              configbrokerhttp[0]=configbrokerhttp[0].substring(0,configbrokerhttp[0].length()-1);              
              this->setChangeConfigWifi(1);
            }
            if(configportmqtt[2]=="unsuccess"&&configportmqtt[1]=="selected"){
              configportmqtt[0]=configportmqtt[0].substring(0,configportmqtt[0].length()-1);
              this->setChangeConfigWifi(1);
            }
            if(configusernamemqtt[2]=="unsuccess"&&configusernamemqtt[1]=="selected"){
              configusernamemqtt[0]=configusernamemqtt[0].substring(0,configusernamemqtt[0].length()-1);
              this->setChangeConfigWifi(1);
            }
            if(configpasswordmqtt[2]=="unsuccess"&&configpasswordmqtt[1]=="selected"){
              configpasswordmqtt[0]=configpasswordmqtt[0].substring(0,configpasswordmqtt[0].length()-1);
              this->setChangeConfigWifi(1);
            }
          }else{
            if(configbrokerhttp[2]=="unsuccess"&&configbrokerhttp[1]=="selected"){
              configbrokerhttp[0]=configbrokerhttp[0]+keyAlphabet[this->startrow][this->startcol];
              this->setChangeConfigWifi(1);
            }
            if(configportmqtt[2]=="unsuccess"&&configportmqtt[1]=="selected"){
              configportmqtt[0]=configportmqtt[0]+this->keyAlphabet[this->startrow][this->startcol];
              this->setChangeConfigWifi(1);
            }
            if(configusernamemqtt[2]=="unsuccess"&&configusernamemqtt[1]=="selected"){
              configusernamemqtt[0]=configusernamemqtt[0]+keyAlphabet[this->startrow][this->startcol];
              this->setChangeConfigWifi(1);
            }
            if(configpasswordmqtt[2]=="unsuccess"&&configpasswordmqtt[1]=="selected"){
              configpasswordmqtt[0]=configpasswordmqtt[0]+this->keyAlphabet[this->startrow][this->startcol];
              this->setChangeConfigWifi(1);
            }
          }
          
          // Serial.println(configname[1]);
          // Serial.println(configname[2]);

          // Serial.println(configpassword[1]);
          // Serial.println(configpassword[2]);
        }else if(key=='#'){
          if(configbrokerhttp[1]=="selected"){
            configbrokerhttp[2] = "success";
            configbrokerhttp[1] = "unselected";
            configportmqtt[1] = "selected";
            this->setChangeConfigWifi(1);
          }else if(configportmqtt[1]=="selected"){
            configportmqtt[2] = "success";
            configportmqtt[1] = "unselected";
            configusernamemqtt[1] = "selected";
            this->setChangeConfigWifi(1);
          }else if(configusernamemqtt[1]=="selected"){
            configusernamemqtt[2] = "success";
            configusernamemqtt[1] = "unselected";
            configpasswordmqtt[1] = "selected";
            this->setChangeConfigWifi(1);
          }else if(configpasswordmqtt[1]=="selected"){
            configpasswordmqtt[2] = "success";
            configpasswordmqtt[1] = "unselected";
            this->setChangeConfigWifi(1);
          }else{
            if(configbrokerhttp[2]=="success"&&configportmqtt[2]=="success"&&configusernamemqtt[2]=="success"&&configpasswordmqtt[2]=="success"){
              this->page[9][1] = "hide";
              this->page[12][1] = "show"; 
              this->setStatus("change");
              this->clearScreen();       
            }
          }
        }else if(key=='*'){
          if(configbrokerhttp[2] == "success"){
            if(configportmqtt[2] == "success"){
              if(configusernamemqtt[2] == "success"){
                if(configpasswordmqtt[2]=="success"){
                  configpasswordmqtt[2] = "unsuccess";
                  configpasswordmqtt[1] = "selected";
                  this->setChangeConfigWifi(1);
                }else{
                  configpasswordmqtt[1] = "unselected";
                  configusernamemqtt[1] = "selected";
                  configusernamemqtt[2] = "unsuccess";
                  this->setChangeConfigWifi(1);
                }
              }else{
                configusernamemqtt[1] = "unselected";
                configportmqtt[1] = "selected";
                configportmqtt[2] = "unsuccess";
                this->setChangeConfigWifi(1);
              }
            }else{
              configportmqtt[1] = "unselected";
              configbrokerhttp[1] = "selected";
              configbrokerhttp[2] = "unsuccess";
              this->setChangeConfigWifi(1);
            }
          }else{
            this->setShow(0);
            this->setChangeConfigWifi(0);
            this->page[9][1] = "hide";
            this->page[10][1] = "show"; 
            this->setStatus("change");
            // this->clearScreen();
          }
        }else{
          
        }
        choosekeyboard = this->keyAlphabet[this->startrow][this->startcol];
        for (int row = 0; row < NUMBER_ROWS; row++) {
            int NUMBER_COLS = strlen(this->keyAlphabet[row])-1;
            Serial.println("startrow");
            Serial.println(this->startrow);
            Serial.println("startcol");
            Serial.println(this->startcol);
            Serial.print(choosekeyboard);
            for (int col = 0; col < 10; col++) {
              int keyX = startX + col * (KEY_WIDTH + KEY_MARGIN_X);
              int keyY = startY + row * (KEY_HEIGHT + KEY_MARGIN_Y);
              // Draw the key label (character)
               // Set the text color (black)
              this->myGLCD.setFont(this->font[2]); // Choose a font (you can change the font if needed)
              if(row==2){
                if(col>=0&&col<=8){
                  if(col==0){
                     // Draw the key button
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );                    
                    this->myGLCD.drawBitmap(keyX + 25, keyY + 101, 16, 16, this->icons[4], 0, 16, 16);

                    // this->myGLCD.printChar(keyAlphabetUpperCase[row][col], keyX + 18, keyY + 105);
                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }else if(col==8){
                     // Draw the key button
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );                    
                    this->myGLCD.drawBitmap(keyX + 25, keyY + 101, 16, 16, this->icons[5], 0, 16, 16);

                    // this->myGLCD.printChar(keyAlphabetUpperCase[row][col], keyX + 18, keyY + 105);
                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }else{
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );

                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 30, keyY + 106);
                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  } 
                }
              }else if(row==3){
                if(col>=0&&col<5){
                  if(col==0){
                    // Draw the key button
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );                    
                    this->myGLCD.drawBitmap(keyX + 25, keyY + 101, 16, 16, this->icons[6], 0, 16, 16);

                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }else if(col==1){
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );

                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 30, keyY + 103);

                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }else if(col==2){
                    // Draw the key button
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );                    
                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.print("Space", keyX + 75, keyY + 106);

                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }else if(col==3){
                    // Draw the key button
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 142, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );                    
                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 155, keyY + 106);

                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 142, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 142, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }
                }
              }else if(row==1){
                if(this->keyAlphabet[row][9]!=' '){
                  if(col>=0&&col<=9){
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );

                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 15, keyY + 106);
                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }
                }else{
                  if(col>=0&&col<=8){
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );

                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 30, keyY + 106);
                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }
                }

                
              }else{ 
                this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                this->myGLCD.fillRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );

                this->myGLCD.setColor(0, 0, 0);
                this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 15, keyY + 106);
                if(this->keyAlphabet[row][col]==choosekeyboard){
                  this->myGLCD.setColor(0, 255, 100);
                  this->myGLCD.drawRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                }else{
                  this->myGLCD.setColor(0, 0, 0);
                  this->myGLCD.drawRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                }
              }
            }
          // }
        }
      }else if(x>=7&&x<=37){
        if(y>=5&&y<=35){
          this->myGLCD.setColor(220, 2, 2);
          this->myGLCD.drawRoundRect (7, 5, 37, 35);
          delay(1000);
          this->clearScreen();
          this->setStatus("change");
          this->page[9][1] = "hide";
          this->page[10][1] = "show";
        }
      }else if(x>=280&&x<=310){
        if(y>=5&&y<=35){
          if(configbrokerhttp[2]=="success"&&configportmqtt[2]=="success"||configbrokerhttp[0]!=""&&configportmqtt[0]!=""){
            this->myGLCD.setColor(220, 2, 2);
            this->myGLCD.drawRoundRect (280, 5, 310, 35);
            delay(1000);
            this->clearScreen();
            this->setStatus("change");
            this->page[9][1] = "hide";
            this->page[12][1] = "show";
          }
        }
      }
    }  
    void drawKeyboardCalibrations(char key, String slope[], String intercept[], int x = 0, int y = 0){
      int startX = KEY_MARGIN_X;
      int startY = KEY_MARGIN_Y+15;
      this->myGLCD.setBackColor(192, 192, 192);
      if(x==0&&y==0){
        if(key=='U'){
          if(this->startrow==0){
            if(this->startcol==0||this->startcol==1){
              this->startcol=0;
            }else if(this->startcol==2){
              this->startcol=1;
            }else if(this->startcol>=3&&this->startcol<=6){
              this->startcol=2;
            }else if(this->startcol==7){
              this->startcol=3;
            }else if(this->startcol==8||this->startcol==9){
              this->startcol=3;
            }
            this->startrow=3;
          }else if(this->startrow==3){
            if(this->startcol==2){
              this->startcol=2;
            }else if(this->startcol==3){
              this->startcol=8;
            }
            this->startrow--;
          }else{
            this->startrow--;
          }
        }else if(key=='D'){
          if(this->startrow==0){
            if(this->startcol==9){
              if(keyAlphabet[1][9]==' '){
                this->startcol=8;
              }
              this->startrow++;
            }else{
              this->startrow++;
            }
          }else if(this->startrow==3){
            if(this->startcol==3){
              this->startcol=7;
            }else if(this->startrow==1){
              this->startcol=1;
            }
            this->startrow=0;
          }else if(this->startrow==2){
            if(this->startcol>=2&&this->startcol<=6){
              this->startcol=2;
            }else if(this->startcol==7){
              this->startcol=3;
            }else if(this->startcol==8){
              this->startcol=3;
            }
            this->startrow++;
          }else if(this->startrow==1){
            if(this->startcol==9){
              this->startcol=8;
            }
            this->startrow++;
          }else{
            this->startrow++;
          }
        }else if(key=='L'){
          if(this->startrow==0){
            if(this->startcol==0){
              this->startcol=9;
            }else{
              this->startcol--;
            }
          }else if(this->startrow==1){
            if(this->keyAlphabet[1][9]==' '){
              if(this->startcol==0){
                this->startcol=8;
              }else{
                this->startcol--;
              }
            }else{
              if(this->startcol==0){
                this->startcol=9;
              }else{
                this->startcol--;
              }
            }
          }else if(this->startrow==2){
            if(this->startcol==0){
              this->startcol=8;
            }else{
              this->startcol--;
            }
          }else if(this->startrow==3){
            if(this->startcol==0){
              this->startcol=3;
            }else{
              this->startcol--;
            }
          }
        }else if(key=='R'){
          if(this->startrow==0){
            if(this->startcol==9){
              this->startcol=0;
            }else{
              this->startcol++;
            }
          }else if(this->startrow==1){
            if(this->keyAlphabet[1][9]==' '){
              if(this->startcol==8){
                this->startcol=0;
              }else{
                this->startcol++;
              }
            }else{
              if(this->startcol==9){
                this->startcol=0;
              }else{
                this->startcol++;
              }
            }
          }else if(this->startrow==2){
            if(this->startcol==8){
              this->startcol=0;
            }else{
              this->startcol++;
            }
          }else if(this->startrow==3){
            if(this->startcol==3){
              this->startcol=0;
            }else{
              this->startcol++;
            }
          }
        }else if(key=='S'){
          if(this->startrow==2&&this->startcol==0){
            if(getStatusKeyboard()==0){
              this->setkeyAlphabet(this->keyAlphabetUpperCase);
              this->setStatusKeyboard(1);
            }else if(getStatusKeyboard()==1){
              this->setkeyAlphabet(this->keyAlphabetLowerCase);
              this->setStatusKeyboard(0);
            }else if(getStatusKeyboard()==2){
              this->setkeyAlphabet(this->keyAlphabetLowerCase);
              this->setStatusKeyboard(0);
              this->myGLCD.setColor(255, 255, 255);
              this->myGLCD.fillRect(6, 95, 317 ,234 );
            }
          }else if(startrow==3&&startcol==2){
            if(slope[2]=="unsuccess"&&slope[1]=="selected"){
              slope[0]=slope[0]+' ';
              this->setShowPageCalibrations(1);
            }
            if(intercept[2]=="unsuccess"&&intercept[1]=="selected"){
              intercept[0]=intercept[0]+' ';
              this->setShowPageCalibrations(1);
            }
          }else if(this->startrow==2&&this->startcol==8){
            if(slope[2]=="unsuccess"&&slope[1]=="selected"){
              slope[0]=slope[0].substring(0,slope[0].length()-1);              
              this->setShowPageCalibrations(1);
            }
            if(intercept[2]=="unsuccess"&&intercept[1]=="selected"){
              intercept[0]=intercept[0].substring(0,intercept[0].length()-1);
              this->setShowPageCalibrations(1);
            }
          }else{
            if(slope[2]=="unsuccess"&&slope[1]=="selected"){
              slope[0]=slope[0]+keyAlphabet[this->startrow][this->startcol];
              this->setShowPageCalibrations(1);
            }
            if(intercept[2]=="unsuccess"&&intercept[1]=="selected"){
              intercept[0]=intercept[0]+this->keyAlphabet[this->startrow][this->startcol];
              this->setShowPageCalibrations(1);
            }
          }
          
          // Serial.println(configname[1]);
          // Serial.println(configname[2]);

          // Serial.println(configpassword[1]);
          // Serial.println(configpassword[2]);
        }else if(key=='#'){
          if(slope[1]=="selected"){
            slope[2] = "success";
            slope[1] = "unselected";
            intercept[1] = "selected";
            this->setShowPageCalibrations(1);
          }else if(intercept[1]=="selected"){
            intercept[2] = "success";
            intercept[1] = "unselected";
            this->setShowPageCalibrations(1);
          }else{
            if(slope[2]=="success"&&intercept[2]=="success"){
              this->page[13][1] = "hide";
              this->page[14][1] = "show"; 
              this->setStatus("change");
              this->clearScreen();       
            }
          }
        }else if(key=='*'){
          if(slope[2] == "success"){
            if(intercept[2] == "success"){
              intercept[2] = "unsuccess";
              intercept[1] = "selected";
              this->setShowPageCalibrations(1);
            }else{
              slope[2] = "unsuccess";
              slope[1] = "selected";
              intercept[2] = "unsuccess";
              intercept[1] = "unselected";
              this->setShowPageCalibrations(1);
            }
          }else{
            this->setShow(0);
            this->setShowPageCalibrations(0);
            this->page[1][1] = "hide";
            this->page[10][1] = "show"; 
            this->setStatus("change");
            this->clearScreen();
          }
        }else{
          
        }
        choosekeyboard = this->keyAlphabet[this->startrow][this->startcol];
        for (int row = 0; row < NUMBER_ROWS; row++) {
            int NUMBER_COLS = strlen(this->keyAlphabet[row])-1;
            Serial.println("startrow");
            Serial.println(this->startrow);
            Serial.println("startcol");
            Serial.println(this->startcol);
            Serial.print(choosekeyboard);
            for (int col = 0; col < 10; col++) {
              int keyX = startX + col * (KEY_WIDTH + KEY_MARGIN_X);
              int keyY = startY + row * (KEY_HEIGHT + KEY_MARGIN_Y);
              // Draw the key label (character)
               // Set the text color (black)
              this->myGLCD.setFont(this->font[2]); // Choose a font (you can change the font if needed)
              if(row==2){
                if(col>=0&&col<=8){
                  if(col==0){
                     // Draw the key button
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );                    
                    this->myGLCD.drawBitmap(keyX + 25, keyY + 101, 16, 16, this->icons[4], 0, 16, 16);

                    // this->myGLCD.printChar(keyAlphabetUpperCase[row][col], keyX + 18, keyY + 105);
                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }else if(col==8){
                     // Draw the key button
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );                    
                    this->myGLCD.drawBitmap(keyX + 25, keyY + 101, 16, 16, this->icons[5], 0, 16, 16);

                    // this->myGLCD.printChar(keyAlphabetUpperCase[row][col], keyX + 18, keyY + 105);
                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }else{
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );

                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 30, keyY + 106);
                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  } 
                }
              }else if(row==3){
                if(col>=0&&col<5){
                  if(col==0){
                    // Draw the key button
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );                    
                    this->myGLCD.drawBitmap(keyX + 25, keyY + 101, 16, 16, this->icons[6], 0, 16, 16);

                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(255, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }else if(col==1){
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );

                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 30, keyY + 103);

                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }else if(col==2){
                    // Draw the key button
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );                    
                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.print("Space", keyX + 75, keyY + 106);

                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }else if(col==3){
                    // Draw the key button
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 142, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );                    
                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 155, keyY + 106);

                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 142, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 142, keyY + 95, keyX + 142 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }
                }
              }else if(row==1){
                if(this->keyAlphabet[row][9]!=' '){
                  if(col>=0&&col<=9){
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );

                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 15, keyY + 106);
                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }
                }else{
                  if(col>=0&&col<=8){
                    this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                    this->myGLCD.fillRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );

                    this->myGLCD.setColor(0, 0, 0);
                    this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 30, keyY + 106);
                    if(this->keyAlphabet[row][col]==choosekeyboard){
                      this->myGLCD.setColor(0, 255, 100);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }else{
                      this->myGLCD.setColor(0, 0, 0);
                      this->myGLCD.drawRoundRect(keyX + 18, keyY + 95, keyX + 18 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                    }
                  }
                }

                
              }else{ 
                this->myGLCD.setColor(192, 192, 192); // Set the button color (light gray)
                this->myGLCD.fillRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );

                this->myGLCD.setColor(0, 0, 0);
                this->myGLCD.printChar(this->keyAlphabet[row][col], keyX + 15, keyY + 106);
                if(this->keyAlphabet[row][col]==choosekeyboard){
                  this->myGLCD.setColor(0, 255, 100);
                  this->myGLCD.drawRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                }else{
                  this->myGLCD.setColor(0, 0, 0);
                  this->myGLCD.drawRoundRect(keyX + 3, keyY + 95, keyX + 3 + KEY_WIDTH , keyY + 95 + KEY_HEIGHT - 1 );
                }
              }
            }
          // }
        }
      }else if(x>=7&&x<=37){
        if(y>=5&&y<=35){
          this->myGLCD.setColor(220, 2, 2);
          this->myGLCD.drawRoundRect (7, 5, 37, 35);
          delay(500);
          this->clearScreen();
          this->setStatus("change");
          this->page[13][1] = "hide";
          this->page[4][1] = "show";
        }
      }else if(x>=280&&x<=310){
        if(y>=5&&y<=35){
          if(slope[2]=="success"&&intercept[2]=="success"||slope[0]!=""&&intercept[0]!=""){
            this->myGLCD.setColor(220, 2, 2);
            this->myGLCD.drawRoundRect (280, 5, 310, 35);
            delay(500);
            this->clearScreen();
            this->setStatus("change");
            this->page[13][1] = "hide";
            this->page[14][1] = "show";
          }
        }
      }
    }
    void drawChooseMeasuring(int x, int y){
      if(x>=7&&x<=44){
        if(y>=5&&y<=35){
          if(x>=7&&x<=37){
             this->myGLCD.setColor(220, 2, 2);
            this->myGLCD.drawRoundRect (7, 5, 37, 35);
            delay(500);
            this->clearScreen();
            this->setStatus("change");
            this->page[2][1] = "hide";
            this->page[0][1] = "show";
          }
        }else if(y>=55&&y<=92){
          this->checkSensor(1,0,0);
          this->sensorMeasuring = 1;
          this->setSirens(1);
        }else if(y>=100&&y<=137){
          this->checkSensor(0,1,0);
          this->sensorMeasuring = 2;
          this->setSirens(1);
        }else if(y>=145&&y<=182){
          this->checkSensor(0,0,1);
          this->sensorMeasuring = 3;
          this->setSirens(1);
        }
      }else if(x>=280&&x<=310){
        if(y>=5&&y<=35){
          this->setStartPost(1);
          this->drawPostSaveMeasuring(1, 0);
        }else if(y>=45&&y<=75){
          this->setStartSave(1);
          this->drawPostSaveMeasuring(0, 1);
        }
      }else if(x>=50&&x<=270){
        if(y>=180&&y<=210){
          if(x>=50&&x<=150){
            this->drawStatusMeasuring(1,0);
            this->setSirens(1);
            Serial.println("startttt");
          }else if(x>=170&&x<=270){
            this->drawStatusMeasuring(0,1);
          }
        }
      }
    }
    void drawStatusMeasuring(int start = 0, int stop = 0){
      if(start==1){
        //print button start measuring
        this->myGLCD.setColor(51, 255, 51);
        this->myGLCD.setBackColor(51, 255, 51);
        this->myGLCD.fillRoundRect (50, 180, 150, 210);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (50, 180, 150, 210);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setFont(this->font[2]);
        this->myGLCD.print("start", 82, 192);
        this->statusSensorMeasuring = "start";
      }else{
        //print button start measuring
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.fillRoundRect (50, 180, 150, 210);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (50, 180, 150, 210);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setFont(this->font[2]);
        this->myGLCD.print("start", 82, 192);
      }
      if(stop==1){
        //print button stop measuring
        this->myGLCD.setColor(51, 255, 51);
        this->myGLCD.setBackColor(51, 255, 51);
        this->myGLCD.fillRoundRect (170, 180, 270, 210);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (170, 180, 270, 210);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setFont(this->font[2]);
        this->myGLCD.print("stop", 205, 192);
        this->statusSensorMeasuring = "stop";
      }else{
        //print button stop measuring
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.fillRoundRect (170, 180, 270, 210);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (170, 180, 270, 210);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setFont(this->font[2]);
        this->myGLCD.print("stop", 205, 192);
      }
    }
    void drawPostSaveMeasuring(int post, int save){
      //print button post server
      if(post==0){
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (280, 5, 310, 35);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (280, 5, 310, 35);
        this->myGLCD.setBackColor(192, 192, 192);
      }else{
        this->myGLCD.setColor(0, 128, 255);
        this->myGLCD.fillRoundRect (280, 5, 310, 35);
        this->myGLCD.setColor(0, 128, 255);
        this->myGLCD.drawRoundRect (280, 5, 310, 35);
        this->myGLCD.setBackColor(0, 128, 255);
      }
      this->myGLCD.setColor(0, 0, 0);
      this->myGLCD.setFont(this->font[5]);
      this->myGLCD.printNumI(5, 288, 13);
      //print button save value
      if(save==0){
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (280, 45, 310, 75);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (280, 45, 310, 75);
        this->myGLCD.setBackColor(192, 192, 192);
      }else{
        this->myGLCD.setColor(0, 128, 255);
        this->myGLCD.fillRoundRect(280, 45, 310, 75);
        this->myGLCD.setColor(0, 128, 255);
        this->myGLCD.drawRoundRect (280, 45, 310, 75);
        this->myGLCD.setBackColor(0, 128, 255);
      }

      this->myGLCD.setColor(0, 0, 0);
      this->myGLCD.setFont(this->font[5]);
      this->myGLCD.printNumI(7, 288, 53);
    }
    float getValueMeasuring(){
      return this->valueMeasuring;
    }
    void drawMeasuring(String nameSensor, String unit, String value){
        nameSensor.trim();
        this->valueMeasuring = value.toFloat();
        char* nameSensorMeasuring = nameSensor.c_str();
        char* unitSensorMeasuring = unit.c_str();
        char* valueMeasuring = value.c_str();

        int lengthUnit = 55+(strlen(nameSensorMeasuring))*10+2;
        this->myGLCD.setBackColor(255, 255, 255);
        this->myGLCD.setFont(this->font[1]); // Set a large font for displaying the time
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRect(55, 45, 270, 68);
        this->myGLCD.setColor(0, 0, 0);
        for (int i = 0; i < strlen(nameSensorMeasuring); i++) {
            this->myGLCD.printChar(nameSensorMeasuring[i], 55 + i * 10, 55);
        }

        this->myGLCD.setFont(this->font[1]); // Set a large font for displaying the time
        for (int i = 0; i<=strlen(unitSensorMeasuring)+1; i++) {
          if(i==0){
            this->myGLCD.printChar('(', lengthUnit + i * 10, 55);
          }else if(i==strlen(unitSensorMeasuring)+1){
            this->myGLCD.printChar(')', lengthUnit + i * 10, 55);
          }else{
            this->myGLCD.printChar(unitSensorMeasuring[i-1], lengthUnit + i * 10, 55);
          }
        }
        //print window measuring
        //115 114 118
        this->myGLCD.setColor(0, 128, 255);
        this->myGLCD.setBackColor(0, 128, 255);
        this->myGLCD.setFont(this->font[6]);
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.print((value=="0.00"?"000.00":valueMeasuring), 60, 95);

    }
    void checkChooseHistory(int x, int y){
      // int index = {1,2,3,4};
      //  {"index", "show"},
      //  {"configwifi", "hide"},
      //  {"start", "hide"},
      //  {"history", "hide"},
      //  {"index", "hide"}
      //option config this->myGLCD.fillRoundRect (15, 90, 150, 140);
      //option start  this->myGLCD.fillRoundRect (165, 90, 300, 140);
      //option History this->myGLCD.fillRoundRect (15, 155, 150, 205);
      //option new  this->myGLCD.fillRoundRect (165, 155, 300, 205);
      if(y>=25&&y<=55){
        if(x>=7&&x<=37){
          Serial.println("hidesssssssssss");
          this->myGLCD.setColor(220, 2, 2);
          this->myGLCD.drawRoundRect (7, 25, 37, 55);
          this->page[0][1] = "show";
          this->page[3][1] = "hide"; 
          this->clearScreen();
          this->setStatus("change");
        }
      }else if(y>=90&&y<=140){
        if(x>=15&&x<=100){
          this->myGLCD.setColor(70, 144, 255);
          this->myGLCD.drawRoundRect (15, 90, 100, 140);
          this->setDetailHistory("PH","PH");
          this->clearScreen();
          this->setStatus("change");
          this->page[6][1] = "show";
          this->page[3][1] = "hide"; 
          this->setChangedetailhistory(1);
        }else if(x>=115&&x<=200){
          this->myGLCD.setColor(70, 144, 255);
          this->myGLCD.drawRoundRect (115, 90, 200, 140);
          this->setDetailHistory("TEMPERATURE","TEMP");
          this->clearScreen();
          this->setStatus("change");
          this->page[6][1] = "show";
          this->page[3][1] = "hide"; 
          this->setChangedetailhistory(1);
        }else if(x>=215&&x<=300){
          this->myGLCD.setColor(70, 144, 255);
          this->myGLCD.drawRoundRect (215, 90, 300, 140);
          this->setDetailHistory("SALINITY","SAL");
          this->clearScreen();
          this->setStatus("change");
          this->page[6][1] = "show";
          this->page[3][1] = "hide"; 
          this->setChangedetailhistory(1);
        }
      }      
        // Serial.println(this->page[0][1]);
        // Serial.println(this->page[1][1]);
        // Serial.println(this->page[2][1]);
        // Serial.println(this->page[3][1]);
        // Serial.println(this->page[4][1]);
    }  
    void drawUpDown(){
      if(lengthScoll>8){
        int widthScoll = round(120/(lengthScoll-7));
        int startFillScoll = 90;
        // Serial.println(this->historysensor.size());
        Serial.println("lengthScoll");
        Serial.println(lengthScoll);
        Serial.println("widthScoll");
        Serial.println(widthScoll);
        Serial.println("indexScoll");
        Serial.println(indexScoll);
        Serial.println("choosex");
        Serial.println(this->choosex);
        Serial.println("choosey");
        Serial.println(this->choosey);
        for(int i = 0; i < lengthScoll ; i++){//90-210//265-285
          if(i==indexScoll){
            this->myGLCD.setColor(0,0,0);
            this->myGLCD.fillRect(268, startFillScoll+1, 282, startFillScoll+widthScoll+1);
          }else{
            this->myGLCD.setColor(255,255,255);
            this->myGLCD.fillRect(268, startFillScoll+1, 282, startFillScoll+widthScoll+1);
          }
          startFillScoll+=widthScoll;
        }
      }
    }
     void drawHistory(char* charId, String value,  String datetime, String status, int linex1, int linex2){
      this->myGLCD.setColor(0,0,0);
      this->myGLCD.drawRect(49, 69, 265, 231);
      this->myGLCD.setColor(255,255,255);
      this->myGLCD.setBackColor(255,255,255);
        if(linex1>=70&&linex1<=210){
          if(choosex>=55&&choosex<=265){
            if(choosey>=linex1&&choosey<=linex2){
              choosePost(charId);
              Serial.println("aaaaaa");
              Serial.println(charId);
            }
          }
          if(indexOfChoosePost(charId)!=-1){
            this->myGLCD.setBackColor(192, 192, 192);
            this->myGLCD.setColor(192, 192, 192);
            this->myGLCD.fillRect(50, linex1, 264, linex2);
          }else{
            if(status=="0"){
              this->myGLCD.setBackColor(255,255,0);
              this->myGLCD.setColor(255,255,0);
              this->myGLCD.fillRect(50, linex1, 264, linex2);
            }else if(status=="1"){
              this->myGLCD.setBackColor(0,255,0);
              this->myGLCD.setColor(0,255,0);
              this->myGLCD.fillRect(50, linex1, 264, linex2);
            }
          }
          this->myGLCD.setFont(this->font[2]);
          this->myGLCD.setColor(0,0,0);
          this->myGLCD.print(String(value), 51, linex1+5);
          this->myGLCD.print("|"+datetime, 102, linex1+5);
          Serial.println("---------------");
          // Print the values
          Serial.print("Id: ");
          Serial.println(charId);
          Serial.println(indexOfChoosePost(charId));
          Serial.print("Value: ");
          Serial.println(value);
          Serial.print("status: ");
          Serial.println(status);
          Serial.print("Datetime: ");
          Serial.println(datetime);
          Serial.println("---------------");
        }
      this->drawChooseUpDown(0,0);
    }
    int indexOfChoosePost(char* id) {
      for (int i = 0; i < getLenChoosePost(); ++i) {
          if (listchooseidpost[i] != nullptr && strcmp(listchooseidpost[i], id) == 0) {
              return i;
          }
      }
      return -1;
    }
    int getLenChoosePost() {
      int len = 0;
      for (int i = 0; i < 20; ++i) {
          if (listchooseidpost[i] != nullptr) {
              len++;
          }
      }
      return len;
    }
    char** getlistchooseidpost(){
      return this->listchooseidpost;
    }
    void clearList() {
      // if(getLenChoosePost()>0){
        for (int i = 0; i < 20; ++i) {
          if (listchooseidpost[i] != nullptr) {
              delete[] listchooseidpost[i];
              listchooseidpost[i] = nullptr;
          }
        }
      // }
    }
    void choosePost(char* id) {
      Serial.println("add-----------");
      // trim(id);
      String value = id;
      value.trim();
      Serial.println(value);
      int index = indexOfChoosePost(id);
      if (index == -1) {
          Serial.println("no have");
          // ID doesn't exist, add it to the list
          for (int i = 0; i < 20; ++i) {
              if (listchooseidpost[i] == nullptr) {
                  Serial.println("ok");
                  Serial.println(value);
                  listchooseidpost[i] = new char[strlen(id)];
                  strcpy(listchooseidpost[i], value.c_str());
                  break;
              }
          }
      } else {
          Serial.println("have");
          // ID already exists, remove it from the list
          delete[] listchooseidpost[index];
          listchooseidpost[index] = nullptr;
      }
      Serial.println("add-----------");
    } 
    void drawUpDownPost(){
      int widthScoll = round(120/this->lengthChoose);
      int startFillScoll = 90;
      // Serial.println(this->historysensor.size());
      Serial.println("widthScoll");
      Serial.println(widthScoll);
      Serial.println("indexScoll");
      Serial.println(indexScoll);
      for(int i = 0; i < this->lengthChoose ; i++){//90-210//265-285
        if(i==indexScoll){
          this->myGLCD.setColor(0,0,0);
          this->myGLCD.fillRect(268, startFillScoll+1, 282, startFillScoll+widthScoll+1);
        }else{
          this->myGLCD.setColor(255,255,255);
          this->myGLCD.fillRect(268, startFillScoll+1, 282, startFillScoll+widthScoll+1);
        }
        startFillScoll+=widthScoll;
      }
    }
    void drawPostServer(String id, String value,  String datetime, String status, int linex1, int linex2){
      this->myGLCD.setColor(0,0,0);
      this->myGLCD.drawRect(49, 69, 265, 231);
      this->myGLCD.setColor(255,255,255);
      this->myGLCD.setBackColor(255,255,255);
      if(linex1>=70&&linex1<=210){
        this->myGLCD.setBackColor(255,255,255);
        this->myGLCD.setColor(255,255,255);
        this->myGLCD.fillRect(56, linex1, 264, linex2);
        this->myGLCD.setFont(this->font[2]);
        this->myGLCD.setColor(0,0,0);
        this->myGLCD.print(String(value), 51, linex1+5);
        this->myGLCD.print("|"+datetime,102,linex1+5);
        Serial.println("---------------");
        Serial.print("Id: ");
        Serial.print("Value: ");
        Serial.println(value);
        Serial.print("Datetime: ");
        Serial.println(datetime);
        Serial.println("---------------");
      }
    }
    void checkDrawPostServer(int x, int y){
      if(x>=265&&x<=310){
        if(y>=25&&y<=55){
          if(x>=280&&x<=310){
            this->myGLCD.setColor(255, 255, 255);
            this->myGLCD.fillRoundRect (280, 25, 310, 55);
            this->myGLCD.setColor(13, 110, 253);
            this->myGLCD.drawRoundRect (280, 25, 310, 55);
            this->myGLCD.setColor(0, 0, 0);
            this->myGLCD.setBackColor(255, 255, 255);
            this->myGLCD.setFont(this->font[5]);
            this->myGLCD.printNumI(4, 288, 34); 
            this->clearScreen();
            this->setStatus("change");
            this->page[7][1] = "hide";
            this->page[15][1] = "show"; 
          }
        }else if(y>=70&&y<=90){
          if(x>=265&&x<=285){
            this->drawChooseUpDown(1,0);
            if(lengthChoose>8){
              indexScoll--;
              starthispost--;
              endhispost--;
              if(starthis<0){
                starthispost = 0;
                endhispost = 8;
              }
              this->setChangedetailposthistory(1);
            }    
            if(indexScoll<=0){
              indexScoll=0;
            }
          }
        }else if(y>=210&&y<=230){
          if(x>=265&&x<=285){
            this->drawChooseUpDown(0,1);
            if(lengthChoose>8){
              indexScoll++;
              starthispost++;
              endhispost++;
              if(endhispost>lengthChoose){
                starthispost=lengthChoose-8;
                endhispost=lengthChoose;
              }
              this->setChangedetailposthistory(1);
            }
            if(indexScoll>=lengthChoose-8){
              indexScoll=lengthChoose-8;
            }
          }
        }
      }else if(x>=5&&x<=45){
        if(y>=25&&y<=55){
          if(x>=7&&x<=37){
            this->myGLCD.setColor(220, 2, 2);
            this->myGLCD.drawRoundRect (7, 25, 37, 55);
            this->clearScreen();
            this->setStatus("change");
            this->page[7][1] = "hide";
            this->page[6][1] = "show"; 
          }
        }else if(y>=57&&y<=105){
          this->drawChooseMethod(1,0);
        }else if(y>=125&&y<=155){
          this->drawChooseMethod(0,1);
        }
      } 
    }
    String getChooseMethod(){
      return this->methodPost;
    }
    void drawChooseMethod(int choosehttp = 0, int choosemqtt = 0){
      // //print button http
      if(choosehttp==0){
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (5, 75, 45, 105);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (5, 75, 45, 105);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.setFont(this->font[2]);
        this->myGLCD.print("http", 10, 87);
        this->methodPost = "MQTT";
      }else{
        this->myGLCD.setColor(0, 128, 255);
        this->myGLCD.fillRoundRect (5, 75, 45, 105);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (5, 75, 45, 105);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(0, 128, 255);
        this->myGLCD.setFont(this->font[2]);
        this->myGLCD.print("http", 10, 87);
        this->methodPost = "HTTP";
      }
      if(choosemqtt==0){
        // //print button mqtt
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.fillRoundRect (5, 125, 45, 155);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (5, 125, 45, 155);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(192, 192, 192);
        this->myGLCD.setFont(this->font[2]);
        this->myGLCD.print("mqtt", 10, 137);
        this->methodPost = "HTTP";
      }else{
        this->myGLCD.setColor(0, 128, 255);
        this->myGLCD.fillRoundRect (5, 125, 45, 155);
        this->myGLCD.setColor(192, 192, 192);
        this->myGLCD.drawRoundRect (5, 125, 45, 155);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(0, 128, 255);
        this->myGLCD.setFont(this->font[2]);
        this->myGLCD.print("mqtt", 10, 137);
        this->methodPost = "MQTT";
      }
    }
    void succesPostServer(){
      this->page[6][1] = "show";
      this->page[15][1] = "hide"; 
      this->clearScreen();
      this->setStatus("change");
    }
    void drawStatusPostServer(String result, String resultTime, String status, int hightLine){
      this->myGLCD.setFont(this->font[2]);
      this->myGLCD.setColor(0,0,0); 
      this->myGLCD.print(result, 40, 75+hightLine);
      this->myGLCD.print(resultTime, 90, 75+hightLine);
      if(status=="done"){
        this->myGLCD.setColor(0,255,0); 
      }else if(status=="fail"){
        this->myGLCD.setColor(255,0,0); 
      }
      this->myGLCD.print(status, 250, 75+hightLine);
    }
    void drawStatusConfigCalibrations(int status){
      this->myGLCD.setFont(this->font[4]);
      this->myGLCD.setColor(0, 0, 0);
      this->myGLCD.setColor(255,255,255);
      this->myGLCD.fillRect(105, 65, 250, 90);
      this->myGLCD.setColor(0,0,0);
      this->myGLCD.print((status==1)?"ok":"fail", 115, 75);
      delay(1000);
      this->page[14][1] = "hide"; 
      this->page[0][1] = "show";
      this->status = "change";
      this->clearScreen();
    }
    void drawConfigTime(int status){
      if(status==1){
        this->myGLCD.setColor(255,255,255);  
        this->myGLCD.fillRect(90, 92, 250, 150);
        this->myGLCD.setColor(0,0,0);
        this->myGLCD.print("done...", 110, 100);
        delay(3000);
        this->page[16][1] = "hide"; 
        this->page[0][1] = "show";
        this->status = "change";
        this->clearScreen();
      }else{
        this->myGLCD.setColor(255,255,255);  
        this->myGLCD.fillRect(90, 92, 250, 150);
        this->myGLCD.setColor(0,0,0);
        this->myGLCD.print("fail...", 110, 100);
        delay(3000);
        this->page[16][1] = "hide"; 
        this->page[0][1] = "show";
        this->status = "change";
        this->clearScreen();  
      }
    }
    void drawStatusPostSaveMeasuring(int status, char* value=""){
      this->myGLCD.setFont(this->font[1]);
      if(status==1){
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRoundRect(280, 85, 310, 115);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(255, 255, 255);
        this->myGLCD.print("Ok", 287, 94);
      }else if(status==2){
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRoundRect(280, 85, 310, 115);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(255, 255, 255);
        this->myGLCD.print("Fail", 287, 94);
      }else if(status==0){
        this->myGLCD.setColor(255, 255, 255);
        this->myGLCD.fillRoundRect(280, 85, 310, 115);
        this->myGLCD.setColor(0, 0, 0);
        this->myGLCD.setBackColor(255, 255, 255);
        if(value!=""){
          this->myGLCD.print(value, 287, 94);
        }
      }
    }
};



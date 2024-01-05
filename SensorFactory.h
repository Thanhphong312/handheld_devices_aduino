#ifndef SensorFactory_h
#define SensorFactory_h
#include "Sensor.h"
class SensorFactory{
  private: 
    int sensorNumber;
    char sensorName[16];
    char shortName[12];// 11 char + terminator
    float voltageID;
    char* donVi = "Do C";
    byte resistorIDInfo[4][32] = {
      {110, 111, 110, 101, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,    110, 111, 110, 101, 32, 32, 32, 32, 32, 32,     }, //0 none*
      {84, 101, 109, 112, 101, 114, 97, 116, 117, 114, 101, 32, 32, 32,    84, 69, 77, 80, 32, 32, 32, 32, 32, 32,     }, //1 TEMP*
      {83, 97, 108, 105, 110, 105, 116, 121, 32, 32, 32, 32, 32, 32,    83, 65, 76, 32, 32, 32, 32, 32, 32, 32,     }, //2 SALT*
      {112, 72, 32, 101, 108, 101, 99, 116, 114, 111, 100, 101, 32, 32,    80, 72, 32, 32, 32, 32, 32, 32, 32, 32,     } //3 PH*
    };
    int numberAveraged  = 10;
    int sig1Pin = A0;  // Chân đọc từ cảm biến Sig1
    int pinID = A1;
    Sensor *sensor;
    SensorTemp* sensorTemp = nullptr; // Use 'new' to create instances of derived classes
    SensorSalinity* sensorSalinity = nullptr; // Use 'new' to create instances of derived classes
    SensorPH* sensorPH = nullptr; // Use 'new' to create instances of derived classes
  public:
    SensorFactory(){
     
    };
    ~SensorFactory(){};
    void setSensorFactory(int sig1Pin = A0, int pinID = A1, float slopePH = -7.78, float interceptPH = 16.34, float slopeSAL = 16.34, float interceptSAL = 0.0){
      this->sig1Pin = sig1Pin;
      this->pinID = pinID;
      sensorTemp = new SensorTemp();
      sensorSalinity = new SensorSalinity(slopeSAL, interceptSAL);
      sensorPH = new SensorPH(slopePH, interceptPH);
    }
    void checkSensor(int chooseSensor=0, float slopePH = -7.78, float interceptPH = 16.34, float slopeSAL = 16.34, float interceptSAL = 0.0){
      Serial.println(slopePH, 10);
      Serial.println(interceptPH, 10);
      Serial.println(slopeSAL, 10);
      Serial.println(interceptSAL, 10);
      byte sensorData[128];
      for (int _i = 0; _i < 128; _i++) // clear our digital ID sensor data
      {
          sensorData[_i] = 0;
      }
      if(chooseSensor==0){
        this->voltageID = analogRead(this->pinID) / 1024.0 * 5.00;
      }else{
        if(chooseSensor==1){
          this->voltageID = 2.5;
        }else if(chooseSensor==2){
          this->voltageID = 0.9;
        }else if(chooseSensor==3){
          this->voltageID = 1.7;
        }
      }
      // Serial.println(this->voltageID);
      //0.51 0.62 0.96 1.2
      //PH 1.44
      if (this->voltageID > 2.00 && this->voltageID < 3.20){
        this->sensorNumber = 1; //Stainless Steel or Surface Temperature Probe
        this->sensor = this->sensorTemp;
        this->donVi = "Do C";
      }else if (this->voltageID > 0.50 && this->voltageID < 1.2){
        this->sensorNumber = 2; //Sal
        this->sensorSalinity->config(slopeSAL, interceptSAL);
        this->sensor = this->sensorSalinity;
        this->donVi = "ppt";
      }else if (this->voltageID > 1.40 && this->voltageID < 1.99){
        this->sensorNumber = 3; //PH
        this->sensorSalinity->config(slopePH, interceptPH);
        this->sensor = this->sensorPH;
        this->donVi = "H+";
      } else{
        this->sensorNumber = 0;
        this->sensor = nullptr;
        this->donVi = "none";
      }
      for (int _i = 0; _i < 14; _i++)//
      {
        sensorData[_i] = this->resistorIDInfo[this->sensorNumber][_i];
      }
      //read ShortName char string into array in the right spots, characters 15-24
      for (int _i = 0; _i < 10; _i++)//
      {
        sensorData[20 + _i] = this->resistorIDInfo[this->sensorNumber][14 + _i];
      }
      // Determine the sensor name:
      for (int _i = 0; _i < 16; _i++)
        {
          char _c =  sensorData[_i];
          this->sensorName[_i] = _c;
        }
        this->sensorName[16] = '\0';
        //Determine the short name:
      for (int _i = 0; _i < 11; _i++)
        {
          char _c =  sensorData[_i + 20];  // changed from 28 to 29
          this->shortName[_i] = _c;
        }
      this->shortName[11] = '\0';
    }
    float getVoltageID(){
      return this->voltageID;
    }
    float getVoltageSig1(){
      return this->getConvertAnalog();
    }
    float getSensorNumber(){
      return this->sensorNumber;
    }
    float get(){
      float analog = this->getConvertAnalog();
      // float analog = 5.00;
      if(this->sensor==nullptr){
        return 0;
      }else{
        this->sensor->setAnalog(analog);
        return this->sensor->get();
      }
    }
    float getConvertAnalog() {
      int numberAveraged = 10; //number of readings averaged for reading reported
      int _count;
      int _sum = 0;
      float _voltage;
        for (int _i = 0 ; _i < numberAveraged; _i++)
        {
           _count = analogRead(this->sig1Pin); //read 0 to 5 volt analog lines, Analog 1
          //  _count = 1000; //read 0 to 5 volt analog lines, Analog 1
          _sum = _sum + _count;
        }//end of for loop
      _voltage = _sum / numberAveraged / 1024.0 * 5.0;
      return _voltage;
    }
    void setDonVi(char* donVi){
      this->donVi = donVi;
    }
    char* getSensorName(){
      return this->sensorName;
    }
    char* getShortName(){
      return this->shortName;
    }
    char* getDonVi(){
      return this->donVi;
    }
};
#endif
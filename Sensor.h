#ifndef Sensor_h
#define Sensor_h
class Sensor{
  public:
    virtual float get() = 0;
    virtual void setAnalog(float voltage) = 0;
    virtual void config(float slope, float intercept) = 0;
};
class  SensorTemp : public Sensor{
  private:
      // Các hệ số Steinhart-Hart
    const float K0 = 1.02119e-3;
    const float K1 = 2.22468e-4;
    const float K2 = 1.33342e-7; 
    float resistor = 15000;
    float logR = 0.0;
    float voltage = 0.0;
    float sensorReading = 0.0;
  public:  
    SensorTemp(){
    }
    ~SensorTemp(){}
    void setAnalog(float voltage) override {
      this->voltage = voltage;
    }
    float get() override {
      long _resistance = (this->resistor * this->voltage ) / (5.0-this->voltage);
      this->logR = log(_resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
      sensorReading = 1 / (K0 + (K1 * logR + (K2 * logR * logR * logR)));
      sensorReading =  sensorReading - 273.15; 
      return sensorReading;
    }
    void config(float slope, float intercept){

    }
};
class  SensorSalinity : public Sensor{
  private:
    float voltage = 0.0;
    float sensorReading = 0.0;
    float slope = 16.3;
    float intercept = 0.0;
  public:  
    SensorSalinity(float slope = 16.3, float intercept = 0){
      this->slope = slope;
      this->intercept = intercept;
    }
    void config(float slope = 16.3, float intercept = 0){
      this->slope = slope;
      this->intercept = intercept;
    }
    ~SensorSalinity(){}
    void setAnalog(float voltage) override {
      this->voltage = voltage;
    }
    float get() override {
      Serial.println("slope");
      Serial.println(slope);
      Serial.println("intercept");
      Serial.println(intercept);
      this->sensorReading = this->voltage * slope + intercept;
      return this->sensorReading;
    }
};
class  SensorPH : public Sensor{
  private:
    float voltage = 0.0;
    float sensorReading = 0.0;
    float slope = -7.78;
    float intercept = 16.34;
  public:  
    SensorPH(float slope = -7.78, float intercept = 16.34){
      this->slope = slope;
      this->intercept = intercept;
    }
    ~SensorPH(){}
    void config(float slope = -7.78, float intercept = 16.34){
      this->slope = slope;
      this->intercept = intercept;
    }
    void setAnalog(float voltage) override {
      this->voltage = voltage;
    }
    float get() override {
      this->sensorReading = this->voltage * this->slope + this->intercept;
      return this->sensorReading;
    }
};
#endif

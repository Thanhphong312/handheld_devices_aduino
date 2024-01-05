#ifndef Battery_h
#define Battery_h
class Battery{
  private:
    float fullBatteryVoltage = 4.3; // giá trị điên áp tối đa khi sạc đầy
    float minBatteryVoltage = 2.5;  // giá trị điện áp khi cần sạc
    int pinBattery = A8;
  public:
    void setPinBattery(int pinBattery){
      this->pinBattery = pinBattery;
    }
    void setFullBettery(float fullBatteryVoltage){
      this->fullBatteryVoltage = fullBatteryVoltage;
    }
    void setMinBattery(float minBatteryVoltage){
      this->minBatteryVoltage = minBatteryVoltage;
    }
    int getBattery(){
      float voltage = analogRead(pinBattery) / 1024.0 * 5.00; // Chuyển đổi giá trị analog thành điện áp (V)
      Serial.println("voltage");
      Serial.println(voltage);
      // Ước tính dung lượng pin còn lại dựa trên điện áp pin
      int batteryLevel = map(voltage, minBatteryVoltage, fullBatteryVoltage, 0, 100); // Ánh xạ giá trị điện áp pin vào phạm vi dung lượng pin từ 0 đến 100%
      batteryLevel = constrain(batteryLevel, 0, 100); 
      Serial.println(batteryLevel);
      return batteryLevel;
    }
};
#endif

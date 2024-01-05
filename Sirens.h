#ifndef Sirens_h
#define Sirens_h
class Sirens{
  private:
    int pin = 12;
  public:
    Sirens(){
    }
    ~Sirens(){

    }
    void setPin(int pin){
      this->pin = pin;
      pinMode(this->pin, OUTPUT);
    }
    void ring(){
      digitalWrite(this->pin, HIGH);
    }
    void stop(){
      digitalWrite(this->pin, LOW);
    }
};
#endif
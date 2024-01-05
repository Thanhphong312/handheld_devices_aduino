#ifndef KPad_h
#define KPad_h
#include <Keypad.h>
using namespace std;
class KPad {
private:
    byte CONTROLROWS = 4;
    byte CONTROLLCOLS = 4;
    int show = 1;
    Keypad mykeypad = Keypad(makeKeymap({}), {}, {}, 0, 0);

    char keys[4][4] = {
          {'1','2','3','A'},
          {'4','5','6','B'},
          {'7','8','9','C'},
          {'*','0','#','D'}
        };
    byte rowPins[4] = {14, 15, 16, 17}; // Connect to the row pinouts of the keypad
    byte colPins[4] = {18, 19, 20, 21};

public:
    KPad(){
        char newkey[4][4] = {
            {'1', '2', '3', 'A'},
            {'4', '5', '6', 'B'},
            {'7', '8', '9', 'C'},
            {'*', '0', '#', 'D'}
        };
        byte newpinRow[4] = {14, 15, 16, 17};
        byte newpinCol[4] = {18, 19, 20, 21};
        this->mykeypad = Keypad(makeKeymap(keys), rowPins, colPins, CONTROLROWS, CONTROLLCOLS);

        // mykeypad = Keypad(makeKeymap((char*)this->keys), this->rowPins, this->colPins, this->CONTROLROWS, this->CONTROLLCOLS);
    }
    ~KPad(){

    }
    void setShow(int show){
      this->show = show;
    }
    int getShow(){
      return this->show;
    }
    void setPinRow(byte numRow, byte *newRowPins){
      this->CONTROLROWS = numRow;
      for(int i = 0 ; i< this->CONTROLROWS ; i++){
        this->rowPins[i] = newRowPins[i];
      }
    }
    void setPinCol(byte numCol, byte *newColPins){
      this->CONTROLLCOLS = numCol;
      for(int i = 0 ; i< this->CONTROLLCOLS ; i++){
        this->colPins[i] = newColPins[i];
      }
    }
    void setKey(char **newkeys){
      for (int i = 0; i < this->CONTROLROWS; i++) {
        for (int j = 0; j < this->CONTROLLCOLS; j++) {
          this->keys[i][j] = newkeys[i][j]; // Copy values from newkey to keys
        }
      }
    }

    char getKeyPad(){
      return this->mykeypad.getKey();
    }
    
};
#endif

#ifndef KPad_h
#define KPad_h
#include <Keypad.h>
using namespace std;
class KPad {
private:
    byte controlRows = 3;
    byte controlCols = 3;
    Keypad mykeypad = Keypad(makeKeymap({}), {}, {}, 0, 0);

    char keys[3][3] = {
          {' ','U', ' '},
          {'L','S','R'},
          {'*','D','#'}
        };
    byte rowPins[4] = {14, 15, 16}; // Connect to the row pinouts of the keypad
    byte colPins[4] = {17, 18, 19};

public:
    KPad(){

    }
    ~KPad(){

    }
    void setKeypad(){
        this->mykeypad = Keypad(makeKeymap(keys), rowPins, colPins, controlRows, controlCols);
    }
    void setPinRow(byte numRow, byte *newRowPins){
      this->controlRows = numRow;
      for(int i = 0 ; i< this->controlRows ; i++){
        this->rowPins[i] = newRowPins[i];
      }
    }
    void setPinCol(byte numCol, byte *newColPins){
      this->controlCols = numCol;
      for(int i = 0 ; i< this->controlCols ; i++){
        this->colPins[i] = newColPins[i];
      }
    }
    void setKey(char **newkeys){
      for (int i = 0; i < this->controlRows; i++) {
        for (int j = 0; j < this->controlCols; j++) {
          this->keys[i][j] = newkeys[i][j]; // Copy values from newkey to keys
        }
      }
    }

    char getKeyPad(){
      return this->mykeypad.getKey();
    }
    
};
#endif

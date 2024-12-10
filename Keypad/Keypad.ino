
#include <Keypad.h>

//pins not decided yet


const byte row = 4; 
const byte col = 3; 

char hexaKeys[row][col] = {// keypad buttons
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[row] = {15,2,4,16}; 
byte colPins[col] = {17,5,18}; 

//initialize class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, row, col); 

void setup(){
  Serial.begin(115200);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
  }
}

const int Led = 12;
#define Buzzer 11 //change to any output pin (not analog inputs) 

void setup() { 
  pinMode(Buzzer, OUTPUT); 
  pinMode(Led, OUTPUT);  
}

void  loop(){ 
  digitalWrite(Led, HIGH); 
  delay(500);
  for(int i=0; i<255; i++) { 
    analogWrite(Buzzer, i); //raise the voltage sent out of the pin by 1
    delay(10);  
  }
  digitalWrite(Led, LOW); 
  delay(500);
  for(int i=255; i>0; i--) { 
    analogWrite(Buzzer, i); //lower the voltage sent out of the pin by 1
    delay(10); 
  }
}
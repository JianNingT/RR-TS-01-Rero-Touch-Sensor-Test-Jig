#include <SoftwareSerial.h>
#include <LCD4Bit_mod.h>
#include <Servo.h>
Servo myServo;
#define TxMode LOW    // Master transmit to G15
#define RxMode HIGH   // Master receive from G15
#define CTRL 2

SoftwareSerial mySerial(0, 1);
LCD4Bit_mod lcd = LCD4Bit_mod(2); 
byte errStatus =0;
byte result = 0;
byte check = 0;


//LCD
int  adc_key_val[5] ={30, 150, 360, 535, 760 };
int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
int oldkey=-1;



void setup(){

  pinMode(2, OUTPUT);
  pinMode(13,OUTPUT);

  myServo.attach(11,950,1300);
  myServo.write(950);

   lcd.init();
   lcd.clear();
   lcd.cursorTo(1,0);
   lcd.printIn("CYTRON TECH.");
   lcd.cursorTo(2, 0);
   lcd.printIn("Touch Test Jig");  
   digitalWrite(13,HIGH);
   delay(1000);
   digitalWrite(13,LOW);   

}

void loop(){

//Waiting for Sensor

  lcd.clear();
  lcd.cursorTo(1,0);
  lcd.printIn("Connect Sensor");
  lcd.cursorTo(2,0);
  lcd.printIn("Press 'Select'");
  result = 0;

  while(result == 0){

  adc_key_in = analogRead(0);    // read the value from the sensor  
  key = get_key(adc_key_in);            // convert into key press
  
  if (key != oldkey)            // if keypress is detected
  {
    delay(50);    // wait for debounce time
    adc_key_in = analogRead(0);    // read the value from the sensor  
    key = get_key(adc_key_in);            // convert into key press
    if (key != oldkey)        
    {     
      oldkey = key;
      if (key >=0){
        result = 1;
        if (key == 4){
            mySerial.begin(19200);
        }
        else{

            //Select not pressed. Reseting Baud Rate from 
            
            lcd.clear();
            lcd.cursorTo(1,0);
            lcd.printIn("Reseting");
            lcd.cursorTo(2,0);
            lcd.printIn("Baud Rate");
            mySerial.begin(115200);
            result = writeTsBd(0x67);
            delay(1000);
            lcd.clear();
              if(result == 0){
                lcd.cursorTo(1,0);
                lcd.printIn("BaudRate Reset");
                lcd.cursorTo(2,0);
                lcd.printIn("Successful");
                digitalWrite(13,HIGH);
                delay(1000);
                digitalWrite(13,LOW);
                
                }
                else{
                  lcd.cursorTo(1,0);
                  lcd.printIn("Unknown Error");
                  digitalWrite(13,HIGH);
                  delay(1000);
                  digitalWrite(13,LOW);
              }
              while(1){}
  
          }
      }
    }
  
  }
  }

  result = 0;
  
//Check if Baud Rate is default (19200).
  lcd.clear();

  lcd.cursorTo(1,0);
  lcd.printIn("CheckingBaudRate");

  byte baudRateCheck = readTsBd(0x67);

  delay(1000);

  lcd.clear();
 
  if(errStatus == 0){
  lcd.cursorTo(1,0);
  lcd.printIn("Baud Rate");
  lcd.cursorTo(2,0);
  lcd.printIn("Check Complete");
  }
  else{
    lcd.cursorTo(1,0);
    lcd.printIn("Can't Connect");
    lcd.cursorTo(2,0);
    lcd.printIn("To Sensor");
    while(1){}
    digitalWrite(13,HIGH);
    delay(1000);
    digitalWrite(13,LOW);
  }

//Begin Sensor Testing


  lcd.clear();
  lcd.cursorTo(1,0);
  lcd.printIn("Begin Sensor");
  lcd.cursorTo(2,0);
  lcd.printIn("Testing");

  check = 0;
  myServo.write(950);
  delay (1000);
  result = readTsTRG(0x67);
  if (result == 0){
  }
  else{
    check = 1;
  }

  myServo.write(1300);
  delay (1000);
  result = readTsTRG(0x67);
  if (result == 1){
  }
  else if(result == 0){
    check = 1;
  }
  else{
    check = 1;
  }

  myServo.write(950);
  delay (1000);
  result = readTsTRG(0x67);
  if (result == 0){
  }
  else{
    check = 1;
  }

  lcd.clear();
  lcd.cursorTo(1,0);
  lcd.printIn("Test Result:");
  lcd.cursorTo(2,0);
  if( check == 0 ){
    lcd.printIn("Pass");
    digitalWrite(13,HIGH);
    delay(1000);
    digitalWrite(13,LOW);
    delay(2000);
  }
  else{
    lcd.printIn("Fail");
    digitalWrite(13,HIGH);
    delay(250);
    digitalWrite(13,LOW);
    delay(250);
    digitalWrite(13,HIGH);
    delay(250);
    digitalWrite(13,LOW);
    delay(250);
    digitalWrite(13,HIGH);
    delay(250);
    digitalWrite(13,LOW);
    delay(250);
  }

  while (pingTs(0x67) == 0){
  }


}

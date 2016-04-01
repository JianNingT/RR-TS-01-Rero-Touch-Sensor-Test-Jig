#include <SoftwareSerial.h>
#define TxMode LOW    // Master transmit to G15
#define RxMode HIGH   // Master receive from G15
#define CTRL 2
#define HEADER 0xFF

//Instruction
#define READ (byte) 0x02
#define WRITE (byte) 0x03
#define RESET (byte) 0x06

//Address
#define TRG (byte) 0x0B
#define Baud (byte) 0x04


//1. Write Baud Rate
//2. Read Baud Rate
//3. Read TRG
//4. Ping
//===============================================================================================================
//1.Write Baud Rate
//Input: ID
//Return: Baud Rate
//============================================================================================================
byte writeTsBd (byte ID)
{
  byte buf[8]; 

//Flush the RX buffer=================================  
  digitalWrite(CTRL,RxMode);
  while(mySerial.available()>0)
  {
    mySerial.read(); //Clear UART buffer
  }  
//Transmit to sensor===================================  
  digitalWrite(CTRL, TxMode);
  mySerial.write(HEADER);
  mySerial.write(HEADER);
  mySerial.write(ID);  //ID
  mySerial.write(0x04); //Length
  mySerial.write(WRITE); //Instruction
  mySerial.write(Baud); //Address
  mySerial.write(0x67); //Number of bytes
  byte checksum=0xFF-ID-0x04-WRITE-Baud-0x67;   
  mySerial.write(checksum); //Checksum

//Receive from sensor====================================
  digitalWrite(CTRL,RxMode);
  int nbyte= mySerial.readBytes(buf,6);

 if(nbyte==0)
   {
    errStatus=0xFF;
    return(0xFF);
  }
  else if (nbyte<6) //incomplete package recieved
  {
    errStatus=buf[4];
    return(0xFF);
  }
  else
  {
    errStatus=buf[4];
    return(0x00);
  }
}

//===============================================================================================================
//2.Read Baud Rate
//Input: ID
//Return: Baud Rate
//============================================================================================================
byte readTsBd (byte ID)
{
  byte buf[8]; 
//Flush the RX buffer=================================  
  digitalWrite(CTRL,RxMode);
  while(mySerial.available()>0)
  {
    mySerial.read(); //Clear UART buffer
  }
//Transmit to sensor===================================  
  digitalWrite(CTRL, TxMode);
  mySerial.write(HEADER);
  mySerial.write(HEADER);
  mySerial.write(ID); //ID
  mySerial.write(0x04); //Length
  mySerial.write(READ); //Instruction
  mySerial.write(Baud); //Address
  mySerial.write(0x01); //Number of bytes
  byte checksum=0xFF-ID-0x04-READ-Baud-0x01;
  mySerial.write(checksum); //checksum

 //Receive from sensor====================================
  digitalWrite(CTRL,RxMode);
  int nbyte= mySerial.readBytes (buf,7);

  if(nbyte==0)
  {
    errStatus=0xFF;
    return(0xFF);
  }
  else if (nbyte<7) //incomplete package recieved
  {
    errStatus=buf[4];
    return(0xFF);
  }
  else
  {
    errStatus=buf[4];
    if(buf[4]!=0) // if return status not zero (error occured)
    {
      return(0xFF); 
    }
    else
    {     
      return(buf[5]); //return BaudRate
    }
  }
}
//===============================================================================================================
//3.Read TRG
//Input: ID
//Return: TRG
//============================================================================================================
byte readTsTRG (byte ID)
{
  byte buf[8]; 
//Flush the RX buffer=================================  
  digitalWrite(CTRL,RxMode);
  while(mySerial.available()>0)
  {
    mySerial.read(); //Clear UART buffer
  }
//Transmit to sensor===================================  
  digitalWrite(CTRL, TxMode);
  mySerial.write(HEADER);
  mySerial.write(HEADER);
  mySerial.write(ID); //ID
  mySerial.write(0x04); //Length
  mySerial.write(READ); //Instruction
  mySerial.write(TRG); //Address
  mySerial.write(0x01); //Number of bytes
  byte checksum=0xFF-ID-0x04-READ-TRG-0x01;
  mySerial.write(checksum); //checksum

 //Receive from sensor====================================
  digitalWrite(CTRL,RxMode);
  int nbyte= mySerial.readBytes (buf,7);

  if(nbyte==0)
  {
    errStatus=0xFF;
    return(0xFF);
  }
  else if (nbyte<7) //incomplete package recieved
  {
    errStatus=buf[4];
    return(0xFF);
  }
  else
  {
    errStatus=buf[4];
    if(buf[4]!=0) // if return status not zero (error occured)
    {
      return(0xFF); 
    }
    else
    {     
      return(buf[5]); //return TRG
    }
  }
}

//===============================================================================================================
//4.Ping
//Input: ID
//Return:
//============================================================================================================
byte pingTs (byte ID)
{
  byte buf[8]; 

//Flush the RX buffer=================================  
  digitalWrite(CTRL,RxMode);
  while(mySerial.available()>0)
  {
    mySerial.read(); //Clear UART buffer
  }  
//Transmit to sensor===================================  
  digitalWrite(CTRL, TxMode);
  mySerial.write(HEADER);
  mySerial.write(HEADER);
  mySerial.write(ID);  //ID
  mySerial.write(0x02); //Length
  mySerial.write(0x01); //Instruction
  byte checksum=0xFF-ID-0x02-0x01;  
  mySerial.write(checksum); //Checksum

//Receive from sensor====================================
  digitalWrite(CTRL,RxMode);
  int nbyte= mySerial.readBytes(buf,6);

 if(nbyte==0)
   {
    errStatus=0xFF;
    return(0xFF);
  }
  else if (nbyte<6) //incomplete package recieved
  {
    errStatus=buf[4];
    return(0xFF);
  }
  else
  {
    errStatus=buf[4];
    return(0x00);
  }
}







// Convert ADC value to key number
int get_key(unsigned int input)
{
  int k;
    
  for (k = 0; k < NUM_KEYS; k++)
  {
    if (input < adc_key_val[k])
    {
           
    return k;
        }
  }
    
    if (k >= NUM_KEYS)
        k = -1;     // No valid key pressed
    
    return k;
}

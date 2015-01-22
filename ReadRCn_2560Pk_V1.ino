/*
project__2560 Read Receiver 4,6,8 CH
by: tinnakon kheowree  
tinnakon_za@hotmail.com
tinnakonza@gmail.com
http://quad3d-tin.lnwshop.com/
https://www.facebook.com/tinnakonza

date: 22-01-2558(2015)  V.1 ReadRCn_2560Pk_V1.ino

support:  Board 2560  
• Atmega2560
• RC Receiver 4,6,8 CH ,Futaba, stom i6, FlySky FS-i6 2.4G 6CH
  ,FlySky TH9x, 9XR,

----------rx-----------           
PORTK
#define THROTTLEPIN                1  //PIN 62 =  PIN A10
#define ROLLPIN                    2  //PIN 63 =  PIN A8
#define PITCHPIN                   0  //PIN 64 =  PIN A9
#define YAWPIN                     3  //PIN 65 =  PIN A11
#define AUX1PIN                    4  //PIN 66 =  PIN A12
#define AUX2PIN                    5  //PIN 67 =  PIN A13
#define AUX3PIN                    6  //PIN 68 =  PIN A14
#define AUX4PIN                    7  //PIN 69 =  PIN A15
*/
#include <Arduino.h>
#include "config.h"
#include "multi_rx2560.h"

void setup()
{
  Serial.begin(57600);//38400
  pinMode(13, OUTPUT);pinMode (30, OUTPUT);pinMode (31, OUTPUT);//pinMode (30, OUTPUT);pinMode (31, OUTPUT);//(13=A=M),(31=B=STABLEPIN),(30=C,GPS FIG LEDPIN)
  digitalWrite(13, HIGH);
  //Serial1.begin(115200);//CRIUS Bluetooth Module pin code 0000
  //Serial3.begin(38400);//3DR Radio Telemetry Kit 433Mhz
  configureReceiver();//find multi_rx.h
  delay(1000);
  RC_Calibrate();//"multi_rxPPM2560.h"
  Serial.print("TK_Quadrotor_Read Receiver");Serial.println("\t");
  previousTime = micros();
}
void loop()
{

   Dt_roop = micros() - previousTime;// 100 Hz task loop (10 ms)  , 5000 = 0.02626 ms
   if(Dt_roop <= 0)
   {
    Dt_roop = 10001; 
   }   
    if (Dt_roop >= 10000) 
    {
      previousTime = micros();
      G_Dt = Dt_roop*0.000001;
      frameCounter++;
 
 if (frameCounter % TASK_50HZ == 0)// 50 Hz tak (20 ms)
 {
  computeRC();//multi_rx.h
  //failsafeCnt++;
  //Fail_Safe();     
}//end roop 50 Hz 
         if (frameCounter % TASK_20HZ == 0)//roop print  ,TASK_5HZ  TASK_10HZ
        {
            Serial.print(CH_THR);Serial.print("\t");
            Serial.print(CH_AIL);Serial.print("\t");  
            Serial.print(CH_ELE);Serial.print("\t");
            Serial.print(CH_RUD);Serial.print("\t");  
            Serial.print(AUX_1);Serial.print("\t"); 
            Serial.print(AUX_2);Serial.print("\t"); 
            Serial.print(AUX_3);Serial.print("\t"); 
            Serial.print(AUX_4);Serial.print("\t"); 
            //Serial.print(failsafeCnt);Serial.print("\t");
      
            //Serial.print(CH_AILf);Serial.print("\t");  
            //Serial.print(CH_ELEf);Serial.print("\t");
            //Serial.print(CH_RUDf);Serial.print("\t");
            
            Serial.print(G_Dt*1000);Serial.print("\t");
            //Serial.print(millis()/1000.0);//millis() micros()
            Serial.print("\n"); 
        }//end roop 5 Hz 
        if (frameCounter >= TASK_1HZ) { // Reset frameCounter back to 0 after reaching 100 (1s)
            frameCounter = 0;
            //time_auto++;
            //Remote_TrimACC();//motor.h
              if(Status_LED == LOW)
            {
            Status_LED = HIGH;
            }
            else
            {
            Status_LED = LOW;
            }
            digitalWrite(13, Status_LED);//A
        }//end roop 1 Hz
    }//end roop 100 HZ 
}

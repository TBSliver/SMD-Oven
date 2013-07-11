#include <max6675.h>                                      //Thermocouple library
#include <PID_AutoTune_v0.h>

double aTuneStep=20, aTuneNoise=1, aTuneStartValue=100;
unsigned int aTuneLookBack=40;


double input = 60, output;                           //main variables
double kp = 1, ki = 0.5, kd = 4;                          //PID variables
int windowSize = 500;                                     //size of the relay PID window
unsigned long windowStartTime;

PID_ATune aTune(&input, &output);

byte thermoDO = 7;                                        //data out from the MAX6675
byte thermoCS = 8;                                        //chip select from the MAX6675
byte thermoCLK = 9;                                       //clock from the MAX6675
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);      //CLK, CS, DO

byte powerRelay = 6;                                      //output for the power relay

boolean ovenActive = true;

void setup() {
    pinMode(powerRelay, OUTPUT);                            //set relay pin to output
  digitalWrite(powerRelay, LOW);                          //set the output to low

Serial.begin(9600);
Serial.println("PID AutoTune");

output=aTuneStartValue;
    aTune.SetNoiseBand(aTuneNoise);
    aTune.SetOutputStep(aTuneStep);
    aTune.SetLookbackSec((int)aTuneLookBack);
    aTune.SetControlType(1);
}


void loop() {
  
  input = thermocouple.readCelsius();
  
  
    if(aTune.Runtime())
    { //we're done, set the tuning parameters
      kp = aTune.GetKp();
      ki = aTune.GetKi();
      kd = aTune.GetKd();
      
      Serial.print(" Kp = ");
      Serial.println(kp);
      Serial.print(" Ki = ");
      Serial.println(ki);
      Serial.print(" Kd = ");
      Serial.println(kd);
    } else {
      ovenRun();
    }
    

}


void ovenRun() {
    unsigned long now = millis();
    if(now - windowStartTime > windowSize)
    { //time to shift the Relay Window
      windowStartTime += windowSize;
    }
    if(output > now - windowStartTime)
      digitalWrite(powerRelay,HIGH);
    else digitalWrite(powerRelay,LOW);
}


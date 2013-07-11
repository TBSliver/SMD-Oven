//===========================================================================
//= HAC:Man SMD Oven Arduino Code - Written by TBSliver                     =
//===========================================================================
//= MAX6675 Library - Adafruit Industries                                   =
//= Arduino PID Library - Brett Beauregard                                  =
//= LiquidCrystal Library - Arduino                                         =
//= SD Card Library - Arduino                                               =
//= Button Debounce code - Adagruit Industries                              =
//=                                                                         =
//= The hardware and software for this device will be released under an     =
//= Open Source license, with the libraries under their respective license  =
//=                                                                         =
//= * SD card attached to SPI bus as follows:                               =
//=  * MOSI - pin 50                                                        =
//=  * MISO - pin 51                                                        =
//=  * CLK  - pin 52                                                        =
//=  * CS   - pin 53                                                        =
//= * LCD connected on the Analog pins (A0 - A5)                            =
//= * MAX6675 connected on pins 7, 8, 9                                     =
//= * Buttons on pins 2, 3, 4, 5                                            =
//= * Output Relay on pin 6                                                 =
//===========================================================================

//===========================================================================
//= Libraries                                                               =
//===========================================================================

#include <max6675.h>                                      //Thermocouple library
#include <LiquidCrystal.h>                                //LCD library
#include <SD.h>                                           //SD Card library
#include <PID_v1.h>                                       //PID library

//===========================================================================
//= PID variables                                                           =
//===========================================================================

double setpoint, input, output;                           //main variables
double kp = 2, ki = 5, kd = 1;                            //PID variables
int windowSize = 500;                                     //size of the relay PID window
unsigned long windowStartTime;                            //timer for the relay window
PID pid(&input, &output, &setpoint, kp, ki, kd, DIRECT);  //PID init

//===========================================================================
//= SD Card variables                                                       =
//===========================================================================

char root[] = "/";                                        //root of the SD Card
char fileName[13];                                        //space for the 8.3 filenaming convention
int curFile = 0;                                          //current file pointer in a folder
int noFiles = 0;                                          //number of files in the root directory
boolean isDir = false;                                    //is the current file a directory

//===========================================================================
//= Thermocouple variables                                                  =
//===========================================================================

byte thermoDO = 5;                                        //data out from the MAX6675
byte thermoCS = 6;                                        //chip select from the MAX6675
byte thermoCLK = 7;                                       //clock from the MAX6675
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);      //CLK, CS, DO

//===========================================================================
//= LCD Screen variables                                                    =
//===========================================================================

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);                //LCD on the Analog Pins
unsigned long lastRefresh = 0;                            //variable for refreshing thermocouple
uint8_t degree[8] = {                                     //make a degree symbol
    B01100,
    B10010,
    B10010,
    B01100,
    B00000,
    B00000,
    B00000,
    B00000
  };
  
//===========================================================================
//= Button Debounce variables                                               =
//===========================================================================

#define DEBOUNCE 10                                       //debounce time in milliseconds
byte buttons[] = {11, 12, 10, 9};                            //Enter, Stop, Prev, Next
#define NUMBUTTONS sizeof(buttons)                        //total number of buttons
byte pressed[NUMBUTTONS],                                 //button arrays
     justpressed[NUMBUTTONS],
     justreleased[NUMBUTTONS];


//===========================================================================
//= Oven variables                                                          =
//===========================================================================

byte powerRelay = 4;                                      //output for the power relay
boolean ovenActive = false;                               //is the oven actually active

//===========================================================================
//= Main setup                                                              =
//===========================================================================

void setup() {
  
  Serial.begin(9600);                                     //initialise serial port
  Serial.println("Welcome to the HAC:Man SMD Oven\nPlease wait while setting up...");
  
  delay(500);                                             //Let MAX chip stabilise
  
  lcdInit();                                              //Initialise the LCD
  
  sdInit();                                               //Initialise SD card
  
  pidInit();                                              //Initialise PID control
  
  buttonInit();                                           //Initialise Buttons
  
  ovenInit();                                             //Initialise Oven bits
  
  lcd.setCursor(0, 0);
  lcd.print("HAC:Man SMD Oven");
  lcd.setCursor(0, 1);
  lcd.print("Setting Up");
  
  
  
  //final setup
  Serial.println("Setup Complete");
  lcd.setCursor(0, 1);
  lcd.print("Setup Complete");
  
  delay(500);
  
  lcd.clear();
}

void loop() {

  check_switches();                                       //the button loop
  pid.Compute();                                          //the main PID loop
  menu();                                                 //the main menu loop
  ovenRun();                                              //the oven control loop
  
  delay(10);                                              //delay to keep the buttons working properly
}


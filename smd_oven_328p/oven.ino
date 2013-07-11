//===========================================================================
//= Oven Initialisation                                                     =
//===========================================================================

void ovenInit() {
  pinMode(powerRelay, OUTPUT);                            //set relay pin to output
  digitalWrite(powerRelay, LOW);                          //set the output to low
}

//===========================================================================
//= Main Oven PID routine for Relay thing                                   =
//===========================================================================

void ovenRun() {
  if(ovenActive) {
    unsigned long now = millis();
    if(now - windowStartTime > windowSize)
    { //time to shift the Relay Window
      windowStartTime += windowSize;
    }
    if(output > now - windowStartTime)
      digitalWrite(powerRelay,HIGH);
    else digitalWrite(powerRelay,LOW);
    }
}

//===========================================================================
//= Temp Settings Load                                                      =
//===========================================================================

void loadSettings() {
  EXROM.read(0, &preTime);
  EXROM.read(4, &soakTime);
  EXROM.read(8, &soakTemp);
  EXROM.read(12, &reflowTime);
  EXROM.read(16, &reflowTemp);
  EXROM.read(20, &kp);
  EXROM.read(24, &ki);
  EXROM.read(28, &kd);
  pid.SetTunings(kp,ki,kd);
}

//===========================================================================
//= Temp Settings Save                                                      =
//===========================================================================

void saveSettings() {
  EXROM.write(0, preTime);
  EXROM.write(4, soakTime);
  EXROM.write(8, soakTemp);
  EXROM.write(12, reflowTime);
  EXROM.write(16, reflowTemp);
  EXROM.write(20, kp);
  EXROM.write(24, ki);
  EXROM.write(28, kd);
}

unsigned long runRefresh = 0, runTime = 0;
byte curStage = 0;
double midPoint;

void runSMD() {
  printTemp(9,0);
  if (millis() < runRefresh) {
     // we wrapped around, lets just try again
     runRefresh = millis();
  }
  if((runRefresh + 400) < millis()) {
    runTime = millis() - startTime;
    lcd.setCursor(0,0);
    lcd.print("Stage:");
    switch(curStage) {
      case 0:
        lcd.setCursor(0,1);
        lcd.print("Pre Soak");
        input = readTemp();
        midPoint = roomTemp + (preSoakSlope * runTime);
        if(midPoint < soakTemp) {
          setpoint = midPoint;
        } else {
          setpoint = soakTemp;
        }
        lcd.setCursor(9,1);
        lcd.print(setpoint);
        if((input >= soakTemp) && (runTime >= (preTime*1000))) {
          startTime = millis();
          curStage++;
        }
        break;
      case 1:                                                                //Soak code
        lcd.setCursor(0,1);
        lcd.print("Soak Time");
        input = readTemp();
        setpoint = soakTemp;
        lcd.setCursor(9,1);
        lcd.print(setpoint);
        if((soakTime*1000) <= runTime) {
          startTime = millis();
          curStage++;
        }
        break;
      case 2:                                                                //Reflow Code - Need to change it so this only counts when its in the 'reflow zone'
        lcd.setCursor(0,1);
        lcd.print("Reflow");
        input = readTemp();
        setpoint = reflowTemp;
        lcd.setCursor(9,1);
        lcd.print(setpoint);
        if(((reflowTime*1000) <= runTime) || (reflowTemp <= readTemp())) {
          startTime = millis();
          curStage++;
        }
        break;
      case 3:
        lcd.setCursor(0,1);
        lcd.print("Cooling");
        pid.SetMode(MANUAL);
        output = 0;
        input = 0;
        setpoint = input;
        lcd.setCursor(9,1);
        lcd.print(setpoint);
        if((roomTemp + 50) >= readTemp()) {
          ovenActive = false;
          curStage = 0;
          menuOpt = 0;
          lastOpt = -1;
          subMenu = false;
        }
        break;
    }
    runRefresh = millis();
  }
}

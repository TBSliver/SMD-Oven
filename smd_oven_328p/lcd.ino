//===========================================================================
//= LCD Display Initialisation                                              =
//===========================================================================

void lcdInit() {
  lcd.begin(16, 2);                            //setup for a 16*2 LCD
  lcd.createChar(0, degree);                   //create a degree symbol from the byte definition
}

boolean subMenu = false, editOpt = false, editRefresh = false;
byte subOpt = 0, menuOpt = 0, lastOpt = -1, lastSubOpt = -1, maxMenu = 2, maxSubOpt = 7;

void menu() {
  
  menuButtons();
  
  if(!subMenu) {
    printTemp(9,0);
    if (lastOpt != menuOpt) {
      lcd.setCursor(0,0);
      lcd.print("-Menu-");
      clearLine(1);
      switch(menuOpt) {
        case 0:
          lcd.setCursor(0,1);
          lcd.print("<");
          lcd.setCursor(2,1);
          lcd.print("Edit Settings");
          lcd.setCursor(15,1);
          lcd.print(">");
          break;
        case 1:
          lcd.setCursor(0,1);
          lcd.print("<");
          lcd.setCursor(2,1);
          lcd.print("Save Settings");
          lcd.setCursor(15,1);
          lcd.print(">");
          break;
        case 2:
          lcd.setCursor(0,1);
          lcd.print("<");
          lcd.setCursor(2,1);
          lcd.print("Run");
          lcd.setCursor(15,1);
          lcd.print(">");
          break;
      }
      lastOpt = menuOpt;
    }
  }
  else {
    if((lastSubOpt != subOpt) || editRefresh) {
      clearLine(1);
      clearLine(0);
      switch (menuOpt) {
        case 0:
          if(editOpt) {
            lcd.setCursor(0,0);
            lcd.print("Edit:");
          } else {
            lcd.setCursor(0,0);
            lcd.print("-Edit Settings-");
          }
          switch(subOpt) {
            case 0:
              if(editOpt) {
                lcd.setCursor(0,1);
                lcd.print("-");
                lcd.setCursor(5,0);
                lcd.print("Soak Temp");
                lcd.setCursor(2,1);
                lcd.print(soakTemp);
                lcd.setCursor(15,1);
                lcd.print("+");
              } else {
                lcd.setCursor(0,1);
                lcd.print("<");
                lcd.setCursor(3,1);
                lcd.print("Soak Temp");
                lcd.setCursor(15,1);
                lcd.print(">");
              }
              break;
            case 1:
              if(editOpt) {
                lcd.setCursor(0,1);
                lcd.print("-");
                lcd.setCursor(5,0);
                lcd.print("Reflow Temp");
                lcd.setCursor(2,1);
                lcd.print(reflowTemp);
                lcd.setCursor(15,1);
                lcd.print("+");
              } else {
                lcd.setCursor(0,1);
                lcd.print("<");
                lcd.setCursor(3,1);
                lcd.print("Reflow Temp");
                lcd.setCursor(15,1);
                lcd.print(">");
              }
              break;
            case 2:
              if(editOpt) {
                lcd.setCursor(0,1);
                lcd.print("-");
                lcd.setCursor(5,0);
                lcd.print("Pre Time");
                lcd.setCursor(2,1);
                lcd.print(preTime);
                lcd.setCursor(15,1);
                lcd.print("+");
              } else {
                lcd.setCursor(0,1);
                lcd.print("<");
                lcd.setCursor(3,1);
                lcd.print("Presoak Time");
                lcd.setCursor(15,1);
                lcd.print(">");
              }
              break;
            case 3:
              if(editOpt) {
                lcd.setCursor(0,1);
                lcd.print("-");
                lcd.setCursor(5,0);
                lcd.print("Soak Time");
                lcd.setCursor(2,1);
                lcd.print(soakTime);
                lcd.setCursor(15,1);
                lcd.print("+");
              } else {
                lcd.setCursor(0,1);
                lcd.print("<");
                lcd.setCursor(3,1);
                lcd.print("Soak Time");
                lcd.setCursor(15,1);
                lcd.print(">");
              }
              break;
            case 4:
              if(editOpt) {
                lcd.setCursor(0,1);
                lcd.print("-");
                lcd.setCursor(5,0);
                lcd.print("Reflow Time");
                lcd.setCursor(2,1);
                lcd.print(reflowTime);
                lcd.setCursor(15,1);
                lcd.print("+");
              } else {
                lcd.setCursor(0,1);
                lcd.print("<");
                lcd.setCursor(3,1);
                lcd.print("Reflow Time");
                lcd.setCursor(15,1);
                lcd.print(">");
              }
              break;
            case 5:
              if(editOpt) {
                lcd.setCursor(0,1);
                lcd.print("-");
                lcd.setCursor(5,0);
                lcd.print("Kp");
                lcd.setCursor(2,1);
                lcd.print(kp);
                lcd.setCursor(15,1);
                lcd.print("+");
              } else {
                lcd.setCursor(0,1);
                lcd.print("<");
                lcd.setCursor(3,1);
                lcd.print("Kp");
                lcd.setCursor(15,1);
                lcd.print(">");
              }
              break;
            case 6:
              if(editOpt) {
                lcd.setCursor(0,1);
                lcd.print("-");
                lcd.setCursor(5,0);
                lcd.print("Ki");
                lcd.setCursor(2,1);
                lcd.print(ki);
                lcd.setCursor(15,1);
                lcd.print("+");
              } else {
                lcd.setCursor(0,1);
                lcd.print("<");
                lcd.setCursor(3,1);
                lcd.print("Ki");
                lcd.setCursor(15,1);
                lcd.print(">");
              }
              break;
            case 7:
              if(editOpt) {
                lcd.setCursor(0,1);
                lcd.print("-");
                lcd.setCursor(5,0);
                lcd.print("Kd");
                lcd.setCursor(2,1);
                lcd.print(kd);
                lcd.setCursor(15,1);
                lcd.print("+");
              } else {
                lcd.setCursor(0,1);
                lcd.print("<");
                lcd.setCursor(3,1);
                lcd.print("Kd");
                lcd.setCursor(15,1);
                lcd.print(">");
              }
              break;
          }
          editRefresh = false;
          break;
        case 1:
          lcd.setCursor(0,0);
          lcd.print("Saving Settings...");
          saveSettings();
          lcd.setCursor(0,1);
          lcd.print("Settings Saved");
          delay(500);
          subMenu = false;
          lastOpt = -1;
          break;
        case 2:
          ovenActive = true;
          pid.SetMode(AUTOMATIC);
          startTime = millis();
          preSoakSlope = ((soakTemp - roomTemp)/(preTime*1000));
          break;
      }
      lastSubOpt = subOpt;
    }
  }
}

void menuButtons() {
  if(justpressed[0]) {                              //Enter
    if(!subMenu) {
      subMenu = true;
      subOpt = 0;
      lastSubOpt = -1;
    } else {
      if(menuOpt == 0) {
        editOpt = true;
        editRefresh = true;
      }
    }
  }
  if(justpressed[1]) {                              //Back
    if(editOpt) {
      editOpt = false;
      lastSubOpt = -1;
      lcd.clear();
      pid.SetTunings(kp,ki,kd);
    } else {
      subMenu = false;
      lastOpt = -1;
      lcd.clear();
    }
  }
  if(justpressed[2]) {                              //Prev
    if(subMenu) {
      if(editOpt) {
        switch(subOpt) {
          case 0:
            soakTemp--;
            break;
          case 1:
            reflowTemp--;
            break;
          case 2:
            preTime--;
            break;
          case 3:
            soakTime--;
            break;
          case 4:
            reflowTime--;
            break;
          case 5:
            kp = kp--;
            break;
          case 6:
            ki = ki - 0.05;
            break;
          case 7:
            kd = kd--;
            break;
        }
        editRefresh = true;
      }
      else if(subOpt == 0)
        subOpt = maxSubOpt;
      else
        subOpt--;
    }
    else if(menuOpt == 0)
      menuOpt = maxMenu;
    else
      menuOpt--;
  }
  if(justpressed[3]) {                              //Next
    if(subMenu) {
      if(editOpt) {
        switch(subOpt) {
          case 0:
            soakTemp++;
            break;
          case 1:
            reflowTemp++;
            break;
          case 2:
            preTime++;
            break;
          case 3:
            soakTime++;
            break;
          case 4:
            reflowTime++;
            break;
          case 5:
            kp = kp++;
            break;
          case 6:
            ki = ki + 0.1;
            break;
          case 7:
            kd = kd++;
            break;
        }
        editRefresh = true;
      }
      else if(subOpt == maxSubOpt)
        subOpt = 0;
      else
        subOpt++;
    }
    else if(menuOpt == maxMenu)
      menuOpt = 0;
    else
      menuOpt++;
  }
}

void clearLine(int line) {
  lcd.setCursor(0,line);
  lcd.print("                ");
}

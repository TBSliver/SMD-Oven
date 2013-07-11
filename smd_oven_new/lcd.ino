//===========================================================================
//= LCD Display Initialisation                                              =
//===========================================================================

void lcdInit() {
  lcd.begin(16, 2);                            //setup for a 16*2 LCD
  lcd.createChar(0, degree);                   //create a degree symbol from the byte definition
}

boolean subMenu = false;
byte subOpt = 1, menuOpt = 0, lastOpt = -1, lastSubOpt = -1, maxMenu = 2;

void menu() {
  
  menuButtons();
  
  lcd.setCursor(0,0);
    if(ovenActive){
      lcd.print("Cur Temp: ");
      printTemp(10,0);
    } else lcd.print("  -Main Menu-   ");
  
  if (lastOpt != menuOpt) {
    clearLine(1);
    switch(menuOpt) {
      case 0:
        lcd.setCursor(0,1);
        lcd.print("<");
        lcd.setCursor(2,1);
        lcd.print("Load File");
        lcd.setCursor(15,1);
        lcd.print(">");
        break;
      case 1:
        lcd.setCursor(0,1);
        lcd.print("<");
        lcd.setCursor(2,1);
        lcd.print("Set Temp");
        lcd.setCursor(15,1);
        lcd.print(">");
        break;
      case 2:
        lcd.setCursor(0,1);
        lcd.print("<");
        lcd.setCursor(2,1);
        lcd.print("Settings");
        lcd.setCursor(15,1);
        lcd.print(">");
        break;
    }
    lastOpt = menuOpt;
  }
  
  if(subMenu) {
    if(lastSubOpt != subOpt) {
      clearLine(1);
      switch (menuOpt) {
        case 0:
          if(subOpt == 0) subOpt = noFiles;
          if(subOpt > noFiles) subOpt = 1;
          
          lcd.setCursor(0,1);
          lcd.print("<");
          lcd.setCursor(2,1);
          
          if(getFileName(root, subOpt - 1))
            lcd.print(fileName);
          else
            lcd.print("N");
          
          lcd.setCursor(15,1);
          lcd.print(">");
          break;

        case 1:
          lcd.setCursor(0,1);
          lcd.print("<");
          lcd.setCursor(2,1);
          lcd.print("Not Ready");
          lcd.setCursor(15,1);
          lcd.print(">");
          break;
        case 2:
          lcd.setCursor(0,1);
          lcd.print("<");
          lcd.setCursor(2,1);
          lcd.print("Not Ready");
          lcd.setCursor(15,1);
          lcd.print(">");
          break;
      }
      lastSubOpt = subOpt;
    }
  }
}

void menuButtons() {
  if(justpressed[0]) {
    if(!subMenu)
      subMenu = true;
      subOpt = 1;
      lastSubOpt = -1;
  }
  if(justpressed[1]) {
    subMenu = false;
    lastOpt = -1;
  }
  if(justpressed[2]) {
    if(subMenu)
      subOpt--;
    else if(menuOpt == 0)
      menuOpt = maxMenu;
    else
      menuOpt--;
  }
  if(justpressed[3]) {
    if(subMenu)
      subOpt++;
    else if(menuOpt == maxMenu)
      menuOpt = 0;
    else
      menuOpt++;
  }
}

void clearLine(int line) {
  lcd.setCursor(0,line);
  lcd.print("               ");
}

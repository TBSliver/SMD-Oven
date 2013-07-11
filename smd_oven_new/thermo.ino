//===========================================================================
//= MAX6675 read wrapper                                                    =
//===========================================================================

double readTemp() {
  return thermocouple.readCelsius();
}

//prints the current temperature at the specified place on an LCD. uses 7 spaces
void printTemp(byte tColumn, byte tRow) {
  if (millis() < lastRefresh) {
     // we wrapped around, lets just try again
     lastRefresh = millis();
  }
  if((lastRefresh + 1000) < millis()) {
    lcd.setCursor(tColumn, tRow);
    lcd.print(readTemp());
    lcd.write((byte)0);
    lcd.print("C");
    lastRefresh = millis();  
  }
}

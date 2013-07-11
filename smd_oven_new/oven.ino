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

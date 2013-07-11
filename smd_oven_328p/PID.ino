//===========================================================================
//= PID Initialisation                                                      =
//===========================================================================

void pidInit() {
  input = readTemp();                      //set the input to use the readTemp method
  setpoint = input;                        //set the PID to the current level
  windowStartTime = millis();              //start the timer for the PID window
  pid.SetOutputLimits(0, windowSize);      //set the PID output to the size of the window
  pid.SetSampleTime(pidSampleTime);        //set sample time
  pid.SetMode(MANUAL);                     //turn the PID off
}

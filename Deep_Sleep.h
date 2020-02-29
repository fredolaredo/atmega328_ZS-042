/**
 * Deep Sleep
 */
 
ISR(WDT_vect){
  //DON'T FORGET THIS!  Needed for the watch dog timer.  
  //This is called after a watch dog timer timeout - 
  //this is the interrupt function called after waking up
  Serial.println("watchdog wakeup !");
  wdt_disable(); // disable watchdog
}// watchdog interrupt

void goToSleep ()
{

  // disable ADC
  ADCSRA = 0;
  // clear various "reset" flags
  MCUSR = 0;
  // allow changes, disable reset
  //WDTCSR = bit (WDCE) | bit (WDE);
  // set interrupt mode and an interval
  //WDTCSR = bit (WDIE) | bit (WDP3) | bit (WDP0); // set WDIE, and 8 seconds delay
  //wdt_reset(); // reset the watchdog
  set_sleep_mode (SLEEP_MODE_PWR_DOWN);
  noInterrupts (); // timed sequence follows
  sleep_enable();
  // turn off brown‐out enable in software
  MCUCR = bit (BODS) | bit (BODSE);
  MCUCR = bit (BODS);
  interrupts (); // guarantees next instruction executed
  sleep_cpu ();
  // cancel sleep as a precaution
  sleep_disable();
}

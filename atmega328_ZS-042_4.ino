
#include "avr/sleep.h"
#include "avr/wdt.h"
#include "Deep_Sleep.h"


#define INTERRUPT_PIN 2
#define TIME_PIN 3
boolean digital_interrupt_enabled = true;
boolean time_interrupt_enabled = true;
#define POWER_PIN 8

#include "Wire.h"
#include "time.h"
#include "ZS042DEFS.h"

#define DS3231_I2C_ADDRESS     0x68
#define AT24C32_I2C_ADDRESS    0x57  // On-board 32k byte EEPROM; 128 pages of 32 bytes each

//////////////////////////////////////
// DIGITAL INTERRUPT
//////////////////////////////////////

void digitalInterrupt(){
  if (digital_interrupt_enabled) {
    Serial.println("Digital Interrupt !");
    digital_interrupt_enabled = false;
  } else {
    return;
  }
}

void timeInterrupt(){
  if (time_interrupt_enabled) {
    Serial.println("Time Interrupt !");
    time_interrupt_enabled = false;
  } else {
    return;
  }
}

//////////////////////////////////////
// SETUP 
//////////////////////////////////////

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println("Setup Started");

  Wire.begin();

  pinMode(INTERRUPT_PIN, INPUT);
  pinMode(POWER_PIN,OUTPUT);

  Serial.println(String("attachInterrupt ") + INTERRUPT_PIN);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), digitalInterrupt, FALLING);

  Serial.println(String("attachInterrupt ") + TIME_PIN);
  attachInterrupt(digitalPinToInterrupt(TIME_PIN), timeInterrupt, FALLING);

  digitalWrite(POWER_PIN,HIGH);

  /* init RTC dateTime with PC system time 
  char dateStr[32] = __TIME__;
  strcat(dateStr, " 06/01/20 6");
  Serial.println(dateStr);
  setRTCDateTimeStr(dateStr);
  */
  
  adjustTime(getRTCDateTime());
  
  Serial.println("Setup ended");
}

//////////////////////////////////////
// LOOP
//////////////////////////////////////

void loop() {

  
  if(!digital_interrupt_enabled) {
    digitalWrite(POWER_PIN,HIGH);
    Serial.println(getRTCDateTimeStr());
    getRTCStatus();
    digital_interrupt_enabled = true;
  }

  if(!time_interrupt_enabled) {
    digitalWrite(POWER_PIN,HIGH);
    Serial.println(getRTCDateTimeStr());
    setRTCAF(1);
    time_interrupt_enabled = true;
  }
  
  char rawData[65];
  if (Serial.available()) {
    size_t byteCount = Serial.readBytesUntil('\n', rawData, sizeof(rawData) - 1); //read in data to buffer
    rawData[byteCount] = NULL;//put an end character on the data
    Serial.print("> ");
    Serial.println(rawData);

    char command[] = "command for RTC";
    
    strncpy(command, "sleep", sizeof(command));
    if (strstr(rawData, command)) {
      Serial.println("Sleep mode");
      Serial.println(getRTCDateTimeStr());
      Serial.flush();
      digitalWrite(POWER_PIN,LOW);
      digital_interrupt_enabled = true;
      time_interrupt_enabled = true;
      goToSleep();
    }

    strncpy(command, "time", sizeof(command));
    if (strstr(rawData, command)) {
      digitalWrite(POWER_PIN,HIGH);
      char data[128];
      strncpy(data, &rawData[strlen(command)], sizeof(data));
      if(strlen(data) > 2) {
        Serial.println("data = " + String(data));
        Serial.println(timeToStr(atol(data)));
      } else {
        Serial.print("RTC : "); Serial.println(timeToStr(getRTCDateTime()));
        Serial.print("now : "); Serial.println(timeToStr(now()));
      }
    }

    strncpy(command, "set", sizeof(command));
    if (strstr(rawData, command)) {
      digitalWrite(POWER_PIN,HIGH);
      char data[128];
      strncpy(data, &rawData[strlen(command)], sizeof(data));
      if(strlen(data) > 2) {
        Serial.println("data = " + String(data));
        Serial.println(setRTCDateTimeStr(data));
      } else {
        Serial.println(getRTCDateTimeStr());
      }
    }

    strncpy(command, "unix", sizeof(command));
    if (strstr(rawData, command)) {
      digitalWrite(POWER_PIN,HIGH);
      char data[128];
      strncpy(data, &rawData[strlen(command)], sizeof(data));
      if(strlen(data) > 2) {
        Serial.print("data = "); Serial.println(atol(data));
        setRTCDateTime(atol(data));
      } 
      Serial.println(getRTCDateTime());
    }

    strncpy(command, "alarm1", sizeof(command));
    if (strstr(rawData, command)) {
      digitalWrite(POWER_PIN,HIGH);
      char data[128];
      strncpy(data, &rawData[strlen(command)], sizeof(data));
      if(strlen(data) > 2) {
        Serial.println("data = " + String(data));
        setRTCAlarm1Day(atol(data));
      }
      Serial.print("time   = "); Serial.println(timeToStr(getRTCDateTime()));
      Serial.print("alarm1 = "); Serial.println(timeToStr(getRTCAlarm1()));
    }

    strncpy(command, "a1mask", sizeof(command));
    if (strstr(rawData, command)) {
      digitalWrite(POWER_PIN,HIGH);
      char data[128];
      strncpy(data, &rawData[strlen(command)], sizeof(data));
      if(strlen(data) > 3) {
        Serial.println("data = " + String(data));
        setRTCAlarm1MaskStr(data);
      }
      Serial.println("Day Hrs Min Sec");
      Serial.println(getRTCAlarm1MaskStr());
    }
    
    strncpy(command, "temp", sizeof(command));
    if (strstr(rawData, command)) {
      digitalWrite(POWER_PIN,HIGH);
      Serial.print(getRTCTemp());
      Serial.println("°C");
    }

    strncpy(command, "control", sizeof(command));
    if (strstr(rawData, command)) {
      digitalWrite(POWER_PIN,HIGH);
      char data[128];
      strncpy(data, &rawData[strlen(command)], sizeof(data));
      if(strlen(data) > 2) {
        Serial.println("data = " + String(data));
        setRTCControl(data);
      } else {
        getRTCControl();
      }
    }

    strncpy(command, "status", sizeof(command));
    if (strstr(rawData, command)) {
      digitalWrite(POWER_PIN,HIGH);
      getRTCStatus();
    }
    
    strncpy(command, "EOSC", sizeof(command));
    if (strstr(rawData, command)) {
      digitalWrite(POWER_PIN,HIGH);
      char data[128];
      strncpy(data, &rawData[strlen(command)], sizeof(data));
      setRTCOESC(atoi(data));
      getRTCStatus();
    }

    strncpy(command, "BBSQW", sizeof(command));
    if (strstr(rawData, command)) {
      digitalWrite(POWER_PIN,HIGH);
      char data[128];
      strncpy(data, &rawData[strlen(command)], sizeof(data));
      setRTCBBSQW(atoi(data));
      getRTCControl();
    }

    strncpy(command, "CONV", sizeof(command));
    if (strstr(rawData, command)) {
      digitalWrite(POWER_PIN,HIGH);
      char data[128];
      strncpy(data, &rawData[strlen(command)], sizeof(data));
      setRTCCONV(atoi(data));
      getRTCControl();
    }

    strncpy(command, "RS", sizeof(command));
    if (strstr(rawData, command)) {
      digitalWrite(POWER_PIN,HIGH);
      char data[128];
      strncpy(data, &rawData[strlen(command)], sizeof(data));
      setRTCRS(atoi(data));
      getRTCControl();
    }

    strncpy(command, "INTCN", sizeof(command));
    if (strstr(rawData, command)) {
      digitalWrite(POWER_PIN,HIGH);
      char data[128];
      strncpy(data, &rawData[strlen(command)], sizeof(data));
      setRTCINTCN(atoi(data));
      getRTCControl();
    }

    strncpy(command, "A2IE", sizeof(command));
    if (strstr(rawData, command)) {
      digitalWrite(POWER_PIN,HIGH);
      char data[128];
      strncpy(data, &rawData[strlen(command)], sizeof(data));
      setRTCA2IE(atoi(data));
      getRTCControl();
    }

    strncpy(command, "A1IE", sizeof(command));
    if (strstr(rawData, command)) {
      digitalWrite(POWER_PIN,HIGH);
      char data[128];
      strncpy(data, &rawData[strlen(command)], sizeof(data));
      setRTCA1IE(atoi(data));
      getRTCControl();
    }

    strncpy(command, "OSF", sizeof(command));
    if (strstr(rawData, command)) {
      digitalWrite(POWER_PIN,HIGH);
      char data[128];
      strncpy(data, &rawData[strlen(command)], sizeof(data));
      setRTCOSF();
      getRTCStatus();
    }

    strncpy(command, "AF", sizeof(command));
    if (strstr(rawData, command)) {
      digitalWrite(POWER_PIN,HIGH);
      char data[64];
      strncpy(data, &rawData[strlen(command)], sizeof(data));
      setRTCAF(atoi(data));
      getRTCStatus();
    }

    strncpy(command, "E32K", sizeof(command));
    if (strstr(rawData, command)) {
      digitalWrite(POWER_PIN,HIGH);
      char data[64];
      strncpy(data, &rawData[strlen(command)], sizeof(data));
      setRTCE32K(atoi(data));
      getRTCStatus();
    }

    strncpy(command, "now", sizeof(command));
    if (strstr(rawData, command)) {
      digitalWrite(POWER_PIN,HIGH);
      Serial.println(__DATE__);
      Serial.println(__TIME__);
    }
    
  }
  
}

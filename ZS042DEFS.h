/**
 * ZS-042 RTC
 * 
 * DS3131 chip
 *
 */
#ifndef ZS042DEFS_H_
#define ZS042DEFS_H_

#define DS3231_I2C_ADDRESS     0x68
#define AT24C32_I2C_ADDRESS    0x57  // On-board 32k byte EEPROM; 128 pages of 32 bytes each

#include "TimeLib.h"
/*
  low level functions to convert to and from system time 
  void breakTime(time_t time, tmElements_t &tm);  // break time_t into elements
  time_t makeTime(const tmElements_t &tm);  // convert time elements into time_t
*/

typedef enum {
  DIMANCHE = 1,
  LUNDI,
  MARDI,
  MERCREDI,
  JEUDI,
  VENDREDI,
  SAMEDI
} WEEKDAYS;

typedef enum {
  A1F     = 1,         // Alarm 1 Flag
  A2F     = 2,         // Alarm 2 Flag (A2F). 1=time matched the alarm 2 registers
  BSY     = 4,         // Busy (BSY). This bit indicates the device is busy executing TCXO functions
  EN32kHz = 8,         // Enable 32kHz Output (EN32kHz).   
  OSF     = 128        // Oscillator Stop Flag (OSF). 1=the oscillator either is stopped or was stopped
} STATUS_FLAGS;

// DS3131 Registers
enum DS3131_REGS
{
  
  SECONDS_REG         = 0x00,
  MINUTES_REG         = 0x01,
  HOURS_REG           = 0x02,
  DAY_WEEK_REG        = 0x03,
  DAY_DATE_REG        = 0x04,
  MON_CENT_DATE_REG   = 0x05,
  YEAR_DATE_REG       = 0x06,
  DATE_REG            = SECONDS_REG,
  
  ALARM1_SECS_REG     = 0x07,
  ALAMR1_MINS_REG     = 0x08,
  ALAMR1_HRS_REG      = 0x09,
  ALARM1_DAY_REG      = 0x0A,
  ALARM1_REG          = ALARM1_SECS_REG,
  
  ALARM2_MINS_REG     = 0x0B,
  ALARM2_HOURS_REG    = 0x0C,
  ALARM2_DAY_DATE_REG = 0x0D,
  ALARM2_REG          = ALARM2_MINS_REG,
  
  CONTROL_REG         = 0x0E,
  CONTROL_STATUS_REG  = 0x0F,
  
  AGING_OFFSET_REG    = 0x10,
  
  TEMP_MSB_REG        = 0x11,
  TEMP_LSB_REG        = 0x12,
  TEMP_REG            = TEMP_MSB_REG
};

enum AMPM {
  AM,
  PM
};

enum RSFREQ {
  freq_1Hz     = 0, 
  freq_1024kHz = 1,
  freq_4096kHz = 2,
  freq_8192kHz = 3 
};

#pragma pack(push, 1)

struct SECONDSstruct {
  uint8_t secs        : 4; //bit 0-3
  uint8_t secs10      : 3; //bit 4-6
  uint8_t m1          : 1; //bit 7
};

struct MINUTESstruct {
  uint8_t mins        : 4; //bit 0-3
  uint8_t mins10      : 3; //bit 4-6
  uint8_t m2          : 1; //bit 7
};

struct HOURSstruct {
  uint8_t hrs         : 4; //bit 0-3
  uint8_t hrs10       : 1; //bit 4
  uint8_t ampm20h     : 1; // si h12=1 (1=PM, 0=AM)   si h12=0 (0=0, 1=2)
  uint8_t h12         : 1; // 1=12, 0=24
  uint8_t m3          : 1; //bit 7
};

struct DAY_WEEKstruct {
  uint8_t day_week    : 3; //bit 0-2
  uint8_t day_weekmsb : 5; //bit 3-7
};

struct DAY_DATEstruct {
  uint8_t date    : 4; //bit 0-3
  uint8_t date10  : 2; //bit 4-5
  uint8_t m4      : 2; //bit 6-7
};

struct MON_CENT_DATEstruct {
  uint8_t months    : 4;
  uint8_t months10  : 1;
  uint8_t monthsmsb : 2;
  uint8_t century   : 1;
};

struct YEAR_DATEstruct {
  uint8_t year      : 4;
  uint8_t year10    : 4;
};

typedef struct  {
  struct SECONDSstruct  SECONDS_bits;
  struct MINUTESstruct  MINUTES_bits;
  struct HOURSstruct    HOURS_bits;
  struct DAY_WEEKstruct DAY_WEEK_bits;
  struct DAY_DATEstruct DAY_DATE_bits;
  struct MON_CENT_DATEstruct MON_CENT_DATE_bits;
  struct YEAR_DATEstruct YEAR_DATE_bits;
} DATEstruct;


///////////////////
// ALARM structs
//////////////////

struct ALARM_DAY_DATEstruct {
  uint8_t day_date  : 4; //
  uint8_t date10    : 2; 
  uint8_t dydt      : 1;  // 1 = day, 0 = date
  uint8_t m4        : 1;
};

typedef struct {
  struct SECONDSstruct           SECONDS_bits;
  struct MINUTESstruct           MINUTES_bits;
  struct HOURSstruct             HOURS_bits;
  struct ALARM_DAY_DATEstruct    DAY_DATE_bits;
} ALARM1struct;

typedef struct {
  struct MINUTESstruct           MINUTES_bits;
  struct HOURSstruct             HOURS_bits;
  struct ALARM_DAY_DATEstruct    DAY_DATE_bits;
} ALARM2struct;


///////////////////
// CONTROL structs
//////////////////

typedef struct {
  uint8_t A1IE        : 1;
  uint8_t A2IE        : 1;
  uint8_t INTCN       : 1;
  uint8_t RS          : 2;  // Rate Select frequency of the square-wave : RS2 / RS1
                            // 00=1Hz, 01=1.024kHz, 10=4.096kHz, 11=8.192kHz 
  uint8_t CONV        : 1;  // Convert Temperature : 1 forces the temperature sensor with TCXO algorithm
  uint8_t BBSQW       : 1;  // Battery-Backed Square-Wave Enable : 1 with INTCN=0 = SQW enabled
  uint8_t EOSC        : 1;  // Enable Oscillator : 1 = started
} CONTROLstruct;

typedef struct {
  uint8_t A1F         : 1; // Alarm 1 Flag (A1F). 1=time matched the alarm 1 registers
  uint8_t A2F         : 1; // Alarm 2 Flag (A2F). 1=time matched the alarm 2 registers
  uint8_t BSY         : 1; // Busy (BSY). This bit indicates the device is busy executing TCXO functions
  uint8_t EN32kHz     : 1; // Enable 32kHz Output (EN32kHz).
  uint8_t msb         : 3;
  uint8_t OSF         : 1; // Oscillator Stop Flag (OSF). 1=the oscillator either is stopped or was stopped
} CONTROL_STATUSstruct;


///////////////////
// TEMP structs
//////////////////

typedef struct  {
  signed char data ;
} TEMP_MSBstruct;

typedef struct  {
  uint8_t zeros       : 6;
  uint8_t dot25       : 2;
} TEMP_LSBstruct;

typedef struct  {
  TEMP_MSBstruct MSB_bits;
  TEMP_LSBstruct LSB_bits;
} TEMPstruct ;

#pragma pack(pop)


//////////////////////////////////////
// timeToStr 
//////////////////////////////////////

char *timeToStr(time_t t){
  tmElements_t tm;
  breakTime(t, tm);
  static char timeStr[64];
  sprintf(timeStr, "%02d:%02d:%02d %02d/%02d/%4d %d", tm.Hour, tm.Minute, tm.Second, tm.Day, tm.Month, tm.Year + 1970, tm.Wday);
  return timeStr;
}

//////////////////////////////////////
// strToTime 
//////////////////////////////////////

time_t strToTime(char* str){
  tmElements_t tm;

  char delim[] = ": /";
  char values[6][5];
  char* token = strtok(str, delim);
  int i = 0;
  while (token != NULL) {
    strncpy(values[i], token, sizeof(values[i]));
    token = strtok(NULL, delim);
    i++;
  }

  tm.Hour   = atoi(values[0]);
  tm.Minute = atoi(values[1]);
  tm.Second = atoi(values[2]);
  tm.Day    = atoi(values[3]);
  tm.Month  = atoi(values[4]);
  tm.Year   = atoi(values[5]) + 30;
  
  return makeTime(tm);
}

//////////////////////////////////////
// readReg 
//////////////////////////////////////

void readReg(void* pReg, DS3131_REGS regAddr, int bytes){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(regAddr); // set DS3231 register pointer
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, bytes);
  for (int i=0; i<bytes ; i++) { 
    *(uint8_t *)(pReg + i) = Wire.read();
  }
}

//////////////////////////////////////
// readReg 1 byte
//////////////////////////////////////

void readReg(void* pReg, DS3131_REGS regAddr){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(regAddr); // set DS3231 register pointer
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 1);
  *(uint8_t *)(pReg) = Wire.read();
}

//////////////////////////////////////
// writeReg
//////////////////////////////////////

void writeReg(void* pReg, DS3131_REGS regAddr, int bytes){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(regAddr);
  for (int i=0; i<bytes ; i++) Wire.write(*(uint8_t *)(pReg + i));
  Wire.endTransmission();
}

//////////////////////////////////////
// writeReg 1 byte
//////////////////////////////////////

void writeReg(void* pReg, DS3131_REGS regAddr){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(regAddr);
  Wire.write(*(uint8_t *)(pReg));  
  Wire.endTransmission();
}


//////////////////////////////////////
// intToDecBin
//////////////////////////////////////

void intToDecBin (int v, int* v10, int* v1) {
  *v10 = (v - (v % 10)) / 10;
  *v1  = v % 10;
}

//////////////////////////////////////
// decOfInt
//////////////////////////////////////

uint8_t decOfInt (int v) {
  return (v - (v % 10)) / 10;
}

//////////////////////////////////////
// unitOfInt
//////////////////////////////////////

uint8_t unitOfInt (int v) {
  return (v % 10);
}

//////////////////////////////////////
// RTCStatus
//////////////////////////////////////

void getRTCStatus() {
  //SerialSerial.println("--> RTCStatus : display");
  CONTROL_STATUSstruct status;
  readReg(&status, CONTROL_STATUS_REG);
  Serial.print("A1F     ");
  Serial.print("A2F     ");
  Serial.print("BSY     ");
  Serial.print("EN32kHz ");
  Serial.print("msb     ");
  Serial.print("OSF     ");
  Serial.println("");
  Serial.print("  "      + String(status.A1F)) ;
  Serial.print("       " + String(status.A2F));
  Serial.print("       " + String(status.BSY) );
  Serial.print("       " + String(status.EN32kHz)  );
  Serial.print("       " + String(status.msb)  );
  Serial.print("       " + String(status.OSF));
  Serial.println("");
  
}

//////////////////////////////////////
// RTCControl display
//////////////////////////////////////

void getRTCControl() {
  //Serial.println("--> RTCControl : display");
  CONTROLstruct control;
  readReg(&control, CONTROL_REG, sizeof(control));
  
  Serial.print("EOSC   ");
  Serial.print("BBSQW ");
  Serial.print("CONV  ");
  Serial.print("RS    ");
  Serial.print("INTCN ");
  Serial.print("A2IE  ");
  Serial.print("A1IE  ");
  Serial.println("");
  Serial.print("  "    + String(control.EOSC)) ;
  Serial.print("     " + String(control.BBSQW));
  Serial.print("     " + String(control.CONV) );
  Serial.print("     " + String(control.RS));
  Serial.print("     " + String(control.INTCN));
  Serial.print("     " + String(control.A2IE) );
  Serial.print("     " + String(control.A1IE) );
  Serial.println("");
  
}

//////////////////////////////////////
// RTCControl set
//////////////////////////////////////

void setRTCControl(char* buf) {
  //Serial.println("--> RTCControl : set " + String(buf));
  CONTROLstruct control;
  readReg(&control, CONTROL_REG);

  char delim[] = ": /,";
  char valueParsed[7][10];
  char* token = strtok(buf, delim);
  int i = 0;
  while (token != NULL) {
    strncpy(valueParsed[i], token, sizeof(valueParsed[i]));
    token = strtok(NULL, delim);
    i++;
  }
 
  control.EOSC  = atoi(valueParsed[0]);
  control.BBSQW = atoi(valueParsed[1]);
  control.CONV  = atoi(valueParsed[2]);
  control.RS    = atoi(valueParsed[3]);
  control.INTCN = atoi(valueParsed[4]);
  control.A2IE  = atoi(valueParsed[5]);
  control.A1IE  = atoi(valueParsed[6]);

  writeReg(&control, CONTROL_REG);
}

//////////////////////////////////////
// checkStatus
//////////////////////////////////////

boolean checkStatus(STATUS_FLAGS statusFlag) {
  //Serial.print("--> checkRTCStatus : "); Serial.println(statusFlag);
  //CONTROL_STATUSstruct status;
  byte status;  
  readReg(&status, CONTROL_STATUS_REG);
  boolean result = status & statusFlag; 
  return result;
}

//////////////////////////////////////
// RTCTemp
//////////////////////////////////////

float getRTCTemp() {
  TEMPstruct temp;
  
  /*TEMP_MSBstruct MSBbits;
  TEMP_LSBstruct LSBbits;
  readReg(&temp, TEMP_REG, sizeof(temp));
  
  readReg(&MSBbits, TEMP_MSB_REG);
  readReg(&LSBbits, TEMP_LSB_REG);
  temp.MSB_bits = MSBbits;
  temp.LSB_bits = LSBbits;
  */
  
  /*
  Serial.print("MSB = "); Serial.println(MSBbits.data, BIN);
  Serial.print("LSB = "); Serial.println(LSBbits.dot25, BIN);
  Serial.print(temp.MSB_bits.data + temp.LSB_bits.dot25 * 0.25);
  Serial.println("°C");
  */
  
  //if ( temp.MSB_bits.data + (temp.LSB_bits.dot25 * 0.25) < 30) {
    return (temp.MSB_bits.data + (temp.LSB_bits.dot25 * 0.25));
  //} else {
  //  return (temp.MSB_bits.data + (temp.LSB_bits.dot25 * 0.25) - 20);
  //}
}

//////////////////////////////////////
// getRTCDate display
//////////////////////////////////////

time_t getRTCDateTime() {
  //Serial.println("--> RTCDateTime : display");
  
  DATEstruct date;
  readReg(&date, DATE_REG, sizeof(date));
  tmElements_t tm;
  
  /*Serial.print(date.HOURS_bits.ampm20h); Serial.print("-"); Serial.print(date.HOURS_bits.hrs10); Serial.print("-"); 
  Serial.print(date.HOURS_bits.hrs); Serial.print(":"); 
  Serial.print(date.MINUTES_bits.mins10); Serial.print("-"); Serial.print(date.MINUTES_bits.mins); Serial.print(":"); 
  Serial.print(date.SECONDS_bits.secs10); Serial.print("-"); Serial.println(date.SECONDS_bits.secs);Serial.print("-");
  Serial.println(date.HOURS_bits.h12);
  
  Serial.print(date.DAY_DATE_bits.date10); Serial.print("-"); Serial.print(date.DAY_DATE_bits.date); Serial.print("/"); 
  Serial.print(date.MON_CENT_DATE_bits.months10); Serial.print("-"); Serial.print(date.MON_CENT_DATE_bits.months); Serial.print("/");
  Serial.print(date.YEAR_DATE_bits.year10);Serial.print("-"); Serial.println(date.YEAR_DATE_bits.year);
  Serial.println(date.DAY_WEEK_bits.day_week);*/
  
  if (date.HOURS_bits.h12) {
    tm.Hour = date.HOURS_bits.hrs10 * 10 + date.HOURS_bits.hrs;
  } else {
    tm.Hour = date.HOURS_bits.ampm20h * 20 + date.HOURS_bits.hrs10 * 10 + date.HOURS_bits.hrs;
  }
  tm.Minute = date.MINUTES_bits.mins10 * 10 + date.MINUTES_bits.mins;
  tm.Second = date.SECONDS_bits.secs10 * 10 + date.SECONDS_bits.secs ;

  tm.Wday = date.DAY_WEEK_bits.day_week;
  
  tm.Day = date.DAY_DATE_bits.date10 * 10 + date.DAY_DATE_bits.date;
  tm.Month = date.MON_CENT_DATE_bits.months10 * 10 + date.MON_CENT_DATE_bits.months;
  
  tm.Year = date.YEAR_DATE_bits.year10 * 10 + date.YEAR_DATE_bits.year + 30 ;

  time_t t = makeTime(tm);

  /*Serial.print("time : "); Serial.print(tm.Hour); Serial.print(":"); Serial.print(tm.Minute);Serial.print(":"); Serial.println(tm.Second);
  Serial.print("date : "); Serial.print(tm.Day); Serial.print("/"); Serial.print(tm.Month);Serial.print("/"); Serial.println(tm.Year - 30);
  Serial.print("unix : "); Serial.println(t);*/
  
  return t;
}


//////////////////////////////////////
// RTCDateTime set
//////////////////////////////////////

time_t setRTCDateTime(time_t t = now()) {
  //Serial.print("--> RTCDateTime set : time_t "); Serial.println(t);
  
  DATEstruct date;
  readReg(&date, DATE_REG, sizeof(date));
  tmElements_t tm;

  breakTime(t, tm);

  /*
  Serial.print("time : "); Serial.print(tm.Hour); Serial.print(":"); Serial.print(tm.Minute);Serial.print(":"); 
  Serial.println(tm.Second);
  Serial.print("date : "); Serial.print(tm.Day); Serial.print("/"); Serial.print(tm.Month);Serial.print("/"); 
  Serial.println(tmYearToY2k(tm.Year));
  Serial.print("unix : "); Serial.println(t);
  */
  
  if (date.HOURS_bits.h12) {
    date.HOURS_bits.hrs10 = decOfInt(tm.Hour);
    date.HOURS_bits.hrs   = unitOfInt(tm.Hour);
  } else {
    date.HOURS_bits.ampm20h = tm.Hour >= 20 ? 1 : 0;
    date.HOURS_bits.hrs10   = decOfInt(tm.Hour);
    date.HOURS_bits.hrs     = unitOfInt(tm.Hour);
  }
  date.MINUTES_bits.mins10 = decOfInt(tm.Minute);
  date.MINUTES_bits.mins   = unitOfInt(tm.Minute);
  date.SECONDS_bits.secs10 = decOfInt(tm.Second);
  date.SECONDS_bits.secs   = unitOfInt(tm.Second);
  date.DAY_WEEK_bits.day_week = tm.Wday;
  date.DAY_DATE_bits.date10 = decOfInt(tm.Day);
  date.DAY_DATE_bits.date   = unitOfInt(tm.Day);
  date.MON_CENT_DATE_bits.months10 = decOfInt(tm.Month);
  date.MON_CENT_DATE_bits.months   = unitOfInt(tm.Month);
  date.YEAR_DATE_bits.year10 = decOfInt(tmYearToY2k(tm.Year));
  date.YEAR_DATE_bits.year   = unitOfInt(tmYearToY2k(tm.Year));

  //writeReg(&date, DATE_REG, sizeof(date));
  writeReg(&date.SECONDS_bits , SECONDS_REG);
  writeReg(&date.MINUTES_bits , MINUTES_REG);
  writeReg(&date.HOURS_bits , HOURS_REG);
  writeReg(&date.DAY_WEEK_bits , DAY_WEEK_REG);
  writeReg(&date.DAY_DATE_bits , DAY_DATE_REG);
  writeReg(&date.MON_CENT_DATE_bits , MON_CENT_DATE_REG);
  writeReg(&date.YEAR_DATE_bits , YEAR_DATE_REG);
  
  return makeTime(tm);
}

//////////////////////////////////////
// RTCDateTimeStr display
//////////////////////////////////////

char *getRTCDateTimeStr () {
  //Serial.println("--> RTCDateTimeStr display");
  
  time_t t = getRTCDateTime();
  setTime(t);
  tmElements_t tm;
  
  breakTime(t, tm);

  static char str[] = "HH:MM:SS dd/mm/yyy day:day";
  sprintf(str,"%02d:%02d:%02d %02d/%02d/%02d day:%d", 
          tm.Hour, tm.Minute, tm.Second, tm.Day, tm.Month, tm.Year + 1970, tm.Wday);
     
  return str;
}



//////////////////////////////////////
// setRTCDateTimeStr set
//////////////////////////////////////

time_t setRTCDateTimeStr (char *timeStr) {
  //Serial.println("--> RTCDateTimeStr set : "); Serial.println(timeStr);
  
  DATEstruct date;
  tmElements_t tm;

  readReg(&date, DATE_REG, sizeof(date));

  char delim[] = ": /";
  char values[7][10];
  char* token = strtok(timeStr, delim);
  int i = 0;
  while (token != NULL) {
    strncpy(values[i], token, sizeof(values[i]));
    token = strtok(NULL, delim);
    i++;
  }

  tm.Hour   = atoi(values[0]);
  tm.Minute = atoi(values[1]);
  tm.Second = atoi(values[2]);
  tm.Day    = atoi(values[3]);
  tm.Month  = atoi(values[4]);
  tm.Year   = atoi(values[5]) + 30;

  return setRTCDateTime(makeTime(tm));
  
}

//////////////////////////////////////
// RTCAF
//////////////////////////////////////

void setRTCAF(uint8_t num) {
  //Serial.println("--> RTCAF : acknowledge alarm : " + String(num));
  CONTROL_STATUSstruct status;
  readReg(&status, CONTROL_STATUS_REG, sizeof(status));
  if (num == 1) status.A1F = 0;
  if (num == 2) status.A2F = 0;
  writeReg(&status, CONTROL_STATUS_REG, sizeof(status));
}

//////////////////////////////////////
// RTCOSF
//////////////////////////////////////

void setRTCOSF() {
  //Serial.println("--> RTCOSF : set " + String(val));
  CONTROL_STATUSstruct control;
  readReg(&control, CONTROL_STATUS_REG, sizeof(control));
  control.OSF  = 0;
  writeReg(&control, CONTROL_STATUS_REG, sizeof(control));
}

//////////////////////////////////////
// RTCEOSC
//////////////////////////////////////

void setRTCOESC(uint8_t val) {
  //Serial.println("--> RTOESC : set " + String(val));
  CONTROLstruct control;
  readReg(&control, CONTROL_REG, sizeof(control));
  control.EOSC  = val;
  writeReg(&control, CONTROL_REG, sizeof(control));
}

//////////////////////////////////////
// RTCBBSQW
//////////////////////////////////////

void setRTCBBSQW(uint8_t val) {
  //Serial.println("--> RTCBBSQW : set " + String(val));
  CONTROLstruct control;
  readReg(&control, CONTROL_REG, sizeof(control));
  control.BBSQW  = val;
  writeReg(&control, CONTROL_REG, sizeof(control));
}

//////////////////////////////////////
// RTCCONV
//////////////////////////////////////

void setRTCCONV(uint8_t val) {
  //Serial.println("--> RTCCONV : set " + String(val));
  CONTROLstruct control;
  readReg(&control, CONTROL_REG, sizeof(control));
  control.CONV  = val;
  writeReg(&control, CONTROL_REG, sizeof(control));
}

//////////////////////////////////////
// RTCRS
//////////////////////////////////////

void setRTCRS(uint8_t val) {
  //Serial.println("--> RTCRS : set " + String(val));
  CONTROLstruct control;
  readReg(&control, CONTROL_REG, sizeof(control));
  control.RS  = val;
  writeReg(&control, CONTROL_REG, sizeof(control));
}

//////////////////////////////////////
// RTCINTCN
//////////////////////////////////////

void setRTCINTCN(uint8_t val) {
  //Serial.println("--> RTCINTCN : set " + String(val));
  CONTROLstruct control;
  readReg(&control, CONTROL_REG, sizeof(control));
  control.INTCN  = val;
  writeReg(&control, CONTROL_REG, sizeof(control));
}

//////////////////////////////////////
// RTCA2IE
//////////////////////////////////////

void setRTCA2IE(uint8_t val) {
  //Serial.println("--> RTCA2IE : set " + String(val));
  CONTROLstruct control;
  readReg(&control, CONTROL_REG, sizeof(control));
  control.A2IE  = val;
  writeReg(&control, CONTROL_REG, sizeof(control));
}

//////////////////////////////////////
// RTCE32K
//////////////////////////////////////

void setRTCE32K(uint8_t val) {
  //Serial.println("--> RTCE32K : set " + String(val));
  CONTROL_STATUSstruct status;
  readReg(&status, CONTROL_STATUS_REG, sizeof(status));
  status.EN32kHz  = val;
  writeReg(&status, CONTROL_STATUS_REG, sizeof(status));
}

//////////////////////////////////////
// RTCA1IE
//////////////////////////////////////

void setRTCA1IE(uint8_t val) {
  //Serial.println("--> RTCA1IE : set " + String(val));
  CONTROLstruct control;
  readReg(&control, CONTROL_REG, sizeof(control));
  control.A1IE  = val;
  writeReg(&control, CONTROL_REG, sizeof(control));
}

//////////////////////////////////////
// getRTCAlarm1 display
//////////////////////////////////////

time_t getRTCAlarm1() {
  //Serial.println("--> getRTCAlarm1 : display");
  ALARM1struct alarm;
  tmElements_t tm;
  readReg(&alarm, ALARM1_REG, sizeof(alarm));
  
  if (alarm.DAY_DATE_bits.dydt) {
    tm.Wday = alarm.DAY_DATE_bits.day_date;
  } else {
    tm.Day = alarm.DAY_DATE_bits.date10 * 10 + alarm.DAY_DATE_bits.day_date;
  }
  
  if (alarm.HOURS_bits.h12) {
    tm.Hour = alarm.HOURS_bits.hrs10 * 10 + alarm.HOURS_bits.hrs;
  } else {
    tm.Hour = alarm.HOURS_bits.ampm20h * 2 + alarm.HOURS_bits.hrs10 * 10 + alarm.HOURS_bits.hrs;
  }
  
  tm.Minute = alarm.MINUTES_bits.mins10 * 10 + alarm.MINUTES_bits.mins;
  tm.Second = alarm.SECONDS_bits.secs10 * 10 + alarm.SECONDS_bits.secs;

  return (makeTime(tm));
}

//////////////////////////////////////
// setRTCAlarm1
//////////////////////////////////////

time_t setRTCAlarm1Day(time_t t = now()) {
  //Serial.print("--> setRTCAlarm1 set : time_t "); Serial.println(t);
  
  ALARM1struct alarm1;
  readReg(&alarm1, ALARM1_REG, sizeof(alarm1));
  tmElements_t tm;

  if (t < now()) t += now();        // if t in the past then t considered seconds from now 

  breakTime(t, tm);
  
  alarm1.HOURS_bits.h12 = false;
  alarm1.HOURS_bits.ampm20h = tm.Hour >= 20 ? 1 : 0;
  alarm1.HOURS_bits.hrs10   = decOfInt(tm.Hour);
  alarm1.HOURS_bits.hrs     = unitOfInt(tm.Hour);
  
  alarm1.MINUTES_bits.mins10 = decOfInt(tm.Minute);
  alarm1.MINUTES_bits.mins   = unitOfInt(tm.Minute);
  
  alarm1.SECONDS_bits.secs10 = decOfInt(tm.Second);
  alarm1.SECONDS_bits.secs   = unitOfInt(tm.Second);

  alarm1.DAY_DATE_bits.day_date = decOfInt(tm.Day);
  alarm1.DAY_DATE_bits.day_date = unitOfInt(tm.Day);

  alarm1.DAY_DATE_bits.dydt = 1;

  writeReg(&alarm1.SECONDS_bits , ALARM1_SECS_REG);
  writeReg(&alarm1.MINUTES_bits , ALAMR1_MINS_REG);
  writeReg(&alarm1.HOURS_bits   , ALAMR1_HRS_REG);
  writeReg(&alarm1.DAY_DATE_bits, ALARM1_DAY_REG);
  
  return makeTime(tm);
}

//////////////////////////////////////
// RTCAlarm1 Mask set
//////////////////////////////////////

void setRTCAlarm1MaskStr (char* dateString) {   // "dydt hh mm ss"
  //Serial.println("--> RTCAlarm1Mask set : " + String(dateString));
  ALARM1struct alarm;
  readReg(&alarm, ALARM1_REG, sizeof(alarm));
  
  char delim[] = ": /";
  char valueParsed[4][10];
  char* token = strtok(dateString, delim);
  int i = 0;
  while (token != NULL) {
    strncpy(valueParsed[i], token, sizeof(valueParsed[i]));
    token = strtok(NULL, delim);
    i++;
  }

  alarm.DAY_DATE_bits.m4   = atoi(valueParsed[0]);
  alarm.HOURS_bits.m3      = atoi(valueParsed[1]);
  alarm.MINUTES_bits.m2    = atoi(valueParsed[2]);
  alarm.SECONDS_bits.m1    = atoi(valueParsed[3]);
  
  writeReg(&alarm, ALARM1_REG, sizeof(alarm));

}

//////////////////////////////////////
// getRTCAlarm1Str Mask get
//////////////////////////////////////

char *getRTCAlarm1MaskStr () {   // "hh mm ss dydt"
  //Serial.println("--> setRTCAlarm1Mask get ");
  ALARM1struct alarm;
  readReg(&alarm, ALARM1_REG, sizeof(alarm));
  static char str[12];
  sprintf(str, "%d   %d   %d   %d", alarm.DAY_DATE_bits.m4, alarm.HOURS_bits.m3, alarm.MINUTES_bits.m2, alarm.SECONDS_bits.m1);
  return str;
}




#endif /* ZS042DEFS_H_ */

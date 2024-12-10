// Need libraries
// Adafruit_dotstar
// Encoder
// Adafruit_SSD1327
// FlashStorage_SAMD

// Interrupt pins
// Encoders
// 10 - Int4
// 11 - Int5
// A5 - Int6
// 7  - Int2
// PBs
// 0 - Int0
// 1 - Int1
// Exts
// 2 - Int7
// 9 - Int3

// debug
#define debug true
#define debug_showpos false // Encoder position (care when turning on)
#define debug_curr false // Tracking current range
#define debug_displaytime true // Tracking display update time
#define debug_ext false // External trigger debug
#define debug_slowrite false // Debug power updates
#define debug_pwr_disp true // Debug power updates on display

// Pins
const byte I0 = A2; // Current sensing
const byte I1 = A3;
const byte Ext0 = 2; // External trigger
const byte Ext1 = 9;
const byte Enc0 = 10;
const byte Enc1 = 11;
const byte Enc2 = A5;
const byte Enc3 = 7;
const byte sysG = 5; // System good
const byte PB0 = 0; // Push button
const byte PB1 = 1; // Push button
const byte onboardled = 13;

const byte Placeholder1 = 12; // To remove
const byte Placeholder2 = A4; // To remove

// Power
#define maxpower 4095 // Just a constant
#define displaypower true
byte powerinc = 16;
int16_t power0 = 0;
int16_t power1 = 0;
uint16_t power0_old = 9999;
uint16_t power1_old = 9999;
volatile uint16_t Aval0, Aval1;
bool sendpower0 = true;
bool clearpower0 = false;
bool sendpower1 = true;
bool clearpower1 = false;
uint8_t updatepower0 = 0; // 0 - no update, 1 - update 1 digit, 2 - update 2 digits, 3 - update 3 digits, 4 - update 4 digits
uint8_t updatepower1 = 0;
bool updatepwrdisp0 = true;
bool updatepwrdisp1 = true;

// Current sense
const float currentmod = 0.1893; // Conversion factor to current
uint32_t curr0, curr0_sense, curr1, curr1_sense;
uint16_t mA0, mA0_old, mA1, mA1_old;
unsigned long int t0, tnow;
uint32_t tcurr = 100; // in ms
uint8_t isense = 0;
uint8_t nsense = 4; // Sense 4 and average
uint8_t updatecurrent0 = 1; // 0 - no update, 1 - update 1 digit, 2 - update 2 digits, 3 - update 3 digits
uint8_t updatecurrent1 = 1;

// Upper and lower bounds of analogWrite
const uint16_t pwr0 = 500; 
const uint16_t pwr1 = 4095;

// Turn off neopixel
#include <Adafruit_DotStar.h>
#include <SPI.h>
#define NUMPIXELS 1 // Number of LEDs in strip
#define DATAPIN 8
#define CLOCKPIN 6
Adafruit_DotStar strip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

// OLED
#include <Adafruit_SSD1327.h>
Adafruit_SSD1327 display(128, 128, &Wire, -1, 1000000);

// UI
#define offset0 20
#define offset1 40
#define midline 63

// Encoder
#include <Encoder.h>
Encoder myEnc0(Enc1, Enc0);
Encoder myEnc1(Enc3, Enc2);
int32_t pos, pos_old, pos1, pos1_old;

// Buttons
volatile bool eeprom_update0 = false;
volatile bool eeprom_update1 = false;

// EEprom
#include <FlashStorage_SAMD.h>
#define S0add  0
#define P0add0 1
#define P0add1 2
#define S1add  3
#define P1add0 4
#define P1add1 5
#define eeprom_sig 42 // Signature
bool commitnow = false;

// Serial
char m, n;


void setup() {
  // put your setup code here, to run once:
  pinMode(I0, INPUT);
  pinMode(I1, INPUT);

  // To remove
  pinMode(Placeholder1, INPUT);
  pinMode(Placeholder2, INPUT);

  // External input
  pinMode(Ext0, INPUT);
  pinMode(Ext1, INPUT);
  attachInterrupt(Ext0, ext0_int, CHANGE);
  attachInterrupt(Ext1, ext1_int, CHANGE);
  
  pinMode(PB0, INPUT);
  pinMode(PB1, INPUT);
  attachInterrupt(digitalPinToInterrupt(PB0), PB0_int, FALLING);
  attachInterrupt(digitalPinToInterrupt(PB1), PB1_int, FALLING);
  
  pinMode(sysG, OUTPUT);
  pinMode(onboardled, OUTPUT);

  #if debug
    Serial.begin(9600);
  #endif

  // Turn off dot star
  strip.begin(); // Initialize pins for output
  strip.setPixelColor(0, 0x000000); // black
  strip.show();  // Turn all LEDs off ASAP

  // Encoder initialization
  encoder_ini();

  // Display initialization
  dispini();

  // Timer initialization
  tnow = millis();
  t0 = tnow;

  // Analog initialization (apparently very important, otherwise they cross talk for some reason)
  analogWrite(A0, 0);
  analogWrite(A1, 0);
  
  // EEPROM initialization
  eeprom_ini();
  
  // System initialized
  digitalWrite(sysG, HIGH);
}

void loop() {
  // Timers
  tnow = millis();

  // Current processing
  if ((tnow - t0) > tcurr){
    t0 = tnow;
    isense++;
    curr0_sense = curr0_sense + analogRead(I0);
    curr1_sense = curr1_sense + analogRead(I1);
    
    if (isense >= nsense){
      // Report and clear
      curr0 = curr0_sense / 4;
      curr1 = curr1_sense / 4;
      curr0_sense = 0;
      curr1_sense = 1;
      isense = 0;

      // Determine if update
      checkcurrentupdate();
      mA0_old = mA0;
      mA1_old = mA1;
    }
  }
  
  // Encoder
  encoder_parse0();
  encoder_parse1();

  
  // Send power (outside interrupt)
  if (sendpower0){
    send_power(Aval0, false);
    sendpower0 = false;
  }
  else if (clearpower0){
    clear_power(false);
    clearpower0 = false;
  }
  if (sendpower1){
    send_power(Aval1, true);
    sendpower1 = false;
  }
  else if (clearpower1){
    clear_power(true);
    clearpower1 = false;
  }

  // Display current
  if ((updatecurrent0 > 0) || (updatecurrent1 > 0)){
    currenttext(updatecurrent0, updatecurrent1);
    updatecurrent0 = 0;
    updatecurrent1 = 0;
  }

  // Display power
  #if displaypower
    if (updatepwrdisp0){
      checkpowerupdate(false);
      power0_old = power0;
    }
    if (updatepwrdisp1){
      checkpowerupdate(true);
      power1_old = power1;
    }
    if (updatepwrdisp0 || updatepwrdisp1){
      powertext(updatepower0, updatepower1);
      updatepwrdisp0 = false;
      updatepwrdisp1 = false;
      updatepower0 = 0;
      updatepower1 = 0;
    }
  #endif
  
  // EEPROM
  if (eeprom_update0){
    eep_commit(false);
    eeprom_update0 = false;
  }
  if (eeprom_update1){
    eep_commit(true);
    eeprom_update1 = false;
  }
  
  // Debug serial
  #if debug
    if (Serial.available() > 1){
      m = Serial.read();
      n = Serial.read();
      parseserial();
    }
  #endif
  
}

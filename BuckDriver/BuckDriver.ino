// debug
#define debug true
#define debug_showanalogread false
#define debug_showpos false
#define committeeprom true

// Pins
const byte I0 = A2; // Current sensing
const byte I1 = A3;
const byte ExtA0 = A4; // External analog
const byte ExtA1 = A5;
const byte sysG = 2; // System good
const byte ALED = 7; // LED status for analog internal or not
const byte ASwitch = 9; // Switch analog internal or external
const byte PB0 = 10; // Push button
const byte Enc1 = 11;
const byte Enc0 = 12;
const byte onboardled = 13;

// Power
#define maxpower 4095 // Just a constant

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

// Power
const float gain = 94.1;
const uint16_t curr_res = 100;
const uint16_t conv = gain * 4096 * curr_res / 3300 / 10;
const uint16_t pwr0 = 4095; // Lower bound, higher values = lower power
const uint16_t pwr1 = 1000; // Higher bound, higher values = lower power

bool sendpower0 = true;
bool sendpower1 = true;

// UI
#define offset0 5
#define offset1 25
#define offset2 47
#define midline 63
const byte offsetvec[4] = {offset0, offset1, offset0+midline, offset1+midline};
bool onoff0 = true;
bool onoff1 = false;
int16_t power0 = 0;
int16_t power1 = 0;
int16_t current0 = 0;
int16_t current1 = 0;
byte powerinc = 16; // Power increment, lower values means higher resolution but takes longer to change large values.

// UI redraw flags
bool redraw = false;
bool redraw_cursor = false;
bool redraw_box = false;
bool redraw_current0 = false;
bool redraw_current1 = false;
bool redraw_powerbar0 = false;
bool redraw_powerbar1 = false;
bool redraw_onoff0 = false;
bool redraw_onoff1 = false;

// Encoder
#include <Encoder.h>
Encoder myEnc(Enc1, Enc0);
int32_t pos, pos_old;
int8_t isel = 0;
bool lsel = false;

// Button
bool b, bold, block;

// EEprom
#include <FlashStorage_SAMD.h>
#define L0add 0
#define P0add0 1
#define P0add1 2
#define L1add 3
#define P1add0 4
#define P1add1 5
bool onoff0_ee, onoff1_ee;
uint16_t power0_ee, power1_ee;
bool commitnow = false;

// Times
unsigned long int t0, t1;
uint32_t tcurr = 500; // in ms


void setup() {
  // put your setup code here, to run once:
  pinMode(I0, INPUT);
  pinMode(I1, INPUT);
  pinMode(ExtA0, INPUT);
  pinMode(ExtA1, INPUT);
  pinMode(ASwitch, INPUT_PULLUP);
  pinMode(PB0, INPUT);
  
  pinMode(sysG, OUTPUT);
  pinMode(ALED, OUTPUT);

  // EEprom initialize
  eeprom_ini();

  // Turn off dot star
  strip.begin(); // Initialize pins for output
  strip.setPixelColor(0, 0x000000); // black
  strip.show();  // Turn all LEDs off ASAP

  // OLED
  display.begin(0x3D);
  display.setTextColor(SSD1327_WHITE);
  display.setTextSize(2);
  display.clearDisplay();
  display.display();

  // Line
  display.fillRect(0, midline-1, 127, 3, SSD1327_WHITE);

  // Menu text
  Menutext(true);
  Menutext(false);
  
  // Upper UI
  currenttext(current0, true);
  powerbar(power0, true);
  onofftext(onoff0, true);

  // Lower UI
  currenttext(current1, false);
  powerbar(power1, false);
  onofftext(onoff1, false);

  drawcursor(isel);
  drawbox(isel, lsel);
  
  display.display();
  
  #if debug
    Serial.begin(9600);
  #endif

  // Encoder initialize
  encoder_ini();

  // Timers
  t0 = millis();
  t1 = t0;

  analogWrite(A0, maxpower);
  analogWrite(A1, maxpower);
  digitalWrite(sysG, HIGH);
}

void loop() {
  // Timers
  t1 = millis();

  // Parse button
  button_parse();
  
  // Parse encoder
  encoder_parse();

  // Get current
  if ((t1 - t0) >= tcurr){
    sample_curr();
    t0 = t1;
  }

  // Cursor
  if (redraw_cursor){
    drawcursor(isel);
    redraw_cursor = false;
  }

  // Box update
  if (redraw_box){
    drawbox(isel, lsel);  
    redraw_box = false;  
  }

  // ON/OFF
  if (redraw_onoff0){
    onofftext(onoff0, true);
    redraw_onoff0 = false;
  }
  if (redraw_onoff1){
    onofftext(onoff1, false);
    redraw_onoff1 = false;
  }

  // Power bar
  if (redraw_powerbar0){
    powerbar(power0, true);
    redraw_powerbar0 = false;
  }
  if (redraw_powerbar1){
    powerbar(power1, false);
    redraw_powerbar1 = false;
  }

  // Current
  if (redraw_current0){
    currenttext(current0, true);
    redraw_current0 = false;
  }
  if (redraw_current1){
    currenttext(current1, false);
    redraw_current1 = false;
  }

  // Send power
  if (sendpower0){
    if (onoff0){
      send_power(power0, true);
    }
    else{
      send_power(0, true);
    }
    sendpower0 = false;
  }
  if (sendpower1){
    if (onoff1){
      send_power(power1, false);
    }
    else{
      send_power(0, false);
    }
    sendpower1 = false;
  }
  
  // Display refresh
  if (redraw){
    redraw = false;
    display.display();
  }

  // Commit eeprom
  if (commitnow){
    if (committeeprom){
      EEPROM.commit();
      #if debug
        Serial.println("EEPROM committed");
      #endif
    }
    commitnow = false;
    #if debug
      Serial.println("Commit flag cleared.");
    #endif
  }

  // Serial debug
  #if debug
    serial_debug();
  #endif
  
  delayMicroseconds(1000);
}

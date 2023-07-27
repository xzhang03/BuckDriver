

void drawcursor(byte ind){
  display.fillRect(0, offset0, 9, 38, SSD1327_BLACK);
  display.fillRect(0, offset0+midline, 9, 38, SSD1327_BLACK);
  display.fillCircle(4, offsetvec[ind]+7, 4, SSD1327_WHITE);
}

void drawbox(byte ind, bool s){
  for (byte i = 0; i < 4; i++){
    if ((i == ind) && s){
      display.drawRect(10, offsetvec[i]-2, 12*2+2, 19, SSD1327_WHITE);
    }
    else{
      display.drawRect(10, offsetvec[i]-2, 12*2+2, 19, SSD1327_BLACK);
    }
  }
}

void Menutext(bool upper){
  uint8_t voffset = offset0;
  if (!upper){
    voffset = voffset + midline;
  }
  display.fillRect(0, voffset, 12*3, 14, SSD1327_BLACK);
  display.setCursor(0, voffset);
  display.print(" L");
  display.print(1-upper);

  voffset = offset1;
  if (!upper){
    voffset = voffset + midline;
  }
  display.fillRect(0, voffset, 12*3, 14, SSD1327_BLACK);
  display.setCursor(0, voffset);
  display.print(" mA");
}

void onofftext(bool on, bool upper){
  uint8_t voffset = offset0;
  if (!upper){
    voffset = voffset + midline;
  }
  display.fillRect(12*5, voffset, 12*4, 16, SSD1327_BLACK);
  display.setCursor(12*5, voffset);
  if (on){
    display.setTextColor(SSD1327_BLACK, SSD1327_WHITE); 
    display.print("ON");
    display.setTextColor(SSD1327_WHITE); 
  }
  else{
    display.print("OFF");
  }
}

void currenttext(uint16_t current, bool upper){
  uint8_t voffset = offset1;
  float mA = current * curr_res * 1.0 / conv;

  #if (debug) && (debug_showanalogread)
    Serial.print("Analog reading: ");
    Serial.print(current);
    Serial.print(" Current: ");
    Serial.println(mA, 2);
  #endif
  
  if (!upper){
    voffset = voffset + midline;
  }
  display.fillRect(12*5, voffset, 12*5, 14, SSD1327_BLACK);
  display.setCursor(12*5, voffset);
  display.print(mA, 1);
}

void powerbar(uint16_t power, bool upper){
  uint8_t voffset = offset2;
  if (!upper){
    voffset = voffset + midline;
  }
  
  display.fillRect(12,voffset, 116, 14, SSD1327_BLACK);
  display.fillRect(12,voffset, power * 116 / maxpower, 14, SSD1327_WHITE);
}

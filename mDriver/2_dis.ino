void dispini(void){
  // OLED
  while(!display.begin(0x3D)){
    delay(5);
    Serial.println("Unable to initialize OLED");
  }
  display.setTextColor(SSD1327_WHITE);
  display.setTextSize(2);
  display.clearDisplay();
  display.display();

  // Line
  display.fillRect(0, midline-1, 127, 3, SSD1327_WHITE);

  // Menu text
  Menutext(false);
  Menutext(true);
    
  display.display();
}

void Menutext(bool lower){
  uint8_t voffset = offset0;
  if (lower){
    voffset = voffset + midline;
  }
  display.fillRect(0, voffset, 12*3, 14, SSD1327_BLACK);
  display.setCursor(0, voffset);
  display.print(" L");
  display.print(lower);

  voffset = offset1;
  if (lower){
    voffset = voffset + midline;
  }
  display.fillRect(0, voffset, 12*3, 14, SSD1327_BLACK);
  display.setCursor(0, voffset);
  display.print(" mA");
}

void checkcurrentupdate(void){
  // Determine if update current 0
  mA0 = curr0 * currentmod;
  mA1 = curr1 * currentmod;
  
  if (mA0 == mA0_old){
    // No update
    updatecurrent0 = 0;
  }
  else if (mA0_old < 10){
    // Update 1 digit
    updatecurrent0 = 1;
  }
  else if (mA0_old < 100){
    // Update 2 digit
    updatecurrent0 = 2;
  }
  else{
    updatecurrent0 = 3;
  }
  
  // Determine if update current 1
  if (mA1 == mA1_old){
    // No update
    updatecurrent1 = 0;
  }
  else if (mA1_old < 10){
    // Update 1 digit
    updatecurrent1 = 1;
  }
  else if (mA1_old < 100){
    // Update 2 digit
    updatecurrent1 = 2;
  }
  else{
    updatecurrent1 = 3;
  }
  
  #if (debug) && (debug_curr)
    Serial.print("Ai0: ");
    Serial.print(curr0);
    Serial.print(" Prev: ");
    Serial.print(curr0_old);
    Serial.print(" Update: ");
    Serial.println(updatecurrent0);
    
    Serial.print("Ai1: ");
    Serial.print(curr1);
    Serial.print(" Prev: ");
    Serial.print(curr1_old);
    Serial.print(" Update: ");
    Serial.println(updatecurrent1);
  #endif
}

void currenttext(uint8_t updatecurr0, uint8_t updatecurr1){
  uint8_t voffset = offset1;
  uint32_t t0_math, t1_math;
  
  // ============== L0 current ==============
  if (updatecurr0 > 0){
    #if (debug) && (debug_displaytime)
      t0_math = micros();
    #endif
  
    display.fillRect(12*5, voffset, 12*updatecurr0, 14, SSD1327_BLACK); //SSD1327_BLACK
    display.setCursor(12*5, voffset);
    display.print(mA0, 1);

    #if (debug) && (debug_displaytime)
      t1_math = micros();
      Serial.print("mA0: ");
      Serial.println(mA0);
      Serial.print("L0 update time 1: ");
      Serial.println(t1_math - t0_math);
    #endif
  }
  

  // ============== L1 current ==============
  if (updatecurr1 > 0){
    #if (debug) && (debug_displaytime)
      t0_math = micros();
    #endif
    
    display.fillRect(12*5, voffset + midline, 12*updatecurr1, 14, SSD1327_BLACK); //SSD1327_BLACK
    display.setCursor(12*5, voffset + midline);
    display.print(mA1, 1);
    #if (debug) && (debug_displaytime)
      t1_math = micros();
      Serial.print("mA1: ");
      Serial.println(mA1);
      Serial.print("L1 update time 1: ");
      Serial.println(t1_math - t0_math);
    #endif
  }

  #if (debug) && (debug_displaytime)
    t0_math = micros();
  #endif
  display.display();
  #if (debug) && (debug_displaytime)
    t1_math = micros();
    Serial.print("Display update time 1: ");
    Serial.println(t1_math - t0_math);
  #endif
}

void checkpowerupdate(bool lower){
  // Determine if update power 0
  if (!lower){  
    if (power0 == power0_old){
      // No update
      updatepower0 = 0;
    }
    else if (power0_old < 10){
      // Update 1 digit
      updatepower0 = 1;
    }
    else if (power0_old < 100){
      // Update 2 digit
      updatepower0 = 2;
    }
    else if (power0_old < 1000){
      // Update 3 digit
      updatepower0 = 3;
    }
    else{
      // Update 4 digits
      updatepower0 = 4;
    }

    #if (debug) && (debug_pwr_disp)
      if (updatepower0 > 0){
        Serial.print("Power0: ");
        Serial.print(power0);
        Serial.print(" Prev: ");
        Serial.print(power0_old);
        Serial.print(" Update: ");
        Serial.println(updatepower0);
      }
    #endif
  }
  else{
    if (power1 == power1_old){
      // No update
      updatepower1 = 0;
    }
    else if (power1_old < 10){
      // Update 1 digit
      updatepower1 = 1;
    }
    else if (power1_old < 100){
      // Update 2 digit
      updatepower1 = 2;
    }
    else if (power1_old < 1000){
      // Update 3 digit
      updatepower1 = 3;
    }
    else{
      // Update 4 digits
      updatepower1 = 4;
    }

    #if (debug) && (debug_pwr_disp)
      if (updatepower1 > 0){
        Serial.print("Power1: ");
        Serial.print(power1);
        Serial.print(" Prev: ");
        Serial.print(power1_old);
        Serial.print(" Update: ");
        Serial.println(updatepower1);
      }
    #endif
  }
}

void powertext(uint8_t updatepwr0, uint8_t updatepwr1){
  uint8_t voffset = offset0;
  uint32_t t0_math, t1_math;
  
  // ============== L0 power ==============
  if (updatepwr0 > 0){
    #if (debug) && (debug_displaytime)
      t0_math = micros();
    #endif
  
    display.fillRect(12*5, voffset, 12*updatepwr0, 14, SSD1327_BLACK); //SSD1327_BLACK
    display.setCursor(12*5, voffset);
    display.print(power0, 1);

    #if (debug) && (debug_displaytime)
      t1_math = micros();
      Serial.print("Power0: ");
      Serial.println(power0);
      Serial.print("Power0 update time 1: ");
      Serial.println(t1_math - t0_math);
    #endif
  }
  
  // ============== L1 power ==============
  if (updatepwr1 > 0){
    #if (debug) && (debug_displaytime)
      t0_math = micros();
    #endif
    
    display.fillRect(12*5, voffset + midline, 12*updatepwr1, 14, SSD1327_BLACK); //SSD1327_BLACK
    display.setCursor(12*5, voffset + midline);
    display.print(power1, 1);
    #if (debug) && (debug_displaytime)
      t1_math = micros();
      Serial.print("Power1: ");
      Serial.println(power1);
      Serial.print("Power1 update time 1: ");
      Serial.println(t1_math - t0_math);
    #endif
  }

  #if (debug) && (debug_displaytime)
    t0_math = micros();
  #endif
  display.display();
  #if (debug) && (debug_displaytime)
    t1_math = micros();
    Serial.print("Display update time 1: ");
    Serial.println(t1_math - t0_math);
  #endif
}

void encoder_ini(void){
  // Positions
  pos = 0;
  pos_old = 0;

  // Buttons
  b = true;
  bold = true;
  block = false;
}

void button_parse(void){
  // Read
  b = digitalRead(PB0);

  // Parse
  if (b){
    // Unpushed
    if (block){
      // Remove button lock
      block = false;
    }
  }
  else{
    if ((!bold) && (!block)){
      // Also pushed previous cycle and no block
      lsel = !lsel;
      block = true;
      redraw_box = true;
      redraw = true;

      // Update encoder
      if (lsel){
        // Selected
        switch (isel){
          case 0:
            if (onoff0){
              pos = 4;
            }
            else{
              pos = 0;
            }
            break;
          case 1:
            pos = power0 / powerinc;
            break;
          case 2:
            if (onoff1){
              pos = 4;
            }
            else{
              pos = 0;
            }
            break;
          case 3:
            pos = power1 / powerinc;
            break;
        }
      }
      else{
        // Not selected
        pos = isel;

        // Update eeprom
        switch (isel){
          case 0:
            if (onoff0 != onoff0_ee){
              onoff0_ee = onoff0;
              if (onoff0_ee){
                EEPROM.update(L0add, 255);
              }
              else{
                EEPROM.update(L0add, 0);
              }
              commitnow = true;
              
              #if debug
                Serial.print("Updating ");
                Serial.print(L0add);
                Serial.print(" to ");
                Serial.println(onoff0_ee);
              #endif
            }
            break;
            
          case 1:
            if (power0 != power0_ee){
              power0_ee = power0;
              EEPROM.update(P0add0, (uint8_t) (power0_ee >> 8));
              EEPROM.update(P0add1, (uint8_t) (power0_ee & 0xFF));
              commitnow = true;
              
              #if debug
                Serial.print("Updating ");
                Serial.print(P0add0);
                Serial.print(" ");
                Serial.print(P0add1);
                Serial.print(" to ");
                Serial.println(power0_ee);
              #endif
            }
            break;
            
          case 2:
            if (onoff1 != onoff1_ee){
              onoff1_ee = onoff1;
              if (onoff1_ee){
                EEPROM.update(L1add, 255);
              }
              else{
                EEPROM.update(L1add, 0);
              }
              commitnow = true;
              
              #if debug
                Serial.print("Updating ");
                Serial.print(L1add);
                Serial.print(" to ");
                Serial.println(onoff1_ee);
              #endif
            }
            break;
            
          case 3:
            if (power1 != power1_ee){
              power1_ee = power1;
              EEPROM.update(P1add0, (uint8_t) (power1_ee >> 8));
              EEPROM.update(P1add1, (uint8_t) (power1_ee & 0xFF));
              commitnow = true;
              
              #if debug
                Serial.print("Updating ");
                Serial.print(P1add0);
                Serial.print(" ");
                Serial.print(P1add1);
                Serial.print(" to ");
                Serial.println(power1_ee);
              #endif
            }
            break;
        }
      }        
      // Position write
      pos_old = pos;
      myEnc.write(pos * 4);
    }
  }

  bold = b;
}

void encoder_parse(void){
  pos = myEnc.read() / 4;

  if (pos != pos_old) {
    pos_old = pos;
    #if (debug && debug_showpos)
      Serial.println(pos);
    #endif
  }
  else{
    return;
  }
  
  if (lsel){
    // button selection is on
    switch (isel){
      case 0:
        onoff0 = !onoff0;
        redraw_onoff0 = true;
        redraw = true;
        sendpower0 = true;
        break;
        
      case 1:
        if (pos < 0){
          pos = 0;
          pos_old = 0;
          power0 = 0;
          myEnc.write(0);
        }
        else if ((pos * powerinc) >= maxpower){
          pos = maxpower / powerinc;
          pos_old = maxpower / powerinc;
          power0 = maxpower;
          myEnc.write(maxpower * 4 / powerinc);
        }
        else{
          power0 = pos * powerinc;
        }
        redraw_powerbar0 = true;
        redraw = true;
        sendpower0 = true;
        break;
        
      case 2:
        onoff1 = !onoff1;
        redraw_onoff1 = true;
        redraw = true;
        sendpower1 = true;
        break;
        
      case 3:
        if (pos < 0){
          pos = 0;
          pos_old = 0;
          power1 = 0;
          myEnc.write(0);
        }
        else if ((pos * powerinc) >= maxpower){
          pos = maxpower / powerinc;
          pos_old = maxpower / powerinc;
          power1 = maxpower;
          myEnc.write(maxpower * 4 / powerinc);
        }
        else{
          power1 = pos * powerinc;
        }
        redraw_powerbar1 = true;
        redraw = true;
        sendpower1 = true;
        break;
    }
  }
  else{
    // button selection is off (just navigating UI);
    isel = pos;
    while (isel > 3){
      isel = isel - 4;
    }
    while (isel < 0){
      isel = isel + 4;
    }
    redraw_cursor = true;
    redraw = true;
  }
}

void encoder_ini(void){
  // Positions
  pos = 0;
  pos_old = 0;
  pos1 = 0;
  pos1_old = 0;
}

void encoder_parse0(void){
  pos = myEnc0.read() / 4;

  if (pos != pos_old) {
    pos_old = pos;
  }
  else{
    return;
  }
  
  if (pos < 0){
    pos = 0;
    pos_old = 0;
    power0 = 0;
    myEnc0.write(0);
  }
  else if ((pos * powerinc) >= maxpower){
    pos = maxpower / powerinc;
    pos_old = maxpower / powerinc;
    power0 = maxpower;
    myEnc0.write(maxpower * 4 / powerinc);
  }
  else{
    power0 = pos * powerinc;
  }

  Aval0 = map(power0, 0, 4095, pwr0, pwr1);
  sendpower0 = digitalRead(Ext0);
  clearpower0 = !sendpower0;
  #if displaypower
    updatepwrdisp0 = true;
  #endif
  
  #if (debug && debug_showpos)
    Serial.print("Enc0: ")
    Serial.print(pos);
    Serial.print(" ");
    Serial.println(power0);
  #endif
}

void encoder_parse1(void){
  pos1 = myEnc1.read() / 4;

  if (pos1 != pos1_old) {
    pos1_old = pos1;
  }
  else{
    return;
  }
  
  if (pos1 < 0){
    pos1 = 0;
    pos1_old = 0;
    power1 = 0;
    myEnc1.write(0);
  }
  else if ((pos1 * powerinc) >= maxpower){
    pos1 = maxpower / powerinc;
    pos1_old = maxpower / powerinc;
    power1 = maxpower;
    myEnc1.write(maxpower * 4 / powerinc);
  }
  else{
    power1 = pos1 * powerinc;
  }

  Aval1 = map(power1, 0, 4095, pwr0, pwr1);
  sendpower1 = digitalRead(Ext1);
  clearpower1 = !sendpower1;
  #if displaypower
    updatepwrdisp1 = true;
  #endif
  
  #if (debug && debug_showpos)
    Serial.print("Enc1: ")
    Serial.print(pos1);
    Serial.print(" ");
    Serial.println(power1);
  #endif
}

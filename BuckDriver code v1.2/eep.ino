void eeprom_ini(void){
  #if debug
    Serial.print(F("\nStart EEPROM_read on ")); Serial.println(BOARD_NAME);
    Serial.println(FLASH_STORAGE_SAMD_VERSION);
  
    Serial.print("EEPROM length: ");
    Serial.println(EEPROM.length());
  #endif

  // Read L0
  if (EEPROM.read(L0add) == 255){
    onoff0_ee = true;
  }
  else{
    onoff0_ee = false;
  }
  onoff0 = onoff0_ee;
  
  // Read L1
  if (EEPROM.read(L1add) == 255){
    onoff1_ee = true;
  }
  else{
    onoff1_ee = false;
  }
  onoff1 = onoff1_ee;

  // Read power0
  power0_ee = (EEPROM.read(P0add0) << 8) | (EEPROM.read(P0add1));
  if (power0_ee > maxpower){
    power0_ee = maxpower;
  }
  else{
    power0 = power0_ee;
  }

  // Read power1
  power1_ee = (EEPROM.read(P1add0) << 8) | (EEPROM.read(P1add1));
  if (power1_ee > maxpower){
    power1_ee = maxpower;
  }
  else{
    power1 = power1_ee;
  }
}

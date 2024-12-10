void eeprom_ini(void){
  uint16_t power0_ee, power1_ee, enc0_ee, enc1_ee;

  // =============== Read power0 ===============
  uint16_t sig0_ee = EEPROM.read(S0add);
  if (sig0_ee == eeprom_sig){
    // EEPROM signature checks out
    power0_ee = (EEPROM.read(P0add0) << 8) | (EEPROM.read(P0add1));
    if (power0_ee > maxpower){
      power0_ee = maxpower;
    }

    // Power 0
    power0 = power0_ee;
    pos = power0_ee / powerinc;
    Aval0 = map(power0, 0, 4095, pwr0, pwr1);

    // Encoder 0
    enc0_ee = pos * 4;
    myEnc0.write(enc0_ee);

    // Send or not
    sendpower0 = digitalRead(Ext0);
    clearpower0 = !sendpower0;
  }

  // =============== Read power1 ===============
  uint16_t sig1_ee = EEPROM.read(S1add);
  if (sig1_ee == eeprom_sig){
    // EEPROM signature checks out
    power1_ee = (EEPROM.read(P1add0) << 8) | (EEPROM.read(P1add1));
    if (power1_ee > maxpower){
      power1_ee = maxpower;
    }

    // Power 1
    power1 = power1_ee;
    pos1 = power1 / powerinc;
    Aval1 = map(power1, 0, 4095, pwr0, pwr1);

    // Encoder 1
    enc1_ee = pos1 * 4;
    myEnc1.write(enc1_ee);

    // Send or not
    sendpower1 = digitalRead(Ext1);
    clearpower1 = !sendpower0;
  }
}

void eep_commit(bool lower){
  if (!lower){
    // Get the bytes
    uint8_t power0_hbyte = (power0 >> 8) & 0xFF;
    uint8_t power0_lbyte = power0 & 0xFF;

    // L0
    EEPROM.update(S0add, eeprom_sig);
    EEPROM.update(P0add0, power0_hbyte);
    EEPROM.update(P0add1, power0_lbyte);

    #if debug
      Serial.println("L0 EEPROM updated");
    #endif
  }
  else{
    // Get the bytes
    uint8_t power1_hbyte = (power1 >> 8) & 0xFF;
    uint8_t power1_lbyte = power1 & 0xFF;

    // L1
    EEPROM.update(S1add, eeprom_sig);
    EEPROM.update(P1add0, power1_hbyte);
    EEPROM.update(P1add1, power1_lbyte);

    #if debug
      Serial.println("L1 EEPROM updated");
    #endif
  }
  
  EEPROM.commit();
  #if debug
    Serial.println("EEPROM committed");
  #endif
}

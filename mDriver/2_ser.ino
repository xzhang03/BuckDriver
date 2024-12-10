void parseserial(void){
  Serial.print(m);
  Serial.print(" ");
  Serial.println(n);

  switch (m){
    case 'm':
      Serial.println("m: menu");
      Serial.println("n: math test");
      Serial.println("s: input states");
      Serial.println("d: driver states");
      Serial.println("i: i2c scan");
      Serial.println("e: eeprom initialize");
      Serial.println("r: read eeprom");
      break;
      
    case 'n':
      mathtest();
      break;

    case 's':
      
      Serial.print("PB0: ");
      Serial.println(digitalRead(PB0));
      Serial.print("PB1: ");
      Serial.println(digitalRead(PB1));
      Serial.print("Trig0: ");
      Serial.println(digitalRead(Ext0));
      Serial.print("Trig1: ");
      Serial.println(digitalRead(Ext1));
      break;

    case 'd':
      Serial.println("======== L0 ========");
      Serial.print("enc0: ");
      Serial.println(myEnc0.read());
      Serial.print("pos0: ");
      Serial.println(pos);
      Serial.print("power0 ");
      Serial.println(power0);
      Serial.print("Aval0 ");
      Serial.println(Aval0);
      Serial.print("sendpower0 ");
      Serial.println(sendpower0);
      Serial.print("clearpower0 ");
      Serial.println(clearpower0);
      Serial.print("curr0 ");
      Serial.println(curr0);
      Serial.print("mA0 ");
      Serial.println(mA0);
      Serial.print("updatepwrdisp0 (1/0) ");
      Serial.println(updatepwrdisp0);
      Serial.print("updatepower0 ");
      Serial.println(updatepower0);
      
      Serial.println("======== L0 ========");
      Serial.print("enc1: ");
      Serial.println(myEnc1.read());
      Serial.print("pos1: ");
      Serial.println(pos1);
      Serial.print("power1 ");
      Serial.println(power1);
      Serial.print("Aval1 ");
      Serial.println(Aval1);
      Serial.print("sendpower1 ");
      Serial.println(sendpower1);
      Serial.print("clearpower1 ");
      Serial.println(clearpower1);
      Serial.print("curr1 ");
      Serial.println(curr1);
      Serial.print("mA1 ");
      Serial.println(mA1);
      Serial.print("updatepwrdisp1 (1/0) ");
      Serial.println(updatepwrdisp1);
      Serial.print("updatepower1 ");
      Serial.println(updatepower1);
      break;
      
    case 'i':
      i2c_scan();
      break;

    case 'e':
      Serial.print(F("\nStart EEPROM_read on ")); 
      Serial.println(BOARD_NAME);
      Serial.println(FLASH_STORAGE_SAMD_VERSION);
    
      Serial.print("EEPROM length: ");
      Serial.println(EEPROM.length());
      break;

    case 'r':
      {
        uint16_t sig0_ee = EEPROM.read(S0add);
        uint16_t sig1_ee = EEPROM.read(S1add);
        uint16_t power0_ee = (EEPROM.read(P0add0) << 8) | (EEPROM.read(P0add1));
        uint16_t power1_ee = (EEPROM.read(P1add0) << 8) | (EEPROM.read(P1add1));
        
        Serial.print("L0 signature read: ");
        Serial.print(sig0_ee);
        if (sig0_ee == eeprom_sig){
          Serial.println(" (match)");
          Serial.print("L0 power read: ");
          Serial.println(power0_ee);
          Serial.print("Aval: ");
          Serial.println(map(power0_ee, 0, 4095, pwr0, pwr1));
        }
        else{
          Serial.println(" (not match)");
          Serial.print("L0 power read: ");
          Serial.println(power0_ee);
        }
        
        Serial.print("L1 signature read: ");
        Serial.print(sig1_ee);
        if (sig1_ee == eeprom_sig){
          Serial.println(" (match)");
          Serial.print("L1 power read: ");
          Serial.println(power1_ee);
          Serial.print("Aval: ");
          Serial.println(map(power1_ee, 0, 4095, pwr0, pwr1));
        }
        else{
          Serial.println(" (not match)");
          Serial.print("L1 power read: ");
          Serial.println(power1_ee);
        }
      }
      break;
  }
}

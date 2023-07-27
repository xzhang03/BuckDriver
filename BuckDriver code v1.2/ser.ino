void serial_debug(void){
  #if debug
    if (Serial.available() > 1){
      byte m = Serial.read();
      byte n = Serial.read();
  
      switch (m){
        case 'm':
          Serial.println("======= Current =======");
          Serial.print("ON/OFF 0: ");
          Serial.println(onoff0);
          Serial.print("Power 0: ");
          Serial.println(power0);
          Serial.print("ON/OFF 1: ");
          Serial.println(onoff1);
          Serial.print("Power 1: ");
          Serial.println(power1_ee);
          Serial.print("isel: ");
          Serial.println(isel);
          Serial.print("lsel: ");
          Serial.println(lsel);
          Serial.print("pos: ");
          Serial.println(pos);
          Serial.print("pos old: ");
          Serial.println(pos_old);
          
          Serial.println("======= eeprom =======");
          Serial.print("ON/OFF 0: ");
          Serial.println(onoff0_ee);
          Serial.print("Power 0: ");
          Serial.println((int16_t) power0_ee);
          Serial.print("ON/OFF 1: ");
          Serial.println(onoff1_ee);
          Serial.print("Power 1: ");
          Serial.println((int16_t) power1_ee);
          break;
      }
    }
  #endif
}

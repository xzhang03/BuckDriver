void PB0_int(void){
  eeprom_update0 = true;
  #if debug
    Serial.println("PB0 pushed");
  #endif
}

void PB1_int(void){
  eeprom_update1 = true;
  #if debug
    Serial.println("PB1 pushed");
  #endif
}

void ext0_int(void){
  noInterrupts();
  #if (debug) && (debug_ext)
    Serial.print(tnow);
    Serial.print(" ");
  #endif
  if (digitalRead(Ext0)){
    analogWrite(A0, Aval0);
    #if (debug) && (debug_ext)
      Serial.println("Wrote A0");
    #endif
  }
  else{
    analogWrite(A0, 0);
    #if (debug) && (debug_ext)
      Serial.println("Cleared A0");
    #endif
  }
  interrupts();
}

void ext1_int(void){
  noInterrupts();
  #if (debug) && (debug_ext)
    Serial.print(tnow);
    Serial.print(" ");
  #endif
  if (digitalRead(Ext1)){
    analogWrite(A1, Aval1);
    #if (debug) && (debug_ext)
      Serial.println("Wrote A1");
    #endif
  }
  else{
    analogWrite(A1, 0);
    #if (debug) && (debug_ext)
      Serial.println("Cleared A1");
    #endif
  }
  interrupts();
}

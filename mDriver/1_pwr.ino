void send_power(uint16_t Aval, bool lower){

  #if debug
    float mV = Aval * 3.3 / 4096;
    Serial.print("Received: ");
    Serial.print(Aval);
    Serial.print(" Voltage: ");
    Serial.println(mV);
  #endif

  #if (debug) && (debug_slowrite)
    Serial.print(tnow);
    Serial.print(" ");
  #endif
  if (!lower){
    analogWrite(A0, Aval);
    #if (debug) && (debug_slowrite)
      Serial.print(" Slow wrote L0");
    #endif
  }
  else{
    analogWrite(A1, Aval);
    #if (debug) && (debug_slowrite)
      Serial.print(" Slow wrote L1");
    #endif
  }
}

void clear_power(bool lower){
  #if (debug) && (debug_slowrite)
    Serial.print(tnow);
    Serial.print(" ");
  #endif
  if (!lower){
    analogWrite(A0, 0);
    #if (debug) && (debug_slowrite)
      Serial.print(" Slow killed L0");
    #endif
  }
  else{
    analogWrite(A1, 0);
    #if (debug) && (debug_slowrite)
      Serial.print(" Slow killed L1");
    #endif
  }
}

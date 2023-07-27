void sample_curr(void){
  uint16_t I0_r = analogRead(I0);
  uint16_t I1_r = analogRead(I1);

  if (I0_r != current0){
    current0 = I0_r;
    redraw_current0 = true;
    redraw = true;
  }

  if (I1_r != current1){
    current1 = I1_r;
    redraw_current1 = true;
    redraw = true;
  }
}

void send_power(uint16_t power, bool upper){
  uint16_t Aval = map(power, 0, 4095, pwr0, pwr1);

  #if debug
    float mV = Aval * 3.3 / 4096;
    Serial.print("Received: ");
    Serial.print(power);
    Serial.print(" Analog writing: ");
    Serial.print(Aval);
    Serial.print(" Voltage: ");
    Serial.println(mV);
  #endif
  
  
  
  if (upper){
    analogWrite(A0, Aval);
  }
  else{
    analogWrite(A1, Aval);
  }
  
}

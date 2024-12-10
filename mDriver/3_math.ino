void mathtest(void){
  uint32_t t0_math, t1_math;
  uint32_t ans;
  float float1, float2;

  // Integer multiplication
  Serial.print("5*4 = ");
  t0_math = micros();
  ans = 5 * 4;
  t1_math = micros();
  Serial.print(ans);
  Serial.print(" us: ");
  Serial.println(t1_math-t0_math);
  
  // Integer Shift
  Serial.print("5 << 2 = ");
  t0_math = micros();
  ans = 5 << 2;
  t1_math = micros();
  Serial.print(ans);
  Serial.print(" us: ");
  Serial.println(t1_math-t0_math);

  // Integer division
  Serial.print("1024/16 = ");
  t0_math = micros();
  ans = 1024 / 16;
  t1_math = micros();
  Serial.print(ans);
  Serial.print(" us: ");
  Serial.println(t1_math-t0_math);
  
  // Integer Shift
  Serial.print("1024 >> 4 = ");
  t0_math = micros();
  ans = 1024 >> 4;
  t1_math = micros();
  Serial.print(ans);
  Serial.print(" us: ");
  Serial.println(t1_math-t0_math);

  // Float multiplication
  Serial.print("700*0.189 = ");
  float1 = 0.189;
  t0_math = micros();
  float2 = float1 * 700;
  t1_math = micros();
  Serial.print(float2);
  Serial.print(" us: ");
  Serial.println(t1_math-t0_math);

  // Float division
  Serial.print("700/5.283 = ");
  float1 = 5.283;
  t0_math = micros();
  float2 = 1.0 * 700 / float1;
  t1_math = micros();
  Serial.print(float2);
  Serial.print(" us: ");
  Serial.println(t1_math-t0_math);

  // Pseudo float shift
  Serial.print("(700 * 3) >> 4 = ");
  t0_math = micros();
  ans = (700 * 3) >> 4;
  t1_math = micros();
  Serial.print(ans);
  Serial.print(" us: ");
  Serial.println(t1_math-t0_math);
}

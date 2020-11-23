void debug(int x)
{
  Serial.print("*****");
  Serial.println(x);
}
/* Wrap the motor driver initialization */
void initMotorController() {
  //drive.init();
  pinMode(A_IN1,OUTPUT);
  pinMode(A_IN2,OUTPUT);
  pinMode(A_PWM,OUTPUT);

  pinMode(B_IN1,OUTPUT);
  pinMode(B_IN2,OUTPUT);
  pinMode(B_PWM,OUTPUT);

  pinMode(C_IN1,OUTPUT);
  pinMode(C_IN2,OUTPUT);
  pinMode(C_PWM,OUTPUT);

  pinMode(D_IN1,OUTPUT);
  pinMode(D_IN2,OUTPUT);
  pinMode(D_PWM,OUTPUT);
}

/* Wrap the drive motor set speed function */
void setMotorSpeed(int wheel, int spd) {
    //debug(spd);

    if(spd > MAX_PWM)    spd = MAX_PWM;
    if(spd < -MAX_PWM)   spd = -1*MAX_PWM;
    if(wheel == A_WHEEL){
      if(spd >= 0){
        direcA = FORWARDS;
        digitalWrite(A_IN1,HIGH);
        digitalWrite(A_IN2,LOW);
        analogWrite(A_PWM,spd);
        int temp = analogRead(A_PWM);
      }
      else{
        direcA = BACKWARDS;
        digitalWrite(A_IN1,LOW);
        digitalWrite(A_IN2,HIGH);
        analogWrite(A_PWM,-spd);
      }
    }
    else if(wheel == B_WHEEL){
      if(spd >= 0){
        direcB = FORWARDS;
        digitalWrite(B_IN1,HIGH);
        digitalWrite(B_IN2,LOW);
        analogWrite(B_PWM,spd);
      }
      else{
        direcB = BACKWARDS;
        digitalWrite(B_IN1,LOW);
        digitalWrite(B_IN2,HIGH);
        analogWrite(B_PWM,-spd);
      }
    }
    else if(wheel == C_WHEEL){
      if(spd >= 0){
        direcC = FORWARDS;
        digitalWrite(C_IN1,HIGH);
        digitalWrite(C_IN2,LOW);
        analogWrite(C_PWM,spd);
      }
      else{
        direcC = BACKWARDS;
        digitalWrite(C_IN1,LOW);
        digitalWrite(C_IN2,HIGH);
        analogWrite(C_PWM,-spd);
      }
    }
    else if(wheel == D_WHEEL){
      if(spd >= 0){
        direcD = FORWARDS;
        digitalWrite(D_IN1,HIGH);
        digitalWrite(D_IN2,LOW);
        analogWrite(D_PWM,spd);
      }
      else{
        direcD = BACKWARDS;
        digitalWrite(D_IN1,LOW);
        digitalWrite(D_IN2,HIGH);
        analogWrite(D_PWM,-spd);
      }
    }
}

// A convenience function for setting both motor speeds
void setMotorSpeeds(int a, int b, int c, int d) {
  setMotorSpeed(A_WHEEL, a);
  setMotorSpeed(B_WHEEL, b);
  setMotorSpeed(C_WHEEL, c);
  setMotorSpeed(D_WHEEL, d);
}

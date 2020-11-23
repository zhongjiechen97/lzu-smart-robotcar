volatile long A_enc_pos = 0L;
volatile long B_enc_pos = 0L;
volatile long C_enc_pos = 0L;
volatile long D_enc_pos = 0L;

void initEncoders()
{
  pinMode(ENC_A_PIN_A,INPUT);
  pinMode(ENC_A_PIN_B,INPUT);
  attachInterrupt(0, encoderA_ISR,CHANGE);
  attachInterrupt(1, encoderA_ISR, CHANGE);

  pinMode(ENC_B_PIN_A, INPUT);
  pinMode(ENC_B_PIN_B, INPUT);
  attachInterrupt(2, encoderB_ISR, CHANGE);
  attachInterrupt(3, encoderB_ISR, CHANGE);

  pinMode(ENC_C_PIN_A, INPUT);
  pinMode(ENC_C_PIN_B, INPUT);
  attachInterrupt(4, encoderC_ISR, CHANGE);
  attachInterrupt(5, encoderC_ISR, CHANGE);
}

void encoderA_ISR()
{
  if(direcA == BACKWARDS)
  {
    A_enc_pos--;
  }
  else
    A_enc_pos++;
}
void encoderB_ISR()
{
  if(direcB == BACKWARDS)
  {
    B_enc_pos--;
  }
  else
    B_enc_pos++;
}
void encoderC_ISR()
{
  if(direcC == BACKWARDS)
  {
    C_enc_pos--;
  }
  else
    C_enc_pos++;
}
void encoderD_ISR()
{
  if(direcD == BACKWARDS)
  {
    D_enc_pos--;
  }
  else
    D_enc_pos++;
}

  /* Wrap the encoder reading function */
long readEncoder(int i) {
  if (i == A_WHEEL) return A_enc_pos;
  else if(i == B_WHEEL)return B_enc_pos;
  else if(i == C_WHEEL)return C_enc_pos;
  else if(i == D_WHEEL)return D_enc_pos;
}

  /* Wrap the encoder reset function */
void resetEncoder(int i) {
  if (i == A_WHEEL){
    A_enc_pos=0L;
    return;
  } else if(i == B_WHEEL){ 
    B_enc_pos=0L;
    return;
  }
  else if(i == C_WHEEL){
    C_enc_pos=0L;
    return;
  }
  else if(i == D_WHEEL){
    D_enc_pos=0L;
    return;
  }
}

/* Wrap the encoder reset function */
void resetEncoders() {
  resetEncoder(A_WHEEL);
  resetEncoder(B_WHEEL);
  resetEncoder(C_WHEEL);
  resetEncoder(D_WHEEL);
}

#define ENC_A_PIN_A 2    
#define ENC_A_PIN_B 3  

#define ENC_B_PIN_A 21    
#define ENC_B_PIN_B 20

#define ENC_C_PIN_A 19    
#define ENC_C_PIN_B 18

//  #define ENC_D_PIN_A A4    
//  #define ENC_D_PIN_B A5
   
long readEncoder(int i);
void resetEncoder(int i);
void resetEncoders();
void initEncoders();
void encoderA_ISR();
void encoderB_ISR();
void encoderC_ISR();
void encoderD_ISR();

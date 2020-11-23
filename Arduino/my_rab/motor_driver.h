#define D_IN1 A5
#define D_IN2 A4
#define D_PWM 5
// extern const uint8_t D_IN1 = A5;
// extern const uint8_t D_IN2 = A4;
// extern const uint8_t D_PWM = 5;
extern const int A_IN1 = 34;
extern const int A_IN2 = 35;
extern const int A_PWM = 12;

extern const int B_IN1 = 36;
extern const int B_IN2 = 37;
extern const int B_PWM = 8;

extern const int C_IN1 = 43;
extern const int C_IN2 = 42;
extern const int C_PWM = 9;

extern bool direcA = false;
extern bool direcB = false;
extern bool direcC = false;
extern bool direcD = false;

void initMotorController();
void setMotorSpeed(int i, int spd);
void setMotorSpeeds(int a, int b, int c, int d);

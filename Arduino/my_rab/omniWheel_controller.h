/* PID setpoint info For a Motor */
typedef struct {
  double TargetTicksPerFrame;    // target speed in ticks per frame
  long Encoder;                  // encoder count
  long PrevEnc;                  // last encoder count

  /*
  * Using previous input (PrevInput) instead of PrevError to avoid derivative kick,
  * see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-derivative-kick/
  */
  int PrevInput;                // last input
  //int PrevErr;                   // last error

  /*
  * Using integrated term (ITerm) instead of integrated error (Ierror),
  * to allow tuning changes,
  * see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-tuning-changes/
  */
  //int Ierror;
  int ITerm;                    //integrated term

  long output;                    // last motor setting
}
SetPointInfo;

SetPointInfo AwheelPID, BwheelPID,CwheelPID,DwheelPID;

/* PID Parameters */
int AKp = 20;
int AKd = 12;
int AKi = 0;
int AKo = 50;

int BKp = 20;
int BKd = 12;
int BKi = 0;
int BKo = 50;

int CKp = 20;
int CKd = 12;
int CKi = 0;
int CKo = 50;

int DKp = 20;
int DKd = 12;
int DKi = 0;
int DKo = 50;

unsigned char moving = 0; // is the base in motion?

/*
* Initialize PID variables to zero to prevent startup spikes
* when turning PID on to start moving
* In particular, assign both Encoder and PrevEnc the current encoder value
* See http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-initialization/
* Note that the assumption here is that PID is only turned on
* when going from stop to moving, that's why we can init everything on zero.
*/
void resetPID(){
   AwheelPID.TargetTicksPerFrame = 0.0;
   AwheelPID.Encoder = readEncoder(A_WHEEL);
   AwheelPID.PrevEnc = AwheelPID.Encoder;
   AwheelPID.output = 0;
   AwheelPID.PrevInput = 0;
   AwheelPID.ITerm = 0;

   BwheelPID.TargetTicksPerFrame = 0.0;
   BwheelPID.Encoder = readEncoder(B_WHEEL);
   BwheelPID.PrevEnc = BwheelPID.Encoder;
   BwheelPID.output = 0;
   BwheelPID.PrevInput = 0;
   BwheelPID.ITerm = 0;

   CwheelPID.TargetTicksPerFrame = 0.0;
   CwheelPID.Encoder = readEncoder(C_WHEEL);
   CwheelPID.PrevEnc = CwheelPID.Encoder;
   CwheelPID.output = 0;
   CwheelPID.PrevInput = 0;
   CwheelPID.ITerm = 0;

   DwheelPID.TargetTicksPerFrame = 0.0;
   DwheelPID.Encoder = readEncoder(D_WHEEL);
   DwheelPID.PrevEnc = DwheelPID.Encoder;
   DwheelPID.output = 0;
   DwheelPID.PrevInput = 0;
   DwheelPID.ITerm = 0;
}

/* PID routine to compute the next motor commands */
void doAWheelPID(SetPointInfo * p) {
 long Perror = 0;
 long output = 0;
 int input = 0;
 //Perror = p->TargetTicksPerFrame - (p->Encoder - p->PrevEnc);

 input = p->Encoder - p->PrevEnc;
 Perror = p->TargetTicksPerFrame - input;


 /*
 * Avoid derivative kick and allow tuning changes,
 * see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-derivative-kick/
 * see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-tuning-changes/
 */
 output = (AKp * Perror - AKd * (input - p->PrevInput) + p->ITerm) / AKo;
 p->PrevEnc = p->Encoder;

 output += p->output;
 // Accumulate Integral error *or* Limit output.
 // Stop accumulating when output saturates
 if (output >= MAX_PWM)
   output = MAX_PWM;
 else if (output <= -MAX_PWM)
   output = -MAX_PWM;
 else
 /*
 * allow turning changes, see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-tuning-changes/
 */
   p->ITerm += AKi * Perror;

 p->output = output;
 p->PrevInput = input;
}

void doBWheelPID(SetPointInfo *p)
{
   long Perror = 0;
   long output = 0;
   int input = 0;

   input = p->Encoder - p->PrevEnc;
   Perror = p->TargetTicksPerFrame - input;

   output = (BKp * Perror - BKd * (input - p->PrevInput) + p->ITerm) / BKo;
   p->PrevEnc = p->Encoder;

   output += p->output;
   if (output >= MAX_PWM)
      output = MAX_PWM;
   else if (output <= -MAX_PWM)
      output = -MAX_PWM;
   else
      p->ITerm += BKi * Perror;

   p->output = output;
   p->PrevInput = input;
}

void doCWheelPID(SetPointInfo *p)
{
   long Perror = 0;
   long output = 0;
   int input = 0;

   input = p->Encoder - p->PrevEnc;
   Perror = p->TargetTicksPerFrame - input;

   output = (CKp * Perror - CKd * (input - p->PrevInput) + p->ITerm) / CKo;
   p->PrevEnc = p->Encoder;

   output += p->output;
   if (output >= MAX_PWM)
      output = MAX_PWM;
   else if (output <= -MAX_PWM)
      output = -MAX_PWM;
   else
      p->ITerm += CKi * Perror;

   p->output = output;
   p->PrevInput = input;
}

void doDWheelPID(SetPointInfo *p)
{
   long Perror = 0;
   long output = 0;
   int input = 0;

   input = p->Encoder - p->PrevEnc;
   Perror = p->TargetTicksPerFrame - input;

   output = (DKp * Perror - DKd * (input - p->PrevInput) + p->ITerm) / DKo;
   p->PrevEnc = p->Encoder;

   output += p->output;
   if (output >= MAX_PWM)
      output = MAX_PWM;
   else if (output <= -MAX_PWM)
      output = -MAX_PWM;
   else
      p->ITerm += DKi * Perror;

   p->output = output;
   p->PrevInput = input;
}

/* Read the encoder values and call the PID routine */
void updatePID() {
 /* Read the encoders */
 AwheelPID.Encoder = readEncoder(A_WHEEL);
 BwheelPID.Encoder = readEncoder(B_WHEEL);
 CwheelPID.Encoder = readEncoder(C_WHEEL);
 DwheelPID.Encoder = readEncoder(D_WHEEL);

 if (!moving) { /* If we're not moving there is nothing more to do */
    if (AwheelPID.PrevInput != 0 || BwheelPID.PrevInput != 0 ||
        CwheelPID.PrevInput != 0 || DwheelPID.PrevInput != 0)
       resetPID();
    return;
 }

 /* Compute PID update for each motor */
 doAWheelPID(&AwheelPID);
 doBWheelPID(&BwheelPID);
 doCWheelPID(&CwheelPID);
 doDWheelPID(&DwheelPID);
 /* Set the motor speeds accordingly */
 setMotorSpeeds(AwheelPID.output,BwheelPID.output,CwheelPID.output,DwheelPID.output);
}

long readPidIn(int wheel){
   long pidin=0;
   if(wheel == A_WHEEL)pidin = AwheelPID.PrevInput;
   else if(wheel == B_WHEEL)pidin = BwheelPID.PrevInput;
   else if(wheel == C_WHEEL)pidin = CwheelPID.PrevInput;
   else pidin = DwheelPID.PrevInput;

   return pidin;
}

long readPidOut(int wheel)
{
   long pidout = 0;
   if (wheel == A_WHEEL)pidout = AwheelPID.output;
   else if (wheel == B_WHEEL)pidout = BwheelPID.output;
   else if (wheel == C_WHEEL)pidout = CwheelPID.output;
   else  pidout = DwheelPID.output;
   return pidout;
}

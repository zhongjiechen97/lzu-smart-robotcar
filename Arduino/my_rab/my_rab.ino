/* Run the PID loop at 30 times per second */
#define PID_RATE 30 // Hz
/* Serial port baud rate */
#define BAUDRATE 57600
/* Maximum PWM signal */
#define MAX_PWM 255
/* Include definition of serial commands */
#include "commands.h"
/* Sensor functions */
#include "sensors.h"
/* Motor driver function definitions */
#include "motor_driver.h"
/* Encoder driver function definitions */
#include "encoder_driver.h"
/* PID parameters and functions */
#include "omniWheel_controller.h"
/* Convert the rate into an interval */
const int PID_INTERVAL = 1000 / PID_RATE;
/* Track the next time we make a PID calculation */
unsigned long nextPID = PID_INTERVAL;
/* Stop the robot if it hasn't received a movement command
 in this number of milliseconds */
#define AUTO_STOP_INTERVAL 200
long lastMotorCommand = AUTO_STOP_INTERVAL;
/* Variable initialization */
// A pair of varibles to help parse serial commands (thanks Fergs)
int arg = 0;
int index = 0;
// Variable to hold an input character
char chr;
// Variable to hold the current single-character command
char cmd;
// Character arrays to hold the first and second arguments,it used for 4 motor
char argv1[64];
char argv2[64];
char argv3[64];
char argv4[64];

// The arguments converted to integers
long arg1=0;
long arg2=0;
long arg3=0;
long arg4=0;

/* Clear the current command parameters */
void resetCommand() {
  cmd = '\0';
  
  memset(argv1, 0, sizeof(argv1));
  memset(argv2, 0, sizeof(argv2));
  memset(argv3, 0, sizeof(argv3));
  memset(argv4, 0, sizeof(argv4));
  
  arg1 = 0;
  arg2 = 0;
  arg3 = 0;
  arg4 = 0;
  
  arg = 0;
  index = 0;
}

/* Run a command.  Commands are defined in commands.h */
int runCommand() {
  int i = 0;
  char *p = argv1;
  char *str;
  
  int pid_args[16];
  arg1 = atoi(argv1);
  arg2 = atoi(argv2);
  arg3 = atoi(argv3);
  arg4 = atoi(argv4);

  switch(cmd){
    case GET_BAUDRATE:
      Serial.println(BAUDRATE);
      break;
    case DHT_READ:
    {
      std::pair<float, float> dht_res;
      uint8_t dhtpin=54;
      dhtpin += argv1[1] - '0';
      dht_res = readDHT22(dhtpin);
      Serial.print(dht_res.first);
      Serial.print(" ");
      Serial.print(dht_res.second);
      Serial.println();
    }
      break;
    case ANALOG_READ:
      Serial.println(analogRead(arg1));
      break;
    case DIGITAL_READ:
      Serial.println(digitalRead(arg1));
      break;
    case ANALOG_WRITE:
      analogWrite(arg1, arg2);
      Serial.println("OK"); 
      break;
    case DIGITAL_WRITE:
      if (arg2 == 0) digitalWrite(arg1, LOW);
      else if (arg2 == 1) digitalWrite(arg1, HIGH);
      Serial.println("OK"); 
      break;
    case PIN_MODE:
      if (arg2 == 0) pinMode(arg1, INPUT);
      else if (arg2 == 1) pinMode(arg1, OUTPUT);
      Serial.println("OK");
      break;
    case PING:
      Serial.println(Pinging(arg1));
      break;
    case READ_ENCODERS://'e'
      Serial.print(readEncoder(A_WHEEL));
      Serial.print(" ");
      Serial.print(readEncoder(B_WHEEL));
      Serial.print(" ");
      Serial.print(readEncoder(C_WHEEL));
      Serial.print(" ");
      Serial.println(readEncoder(D_WHEEL));
      break;
    case RESET_ENCODERS://'r'
      resetEncoders();
      resetPID();
      Serial.println("OK");
      break;
    case MOTOR_SPEEDS://'m'
      /* Reset the auto stop timer */
      // Serial.println(arg1);
      // Serial.println(arg2);
      // Serial.println(arg3);
      // Serial.println(arg4);
      lastMotorCommand = millis();
      if (arg1 == 0 && arg2 == 0 &&arg3 == 0 && arg4 == 0) {
        setMotorSpeeds(0, 0,0,0);
        resetPID();
        moving = 0;
      }
      else {
        moving = 1;
        setMotorSpeeds(arg1,arg2,arg3,arg4);
        delay(100);
        setMotorSpeeds(0,0,0,0);
      }
      AwheelPID.TargetTicksPerFrame = arg1;
      BwheelPID.TargetTicksPerFrame = arg2;
      CwheelPID.TargetTicksPerFrame = arg3;
      DwheelPID.TargetTicksPerFrame = arg4;
      
      Serial.println("OK"); 
      break;
    case UPDATE_PID://'u'
      while ((str = strtok_r(p, ":", &p)) != '\0') {
        pid_args[i] = atoi(str);
        i++;
      }
      AKp = pid_args[0];
      AKd = pid_args[1];
      AKi = pid_args[2];
      AKo = pid_args[3];

      BKp = pid_args[4];
      BKd = pid_args[5];
      BKi = pid_args[6];
      BKo = pid_args[7];

      CKp = pid_args[8];
      CKd = pid_args[9];
      CKi = pid_args[10];
      CKo = pid_args[11];
      
      DKp = pid_args[12];
      DKd = pid_args[13];
      DKi = pid_args[14];
      DKo = pid_args[15];
      
      Serial.print(AKp);
      Serial.print(" ");
      Serial.print(AKd);
      Serial.print(" ");
      Serial.print(AKi);
      Serial.print(" ");
      Serial.print(AKo);
      Serial.print(" ");

      Serial.print(BKp);
      Serial.print(" ");
      Serial.print(BKd);
      Serial.print(" ");
      Serial.print(BKi);
      Serial.print(" ");
      Serial.print(BKo);
      Serial.print(" ");

      Serial.print(CKp);
      Serial.print(" ");
      Serial.print(CKd);
      Serial.print(" ");
      Serial.print(CKi);
      Serial.print(" ");
      Serial.print(CKo);
      Serial.print(" ");

      Serial.print(DKp);
      Serial.print(" ");
      Serial.print(DKd);
      Serial.print(" ");
      Serial.print(DKi);
      Serial.print(" ");
      Serial.print(DKo);
      Serial.print(" ");

      Serial.println("OK");
      break;
    case READ_PIDIN:
    Serial.print(readPidIn(A_WHEEL));
    Serial.print(" ");
    Serial.print(readPidIn(B_WHEEL));
    Serial.print(" ");
    Serial.print(readPidIn(C_WHEEL));
    Serial.print(" ");
    Serial.print(readPidIn(D_WHEEL));
    break;
    case READ_PIDOUT:
    Serial.print(readPidOut(A_WHEEL));
    Serial.print(" ");
    Serial.print(readPidOut(B_WHEEL));
    Serial.print(" ");
    Serial.print(readPidOut(C_WHEEL));
    Serial.print(" ");
    Serial.print(readPidOut(D_WHEEL));
    break;

    default:
      Serial.println("Invalid Command");
      break;
  }
}

/* Setup function--runs once at startup. */
void setup() {
  
  Serial.begin(BAUDRATE);
  initEncoders();
  initMotorController();
  resetPID();
}

void loop() {
  while (Serial.available() > 0) {  
    // Read the next character
    chr = Serial.read();
    // 回车的ASCII码为13
    if (chr == 13) {
      if (arg == 1) argv1[index] = NULL;
      else if (arg == 2) argv2[index] = NULL;
      else if(arg ==3 ) argv3[index] = NULL;
      else if(arg == 4) argv4[index] = NULL;
      runCommand();
      resetCommand();
    }
    /* 用' '分隔命令,参数与参数,四轮车最多有4个参数,故arg最大值为4,此外每输完一个参数后隔一个' '
    输入下一个参数,故arg++,index记录每个参数在字符数组内的最后一位,空格后是新的参数,故要重置为0*/
    else if (chr == ' ') {
      // Step through the arguments
      if (arg == 0) arg = 1;
      else if (arg == 1)  {
        argv1[index] = NULL;
        arg = 2;
        index = 0;
      }
      else if (arg == 2)  {
        argv2[index] = NULL;
        arg = 3;
        index = 0;
      }
      else if (arg == 3)  {
        argv3[index] = NULL;
        arg = 4;
        index = 0;
      }
      continue;
    }
    else {
      switch(arg){
        /* index记录当前参数最后一位,如果不输入空格就继续自增 */
        case 0:// The first arg is the single-letter command
          cmd = chr;break;
        case 1: // Subsequent arguments can be more than one character
          argv1[index] = chr;
          index++;break;
        case 2:
          argv2[index] = chr;
          index++;break;
        case 3:
          argv3[index] = chr;
          index++;break;
        case 4:
          argv4[index] = chr;
          index++;break;
        default:
          break;
      }
    }
  }
  
  
//  if (millis() > nextPID) {
//    updatePID();
//    nextPID += PID_INTERVAL;
//  }
//  
//  // Check to see if we have exceeded the auto-stop interval
//  if ((millis() - lastMotorCommand) > AUTO_STOP_INTERVAL) {
//    setMotorSpeeds(0, 0,0,0);
//    moving = 0;
//  }
}

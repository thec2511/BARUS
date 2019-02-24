///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//File Name		      : BARUS_OPENCR_CODE
//Date of creation	: 2019/01/30
//Creator		        : BARUS team members - Olivier Girouard, Alexandre Demers
//
//Description		    : The program intiate 3 motors in Joint Mode and receive data from a RasperryPi to control them. A servo is also controlled by the Pi.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <DynamixelWorkbench.h>
#include "Motor.h"
#include "Function.h"
#include <Servo.h>

//---- Define ----//

#define DEVICE_NAME ""
#define BAUDRATE  57600

#define MOTOR_ID_1    1
#define MOTOR_ID_2    2
#define MOTOR_ID_3    3
#define SERVOPIN      3

#define MIN_POSITION 5
#define MAX_POSTITON 4085

#define HOME_POSITION_MOTOR1 5
#define HOME_POSITION_MOTOR2 5
#define HOME_POSITION_MOTOR3 5

//---- Detail Declaration ----//

const uint32_t baudrate = BAUDRATE;
const char* deviceName = DEVICE_NAME;

//---- Actuator Declaration ----//
  
Servo gripperServo;
int servoPin = SERVOPIN;

Motor* motor3;
Motor* motor2;
Motor* motor1;

uint8_t motorId_1 = MOTOR_ID_1;
uint8_t motorId_2 = MOTOR_ID_2;
uint8_t motorId_3 = MOTOR_ID_3;

uint16_t motorModel_1 = 0;
uint16_t motorModel_2 = 0;
uint16_t motorModel_3 = 0;


//---- Other Declaration ----//

bool communicationIsOk = false;
bool gripperIsClosed = true;

int32_t homePositionMotor1 = HOME_POSITION_MOTOR1;
int32_t homePositionMotor2 = HOME_POSITION_MOTOR2;
int32_t homePositionMotor3 = HOME_POSITION_MOTOR3;

bool isRolling = false;

void setup() 
{
  Serial.begin(57600);
  
  motor3 = new Motor(baudrate, deviceName, motorId_3 , motorModel_3);
  motor2 = new Motor(baudrate, deviceName, motorId_2 , motorModel_2);
  motor1 = new Motor(baudrate, deviceName, motorId_1 , motorModel_1);

  motor1->initWheelMode();
  motor2->initWheelMode();
  motor3->initWheelMode();

//---- Homming ----//
  
  motor1->setHomePos();
  motor2->setHomePos();
  motor3->setHomePos();
  motor1->homing();
  motor2->homing();
  motor3->homing();

/*
  //gripperServo.attach(servoPin);
  gripperServo.write(90);//angle in degrees
  delay(500);
*/  
 
}

void loop()
{
 
//---- Check communication RPI-OPENCR  ----//
  checkBegin(&communicationIsOk);

  int test = -1;
  test = read_Int();
  //writeIntToRpi(test);

  if(signalIsValid(test)){
    switch(test/10000)
    {
      case 0:
        motor3->motor.torqueOff(motor3->motorID);
        isRolling = false;
        break;
      
      case 1:
        motor1->motorTurnLeft(test%10000);
        delay(500);
        writeIntToRpi(motor1->getCurrentPosition(motor1->motorID));
        break;
        
      case 2:
        motor2->motorTurnLeft(test%10000);
        delay(500);
        writeIntToRpi(motor2->getCurrentPosition(motor2->motorID));
        break;
        
      case 3:
        if(!isRolling){
          motor3->initWheelMode();
          if(test%10000 == 1){
            motor3->rotate(5.0);
          }
          else if(test%10000 == 0){
            motor3->rotate(-5.0);
          }
          isRolling = true;
        }
        break;

      case 4:
        motor3->motorTurnLeft(test%10000);
        delay(500);
        writeIntToRpi(motor3->getCurrentPosition(motor3->motorID));
        break;    
    }
  }
}

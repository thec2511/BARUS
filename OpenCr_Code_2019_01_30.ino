///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Program Name		  : InitiateMotor.py
//Date of creation	: 2019/01/30
//Creator		        : BARUS team members - Olivier Girouard, Alexandre Demers
//
//Description		    : The program intiate 3 motors in Joint Mode and make them mode at the and to make sure the communication is OK
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <DynamixelWorkbench.h>
#include <Servo.h>

#define DEVICE_NAME ""
#define BAUDRATE  57600

#define MOTOR_ID_1    1
#define MOTOR_ID_2    2
#define MOTOR_ID_3    3
#define SERVOPIN      3

/* Creation of motor and servo objects */
DynamixelWorkbench motor1;  
DynamixelWorkbench motor2;
DynamixelWorkbench motor3;
Servo gripperServo;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name		:initJointMode() 
//Parameters		  : &motor (DynamixelWorkbench) - Address of the motor object to initiate in jointMode
//                  motor_ID (uint8_t) - ID of the motor passed in parameter
//                  model_number (uint16_t) - Model Number of the motor passed in parameter
//                  velocity (int32_t) - Profile of velocity to set according to Dynamixel Workbench function jointMode()
//                  acceleration (int32_t) - Profile of acceleration to set according to Dynamixel Workbench function jointMode() 
//Return		      : nothing
//
//Description		  : This function initiate a DynamixelWorkbench object with the device board and the BAUDRATE. It also 
//                  place the object in Joint Mode instead of Wheel Mode.  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////Our Functions://///
void initJointMode(DynamixelWorkbench &motor, uint8_t motor_ID , uint16_t model_number, int32_t velocity, int32_t acceleration){
  const char *log;
  bool result = false;
  
  result = motor.init(DEVICE_NAME, BAUDRATE, &log);
  result = motor.ping(motor_ID, &model_number, &log);
  if (result == false)
  {
    Serial.println(log);
    Serial.println("Failed to ping");
  }
  else
  {
    Serial.println("Succeeded to ping");
    Serial.print("id : ");
    Serial.print(motor_ID);
    Serial.print(" model_number : ");
    Serial.println(model_number);
  }
  result = motor.jointMode(motor_ID, velocity, acceleration, &log); 
}

//-----

void motorTurnLeft(DynamixelWorkbench &motor, uint8_t motor_ID , uint16_t model_number, int32_t goalPosition )
{
  const char *log;
  bool result = false;
  result = motor.setNormalDirection(motor_ID, &log);
  motor.goalPosition(motor_ID, goalPosition);
}

//-----

void motorTurnRight(DynamixelWorkbench &motor, uint8_t motor_ID , uint16_t model_number, int32_t goalPosition)
{
  const char *log;
  bool result = false;
  result = motor.setReverseDirection(motor_ID, &log);
  motor.goalPosition(motor_ID, goalPosition);
}

void setup() 
{
  Serial.begin(57600);

  uint8_t motorId_1 = MOTOR_ID_1;
  uint8_t motorId_2 = MOTOR_ID_2;
  uint8_t motorId_3 = MOTOR_ID_3;
  
  uint16_t motorModel_1 = 0;
  uint16_t motorModel_2 = 0;
  uint16_t motorModel_3 = 0;

  int servoPin = SERVOPIN;

  // Initialise motors for Joint mode
  initJointMode(motor1, motorId_1 , motorModel_1, 0, 0);
  initJointMode(motor2, motorId_2 , motorModel_2, 0, 0);
  initJointMode(motor3, motorId_3 , motorModel_3, 0, 0);

  // Attach servo to pin number
  gripperServo.attach(servoPin);
  
  // Set motors goal position (0 to 4092)
  for (int count = 0; count < 1; count++)
  {
    motorTurnRight(motor2, motorId_2, motorModel_2, 0);
    delay(3000);
    motorTurnLeft(motor2, motorId_2 , motorModel_2, 1000);
    delay(3000);
    motorTurnLeft(motor2, motorId_2 , motorModel_2, 2000);
    delay(3000);
    
    /*
    control joint motors
    motor1.goalPosition(motorId_1, (int32_t)0);
    motor2.goalPosition(motorId_2, (int32_t)0);
    motor3.goalPosition(motorId_3, (int32_t)0);
    delay(3000);
  
    motor1.goalPosition(motorId_1, (int32_t)1000);
    motor2.goalPosition(motorId_2, (int32_t)2000);
    motor3.goalPosition(motorId_3, (int32_t)3000);
    delay(3000);  
    */
    
    /*
    //control servo gripper
    gripperServo.write(0);
    delay(2000);
    gripperServo.write(90);
    delay(2000);
    gripperServo.write(0);
    delay(1000);
    */
    
    
   }

 }


void loop() 
{

}

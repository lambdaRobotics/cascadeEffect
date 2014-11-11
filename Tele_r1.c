#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     leftRaise,     tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     frontLeft,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     backLeft,      tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_1,     frontRight,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     backRight,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     rightRaise,    tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_2,     rotater,       tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    servo_grab,           tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    servo_auto,           tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/************************ PROGRAM DESCRIPTION   ****************************/
/* Tele_r1.c: Teleop mode, release 1 (Naming convetion: Tele or Auto; r#: releae number)
/* This program is to allow manual operation or robot in tele-op phase (include end game)
/*
/* Program Operation:
/* step 1: initialize robot servos, placed in desired position.
/*         hook_servo: in closed down position;
/*         auto_servo: in upright position, get out of way of flag motor handel;
/*         basket_servo: in level position, slight away from floor to allow free movement of robot
/*         motor encoders: geneerally in zero positions;
/************************  End of Description   ***************************/

/************************ SYSTEM INCLUDE FILES  ****************************/
/* System include files added here
/***************************************************************************/

// Lotitech joystick file; I copied this into my local direction
//                         your path may be different
#include "JoystickDriver.c"

// Hi-Technic Sensor Multeplexer file
#include "drivers/hitechnic-sensormux.h"

/************************  End of System Include Files   ***************************/

/************************ GLOBAL CONSTANTS & VARIABLES  ****************************/
// global constants
#define powerlevel_fine_movement 85     // motor power used in small movement steps
#define powerlevel_slow_movement 30
#define encoder_step_per_inch    147    // encoder step for per inch robot movement straight
#define combine_power 50
#define combine_power_half 35


/************************ END OF GLOBAL CONSTANTS AND VARIABLES  ****************************/



/************************ FUNCTION DECLARATIONS  ****************************/

void initializeRobot();                 // robot initialization task
void joystick_motor_control ();         // joystick control task


/******************************  MAIN TASK      ****************************/

task main()
{

 initializeRobot();                     // initialize robot first; pay attention to servo positions!

 waitForStart();                        // wait for start of tele-op phase by FTC operator!!

 /* program wait before going further unless 'teleop running' is selected */

 while (true)
  {
   joystick_motor_control();	//joystick control for teleop mode

  }

}

/******************************  END OF MAIN TASK      ***********************/


/******************************  TASK DEFINITIONS     ***********************/


// initialize robot: servo initial positions encoder reading reset, etc
void initializeRobot()
{
  // Place code here to sinitialize servos to starting positions.
  // Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.

   motor[leftRaise] = 0;
   motor[rightRaise] = 0;
   motor[frontLeft] = 0;
   motor[backLeft] = 0;
   motor[rightRaise] = 0;
   motor[frontRight] = 0;
   motor[frontLeft] = 0;
   motor[rotater] = 0;

   // Servo init
  servo[servo_grab] = 127;
  servo[servo_auto] = 127;

  return;
}

// Joystick motor control routine
void joystick_motor_control ()
{
	  getJoystickSettings(joystick);


  // rack dual motor control, threshold set at +/- 65 to prevent unintended activation

    if (joystick.joy1_y2 > 65 || joystick.joy1_y2 <-65) {

      motor[rightRaise] = joystick.joy1_y2;
    	motor[leftRaise] = joystick.joy1_y2;
    }
    else {
    	motor[rightRaise] = 0;
    	motor[leftRaise] = 0;
    }


  // servo grab control: 0 and 210 are limits
  // up

  	if(joy1Btn(6) == 1 ){
  		if(ServoValue[servo_grab] <= 240) {
  			servo[servo_grab] += 2;
  			wait10Msec(1);
  			}
   }
    // down
    if(joy1Btn(5) == 1 ){
    	if(ServoValue[servo_grab] >= 0) {
  			servo[servo_grab] -= 4;
  			wait10Msec(1);
  			}
   }
  	if(joy1Btn(7) == 1 ){
  		if(ServoValue[servo_auto] <= 255) {
  			servo[servo_auto] += 1;
  			wait10Msec(5);
  			}
   }
    // down
    if(joy1Btn(8) == 1 ){
    	if(ServoValue[servo_auto] >= 0) {
  			servo[servo_auto] -= 1;
  			wait10Msec(5);
  			}
   }



  // motor rotater control: button9,10: toggle motor on/off, in positive and negative directions
  // toggles between off an on. Button 9 will stop it, button 10 will start it
  // once the rotater stops spinning, the balls will fall.

	if(joy1Btn(9) == 1 ){
		if(motor[rotater] < 0){
			motor[rotater] = 0;
			wait10Msec(50);
			} else {

			motor[rotater] = 10;
			wait10Msec(50);      // wait 0.2s to prevent count button press twice
		}
	}

  // brings in spheres
   if (joy1Btn(10) == 1) {
     motor[rotater] = -75;
     wait10Msec(50);     // wait for 0.2s to prevent count button press twice
   }



  // Moving motors control: Coarse
  // Control works like this: If you push joystick to full throttle position
  // and suddently release the throttle, the motor will coast to a gradual stop; this is
  // to prevent sudden stop of robot from a high speed;
  // on the other hand, if you gradually brings the throttle to a close, the motor will stop
  // immeidately stop, no 'coast down' effect, this is to allow relative precision stop
  // For turning, power level is reduced to allow smooth turn;

   if ((joystick.joy1_y1 < 20 && joystick.joy1_y1 > -20) && joystick.joy1_x1 < 20 && joystick.joy1_x1 > -20) {
     if (motor[frontRight] > 1 || motor[frontRight] < -1)
       motor[frontRight]= motor[frontRight] /4;
     if (motor[backRight] > 1 || motor[backRight] < -1)
       motor[backRight] = motor[backRight] /4;
     if (motor[frontLeft] > 1 || motor[frontLeft] < -1)
       motor[frontLeft] = motor[frontLeft] /4;
     if (motor[backLeft] > 1 || motor[backLeft] < -1)
       motor[backLeft] = motor[backLeft] /4;
     //wait1Msec(2);
   }
   else {
    motor[frontRight] = joystick.joy1_y1 - joystick.joy1_x1/2;
  	motor[backRight]  = joystick.joy1_y1 - joystick.joy1_x1/2;
  	motor[frontLeft]  = joystick.joy1_y1 + joystick.joy1_x1/2;
  	motor[backLeft]   = joystick.joy1_y1 + joystick.joy1_x1/2;

 }


 // moving motor control: Fine
 // reduced power and clean button control allows small, precise
 // movement of robot for picking and dump blocks etc.
 // in each pass of this joystick_motor_control task, it reads once the
 // joystick.joy1_y1, x1 and then these buttons;
 // if none of these buttons are pressed, the control is solely set by
 // the left joystick;
 // if the joystick is left at zero position, and one of these buttons pressed,
 // the actual value for motor will be set by these buttons, since it overwrite the
 // values set by the joystick left stick

 // forward
  if( joy1Btn(4) == 1) {
  	motor[frontRight] = powerlevel_slow_movement;
  	motor[backRight]  = powerlevel_slow_movement;
  	motor[frontLeft]  = powerlevel_slow_movement;
  	motor[backLeft]   = powerlevel_slow_movement;
  }
  // backward
  else if (joy1Btn(2) == 1) {
  	motor[frontRight] = -powerlevel_slow_movement;
  	motor[backRight]  = -powerlevel_slow_movement;
  	motor[frontLeft]  = -powerlevel_slow_movement;
  	motor[backLeft]   = -powerlevel_slow_movement;
  }
  //left turn
  else if (joy1Btn(1) == 1){
  	motor[frontLeft]  = -powerlevel_fine_movement;
    motor[backLeft]   = -powerlevel_fine_movement;
    motor[frontRight] = powerlevel_fine_movement;
    motor[backRight]  = powerlevel_fine_movement;
  }
  // right turn
  else if (joy1Btn(3) == 1) {
  	motor[frontLeft]  = powerlevel_fine_movement;
    motor[backLeft]   = powerlevel_fine_movement;
    motor[frontRight] = -powerlevel_fine_movement;
    motor[backRight]  = -powerlevel_fine_movement;
  }




  /* unused button
  //
  if (joystick.joy1_TopHat == 0) {
  	 if(servo[servo_unused] <=225)
       servo[servo_unused] += 10;
     wait10Msec(1);
          }
  if (joystick.joy1_TopHat == 4) {
  	if(servo[servo_unused] >=0)
  	   servo[servo_unused] -= 10;
  	wait10Msec(1);
  }
  */

}

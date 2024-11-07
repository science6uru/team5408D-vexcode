/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       andrewowen                                                */
/*    Created:      11/5/2024, 2:52:11 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "robot-config.hpp"
#include "vex.h"
#include "driverControl.hpp"
#include "controller_button.h"

using namespace vex;


// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here


/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void initialize() {

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  printf("initialize\n");
  setupDriveMotors(leftMotors);
  setupDriveMotors(rightMotors);
  setupArmMotor(arm);
  imu.calibrate(3);
  while(imu.isCalibrating()) {
    printf("calibrating\n");
    wait(200, timeUnits::msec);
  }
  imu.resetRotation();
  
  // imu.setRotation(0, rotationUnits::deg);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

ArmPosition id = Retracted;

void onButtonR1Changed(State state) {
  if(state == 9) {
    id = (ArmPosition)(NUM_ARM_POSITIONS - 1);
    switchArm(id);
    printf("    double clicked %d\n", id);
  }
  else if(state == Click) {
    printf("    clicked %d\n", id);
    if((id + 1) < NUM_ARM_POSITIONS) {
      id = (ArmPosition)(id + 1);
      switchArm((ArmPosition)id);
    }
  }
  printf("%d\n", id);
}

void onButtonR2Changed(State state) {
  printf("  state %d\n", state);
  if(state == 9) {
    id = Retracted;
    switchArm(id);
    printf("    double clicked %d\n", id);
  }
  else if(state == Click) {
    if((id - 1) < NUM_ARM_POSITIONS) {
      id = (ArmPosition)(id - 1);
      switchArm((ArmPosition)id);
    }
    printf("    clicked %d\n", id);
  }
}

bool pistonExtend = false;

void onButtonL1Changed(State state) {
  if(state & Press) {
    if(pistonExtend == false)
    {
      pistonA.open();
      pistonExtend = true;
    }
    else
    {
      pistonA.close();
      pistonExtend = false;
    }
  }
}

void usercontrol(void) {

  BUTTON_HELPER(R1);
  BUTTON_HELPER(R2);
  BUTTON_HELPER(L1);
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    //leftMotors.setVelocity(50, rpm);

    setDriveMotors(Controller.Axis3.position(), Controller.Axis1.position(), imu.rotation(deg));



    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}


void printThreadRunner() {
  while(true) {
    // put stuff here
    //printf("rotation: %f degrees\n", imu.rotation(rotationUnits::deg));
    //printf("imu.calibrating: %d\n", imu.isCalibrating());
    //printf("gyro: %f\n", imu.gyroRate(axisType::xaxis, velocityUnits::dps));
    printf("%f\n",arm.position(deg));
    wait(.5, seconds);
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  
  // Run the pre-autonomous function.
  initialize();
  thread t(printThreadRunner);

  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);


  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}

#ifndef ROBOT_CONFIG_H
#define ROBOT_CONFIG_H

#include "vex.h"

using namespace vex;

extern brain Brain;
extern controller Controller;

#ifdef MOTOR_CONFIG_D
extern motor backLeft;
extern motor frontLeft;
extern motor backRight;
extern motor frontRight;
extern inertial imu;

extern motor_group leftMotors;
extern motor_group rightMotors;

typedef enum {
    Retracted=0,
    PrepareScore,
    ScoreHighGoal,
    ScoreLowGoal,
    Descore,
    NUM_ARM_POSITIONS
} ArmPosition;

#else
extern motor backLeft;
extern motor frontLeft;
extern motor backRight;
extern motor frontRight;

extern motor conveyor;
extern motor arm;

extern inertial imu;

extern motor_group leftMotors;
extern motor_group rightMotors;

typedef enum {
    Retracted=0,
    PrepareScore,
    ScoreHighGoal,
    ScoreLowGoal,
    Descore,
    NUM_ARM_POSITIONS
} ArmPosition;

#endif

extern double armPositions[NUM_ARM_POSITIONS];




void setupDriveMotors(motor_group& mg);
void setupArmMotor(motor m);

#endif // end_H
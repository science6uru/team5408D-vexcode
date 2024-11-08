#ifndef DRIVER_CTRL_H
#define DRIVER_CTRL_H
#include "robot-config.hpp"

void setDriveMotors(double lefty, double rightx, double rot);
void switchArm(ArmPosition id);
void driveConveyor(bool id);

#endif
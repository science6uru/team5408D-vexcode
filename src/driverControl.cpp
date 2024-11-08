#include "driverControl.hpp"

static bool armState = false;

double powerMultiplier = 1;
double rightMultiplier = 1;
double leftMultiplier = 1;

int right_stick_smoothed = 0;
int left_stick_smoothed = 0;
float left_stick_prev = 0;
float right_stick_prev = 0;
double targetHeading = 0.0;
bool goingStraight = false;

int deadzoneX = 10;
int deadzoneY = 5;
int t = 18; //turningCurve --> change to adjust sensitivity of turning
int d = 2; //drivingCurve --> change to adjust sensitivity of forward / backward movement


float defaultDriveCurve(float x) {
    float a = -2.2f;
    float b = 303.0f;
    float c = 1.8f;
    float d = -0.5f;
    float g = 3.4f;
    bool negative = false;
    float outval = 0;

    if (x < 0) {
        negative = true;
        x = fabs(x);
    }
    if (x >= 0 && x <= 42.333) {
        outval = fabs((a * powf(x, c) - g * x) / (b * powf(x, d) + 1));
    } else if (x > 42.333) {
        outval = x;
    }
    if (negative) {
        outval = outval * -1;
    }
    return outval;
}


void switchArm(ArmPosition id) {

    printf("%d\n", id);
    arm.spinToPosition(armPositions[id], deg, false);
}

void driveConveyor(bool id) {
    if(id)
        conveyor.spin(forward, 100, percent);
    else
        conveyor.spin(forward, 0, percent);
}

void setDriveMotors(double lefty, double rightx, double rot) {

    double turnSens = 0.025;

    //Deadzone check
    if (abs(lefty) <= deadzoneY)
        lefty = 0;
    if(abs(rightx) <= deadzoneX)
        rightx = 0;

    //smoothing and analog curve equation
	right_stick_smoothed = ((std::exp(-t / 12.5102293) + std::exp((std::abs(rightx) - 132.55) / 69) * (1 - std::exp(-t / 10))) * rightx * 0.4) + (right_stick_prev * 0.6);
	left_stick_smoothed = ((std::exp(-d / 10) + std::exp((std::abs(lefty) - 100) / 10) * (1 - std::exp(-d / 10))) * lefty * 0.4) + (left_stick_prev * 0.6);
	right_stick_prev = right_stick_smoothed;
	left_stick_prev = left_stick_smoothed;
	//end smoothing

/*
    targetHeading += turnSens * right_stick_smoothed;
    double dAngle = targetHeading - rot;

    right_stick_smoothed = dAngle;
  
    printf("Current rot: %f,  Target rot: %f\n",rot, targetHeading);
*/

    // leftMotors.setVelocity(defaultDriveCurve(left_stick_smoothed, 4), percent);
    // rightMotors.setVelocity(defaultDriveCurve(left_stick_smoothed, 4), percent);
    leftMotors.setVelocity(defaultDriveCurve(left_stick_smoothed) + defaultDriveCurve(right_stick_smoothed), percent);
    rightMotors.setVelocity(defaultDriveCurve(left_stick_smoothed) - defaultDriveCurve(right_stick_smoothed), percent);
}
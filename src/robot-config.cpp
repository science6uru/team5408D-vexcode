#include "robot-config.hpp"

using namespace vex;

brain Brain;
controller Controller = controller(controllerType::primary);

#ifdef MOTOR_CONFIG_D
// motor backLeft = motor(PORT13, gearSetting::ratio6_1, true);
// motor frontLeft = motor(PORT19, gearSetting::ratio6_1, true);
// motor backRight = motor(PORT15, gearSetting::ratio6_1, false);
// motor frontRight = motor(PORT14, gearSetting::ratio6_1, false);
// inertial imu = inertial(PORT17, turnType::right);

// motor_group leftMotors = motor_group(backLeft, frontLeft);
// motor_group rightMotors = motor_group(backRight, frontRight);

double armPositions[NUM_ARM_POSITIONS] = {
0,      //Retracted,
120,    //PrepareScore,
200,    //ScoreHighGoal,
320,    //ScoreLowGoal,
400     //Descore,
};


#else
motor backLeft = motor(PORT17, gearSetting::ratio6_1, true);
motor frontLeft = motor(PORT18, gearSetting::ratio6_1, true);
motor backRight = motor(PORT13, gearSetting::ratio6_1, false);
motor frontRight = motor(PORT12, gearSetting::ratio6_1, false);

motor conveyor = motor(PORT2, gearSetting::ratio6_1, false);
motor arm = motor(PORT10, gearSetting::ratio36_1, false);

pneumatics pistonA = pneumatics(Brain.ThreeWirePort.A);
pneumatics pistonB = pneumatics(Brain.ThreeWirePort.B);

inertial imu = inertial(PORT11, turnType::right);

motor_group leftMotors = motor_group(/*backLeft,*/ frontLeft);
motor_group rightMotors = motor_group(backRight /*,frontRight*/);


double armPositions[NUM_ARM_POSITIONS] = {
0,      //Retracted,
120,    //PrepareScore,
200,    //ScoreHighGoal,
320,    //ScoreLowGoal,
400     //Descore,
};

#endif


void setupDriveMotors(motor_group& mg) {
    mg.setMaxTorque(100, percent);
    mg.setPosition(0, degrees);
    mg.setStopping(brakeType::brake);
    mg.setVelocity(0, percent);
    mg.spin(forward);
}

void setupArmMotor(motor m) {
    m.setMaxTorque(100, percent);
    m.setStopping(brakeType::hold);
    m.setPosition(0, degrees);
    m.setVelocity(0, percent);
    m.spin(forward);

    //Calibrate and zero the arm
    m.setVelocity(-30, rpm);
    while(m.torque(Nm) < 0.6)
    {
        if(m.torque(Nm) >= 0.6) break;
        //printf("%.2f\n",m.torque(Nm));
        wait(1, msec);
        printf("%f\n",m.position(deg));
    }
    m.setVelocity(0, rpm);
    wait(250, msec);
    m.setPosition(0, degrees);
    printf("%f\n",m.position(deg));
}
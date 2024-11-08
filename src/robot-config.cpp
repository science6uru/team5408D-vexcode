#include "robot-config.hpp"

using namespace vex;

brain Brain;
controller Controller = controller(controllerType::primary);

#ifdef MOTOR_CONFIG_D
motor backLeft = motor(PORT3, gearSetting::ratio6_1, true);
motor frontLeft = motor(PORT1, gearSetting::ratio6_1, true);
motor middleLeft = motor(PORT2, gearSetting::ratio6_1, true);

motor backRight = motor(PORT7, gearSetting::ratio6_1, false);
motor middleRight = motor(PORT5, gearSetting::ratio6_1, false);
motor frontRight = motor(PORT4, gearSetting::ratio6_1, false);

inertial imu = inertial(PORT12, turnType::right);

motor arm = motor(PORT19, gearSetting::ratio36_1, false);
motor conveyor = motor(PORT18, gearSetting::ratio36_1, false);

motor intake = motor(PORT21, gearSetting::ratio36_1, false);

pneumatics pistonA = pneumatics(Brain.ThreeWirePort.A);
pneumatics pistonB = pneumatics(Brain.ThreeWirePort.B);

motor_group leftMotors = motor_group(backLeft, frontLeft /*, middleLeft*/);
motor_group rightMotors = motor_group(backRight, frontRight /*, middleRight*/);

double armPositions[NUM_ARM_POSITIONS] = {
0,    //Retracted,
10,    //PrepareScore,
20,    //ScoreHighGoal,
30,    //ScoreLowGoal,
40     //Descore,
};

#else
motor backLeft = motor(PORT18, gearSetting::ratio6_1, true);
motor frontLeft = motor(PORT17, gearSetting::ratio6_1, true);
motor stackLeft = motor(PORT18, gearSetting::ratio6_1, true);

motor backRight = motor(PORT13, gearSetting::ratio6_1, false);
motor frontRight = motor(PORT12, gearSetting::ratio6_1, false);
motor stackRight = motor(PORT15, gearSetting::ratio6_1, false);

motor conveyor = motor(PORT2, gearSetting::ratio6_1, false);

motor arm = motor(PORT10, gearSetting::ratio36_1, false);

pneumatics pistonA = pneumatics(Brain.ThreeWirePort.A);
pneumatics pistonB = pneumatics(Brain.ThreeWirePort.B);

inertial imu = inertial(PORT11, turnType::right);

motor_group leftMotors = motor_group(backLeft, frontLeft, stackLeft);
motor_group rightMotors = motor_group(backRight ,frontRight, stackRight);

double armPositions[NUM_ARM_POSITIONS] = {
0,      //Retracted,
60,    //PrepareScore,
220,    //ScoreHighGoal,
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


void setupConveyorMotor(motor& m) {
    m.setMaxTorque(100, percent);
    m.setPosition(0, degrees);
    m.setStopping(brakeType::brake);
    m.setVelocity(0, percent);
    m.spin(forward);
}


void setupArmMotor(motor& m) {
    m.setMaxTorque(100, percent);
    m.setStopping(brakeType::hold);
    m.setPosition(0, degrees);
    m.setVelocity(0, percent);
    m.spin(forward);

    //Calibrate and zero the arm
    m.setVelocity(-30, rpm);
    while(m.torque(Nm) < 0.4)
    {
        if(m.torque(Nm) >= 0.4) break;
        printf("Calibrating arm [Torque]:%.2f\n",m.torque(Nm));
        wait(1, msec);
        printf("Calibrating arm [Degree]:%f\n",m.position(deg));
    }
    m.setVelocity(0, rpm);
    wait(250, msec);
    m.setPosition(0, degrees);
    printf("%f\n",m.position(deg));
}
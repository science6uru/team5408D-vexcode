#include "controller_button.h"

const vex::controller::button* inputTypeLookup[ControllerButtonType::NUM_BUTTON_TYPES] = {
    &Controller.ButtonA,
    &Controller.ButtonB,
    &Controller.ButtonX,
    &Controller.ButtonY,

    &Controller.ButtonUp,
    &Controller.ButtonDown,
    &Controller.ButtonLeft,
    &Controller.ButtonRight,
    
    &Controller.ButtonL1,
    &Controller.ButtonL2,
    &Controller.ButtonR1,
    &Controller.ButtonR2
};

Input* Input::inputs[ControllerButtonType::NUM_BUTTON_TYPES];
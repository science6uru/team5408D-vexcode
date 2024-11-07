#ifndef CONTROLLER_BUTON_H
#define CONTROLLER_BUTON_H
#include "vex.h"
#include "robot-config.hpp"

typedef unsigned int State;
typedef void (*ButtonEventHandler)(State _state);

#define DOUBLE_CLICK_DELAY 0.25
#define CLICK_DELAY 0.15

typedef enum {
    /** this uses binary logic so that each event can really mean different things
        example: a click is a press and then a release, so when a release makes a click,
        both the release and click have to be set.
        Release is 2 and Click is 6. 6 = 4 & 2 (second and third bits set in binary).
        The code that reads the value can check for Release OR Click and they'd both
        be correct. */
    Press = 1,       //  10000000
    Release = 2,     //  01000000
    Click = 6,       // 4 + Release       //  01100000
    DoublePress = 9, // 8 + Press   //  10010000
    DoubleClick = 22 // 16 + Click  //  01101000
} InputEventType;

#define CONTROLLER

typedef enum {
#ifdef CONTROLLER
    btButtonA = 0,
    btButtonB,
    btButtonX,
    btButtonY,

    btButtonUp,
    btButtonDown,
    btButtonLeft,
    btButtonRight,

    btButtonL1,
    btButtonL2,
    btButtonR1,
    btButtonR2,
#endif
    NUM_BUTTON_TYPES
} ControllerButtonType;

extern const vex::controller::button *inputTypeLookup[ControllerButtonType::NUM_BUTTON_TYPES];

class Input {
public:
    static Input *inputs[ControllerButtonType::NUM_BUTTON_TYPES];
    virtual void changed() = 0;
};


template <ControllerButtonType BT>
class ControllerButton : public Input {

    const vex::controller::button *source;
    double lastTime = 0.0;
    State state = 0;

public:
    ButtonEventHandler handler;

public:
    ControllerButton() : Input(), source(inputTypeLookup[BT]) {
        inputs[(int)BT] = this;
    }
    static void callback() {
        (inputs[BT])->changed();
    }

    bool checkState(InputEventType iet) {
        return iet & this->state;
    }

    void changed() {
        if (handler == 0) {
            return;
        }
        double now = Brain.Timer.value();
        double delay = now - this->lastTime;
        if (source->pressing()) {
            if ((this->state & InputEventType::Click) == InputEventType::Click && delay < DOUBLE_CLICK_DELAY) {
                this->state = InputEventType::DoublePress;
            } else {
                this->state = InputEventType::Press;
            }
        } else {
            if (this->state & InputEventType::Press && delay < CLICK_DELAY) {
                if (this->state == InputEventType::DoublePress) {
                    this->state = InputEventType::DoubleClick;
                } else {
                    this->state = InputEventType::Click;
                }
            } else {
                this->state = InputEventType::Release;
            }
        }
        this->lastTime = now;
        handler(this->state);
    }

    // this is where the actual callbacks are registered
    void registerHandler(ButtonEventHandler _handler) {

        /* The only way this works is because `callback` is a templated static method.
        The callback cannot have any parameters, but because it is a static method in
        a templated class, the function already knows what button it's for by the
        BT template parameter
        */
        this->source->pressed(this->callback);
        this->source->released(this->callback);
        this->handler = _handler;
    }
};


#define BUTTON_HELPER(which) \
ControllerButton<btButton ## which> button ## which; \
button##which.registerHandler (onButton ## which ## Changed);

#endif


#ifndef COMETA_GAMEPADINFO_H
#define COMETA_GAMEPADINFO_H

/**
* Gamepad info that can be retrieved from a Gamepad connected
*/
struct CometaGamepadInfo
{
    /**
    * State of each one of the gamepad buttons
    */
    unsigned char buttons[15];

    /**
    * Axes of the gamepad
    */
    float axes[6];
};

#endif //COMETA_GAMEPADINFO_H

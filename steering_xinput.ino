#include <PsxControllerBitBang.h>
#include <XInput.h>

const byte PIN_PS2_ATT = 9;
const byte PIN_PS2_CMD = 10;
const byte PIN_PS2_DAT = 11;
const byte PIN_PS2_CLK = 12;

const unsigned long POLLING_INTERVAL = 1000U / 50U;
PsxControllerBitBang<PIN_PS2_ATT, PIN_PS2_CMD, PIN_PS2_DAT, PIN_PS2_CLK> psx;

#ifdef ENABLE_SERIAL_DEBUG
    #define dstart(spd) do {Serial.begin (spd); while (!Serial) {digitalWrite (LED_BUILTIN, (millis () / 500) % 2);}} while (0);
    #define debug(...) Serial.print (__VA_ARGS__)
    #define debugln(...) Serial.println (__VA_ARGS__)
#else
    #define dstart(...)
    #define debug(...)
    #define debugln(...)
#endif

boolean haveController = false;

#define deadify(var, thres) (abs(var) > thres ? (var) : 0)

const byte ANALOG_DEAD_ZONE = 50U;

const boolean UseLeftJoystick   = true;
const boolean InvertLeftYAxis   = false;
const boolean UseRightJoystick  = false;
const boolean InvertRightYAxis  = false;

// Pin definitions for analog triggers
const int Pin_TriggerL = A0;
const int Pin_TriggerR = A1;

void setup () {
    // Initialize built-in LED
    pinMode(LED_BUILTIN, OUTPUT);

    // Init XInput library
    XInput.setJoystickRange(0, 255);
    XInput.begin();

    dstart(115200);
    debugln(F("Ready!"));
}

void loop () {
    static unsigned long last = 0;
    if (millis() - last >= POLLING_INTERVAL) {
        last = millis();
        
        if (!haveController) {
            if (psx.begin()) {
                debugln(F("Controller found!"));
                if (!psx.enterConfigMode()) {
                    debugln(F("Cannot enter config mode"));
                } else {
                    // Enable analog sticks
                    if (!psx.enableAnalogSticks()) {
                        debugln(F("Cannot enable analog sticks"));
                    }
                                    
                    if (!psx.exitConfigMode()) {
                        debugln(F("Cannot exit config mode"));
                    }
                }
                
                haveController = true;
            }
        } else {
            if (!psx.read()) {
                debugln(F("Controller lost :("));
                haveController = false;
            } else {
                byte x, y;

                // Buttons
                XInput.setButton(BUTTON_A, psx.buttonPressed(PSB_CROSS));
                XInput.setButton(BUTTON_B, psx.buttonPressed(PSB_CIRCLE));
                XInput.setButton(BUTTON_X, psx.buttonPressed(PSB_SQUARE));
                XInput.setButton(BUTTON_Y, psx.buttonPressed(PSB_TRIANGLE));

                XInput.setButton(BUTTON_LB, psx.buttonPressed(PSB_L1));
                XInput.setButton(BUTTON_RB, psx.buttonPressed(PSB_R1));

                XInput.setButton(BUTTON_BACK, psx.buttonPressed(PSB_SELECT));
                XInput.setButton(BUTTON_START, psx.buttonPressed(PSB_START));

                XInput.setButton(BUTTON_L3, psx.buttonPressed(PSB_L3));
                XInput.setButton(BUTTON_R3, psx.buttonPressed(PSB_R3));

                // D-Pad
                if (psx.buttonPressed(PSB_PAD_UP)) {
                    XInput.setDpad(true, false, false, false);
                    debugln(F("D-PAD UP pressed"));
                } else if (psx.buttonPressed(PSB_PAD_DOWN)) {
                    XInput.setDpad(false, true, false, false);
                    debugln(F("D-PAD DOWN pressed"));
                } else {
                    XInput.setDpad(false, false, false, false);
                }

                if (psx.buttonPressed(PSB_PAD_LEFT)) {
                    XInput.setDpad(false, false, true, false);
                    debugln(F("D-PAD LEFT pressed"));
                } else if (psx.buttonPressed(PSB_PAD_RIGHT)) {
                    XInput.setDpad(false, false, false, true);
                    debugln(F("D-PAD RIGHT pressed"));
                }

                // Left analog stick mapped to X/Y axes
                if (psx.getLeftAnalog(x, y)) {
                    XInput.setJoystickX(JOY_LEFT, x + 12);
                    XInput.setJoystickY(JOY_LEFT, y);
                    debug(F("Left Analog Stick X: "));
                    debug(x);
                    debug(F(", Y: "));
                    debugln(y);
                }

                // Right analog stick
                if (psx.getRightAnalog(x, y)) {
                    int8_t rx = ANALOG_IDLE_VALUE - x - 1;    // [127 ... -128]
                    rx = deadify(rx, ANALOG_DEAD_ZONE);
                    int8_t ry = ANALOG_IDLE_VALUE - y - 1;
                    ry = deadify(ry, ANALOG_DEAD_ZONE);

                    XInput.setJoystickX(JOY_RIGHT, x);
                    XInput.setJoystickY(JOY_RIGHT, y);
                    debug(F("Right Analog Stick X: "));
                    debug(x);
                    debug(F(", Y: "));
                    debugln(y);
                }

                // **Potentiometers as triggers**
                int triggerL = analogRead(Pin_TriggerL);  // Read left trigger pot
                int triggerR = analogRead(Pin_TriggerR);  // Read right trigger pot

                // Scale trigger values to XInput range (0-255)
                triggerL = map(triggerL, 0, 1023, 0, 255);
                triggerR = map(triggerR, 0, 1023, 0, 255);

                XInput.setTrigger(TRIGGER_LEFT, triggerL);
                XInput.setTrigger(TRIGGER_RIGHT, triggerR);

                // Debug potentiometer values
                debug(F("Left Trigger: "));
                debugln(triggerL);
                debug(F("Right Trigger: "));
                debugln(triggerR);

                // Send control data to the computer
                XInput.send();
            }
        }
    }
}

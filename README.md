# PS2 Steering Wheel to XInput Adapter

This project allows a PS2 Steering Wheel to interface with a computer as an XInput device. It uses the `PsxControllerBitBang` library to communicate with the PS2 controller and the `XInput` library to send input data to the computer. Additionally, it reads two potentiometers (analog triggers) as trigger inputs.

## Features

- Maps PS2 buttons to XInput buttons (A, B, X, Y, LB, RB, DPad etc.).
- Maps the PS2 left analog stick x-axis to XInput.
- Reads analog potentiometers and maps them to XInput trigger values (AS PS2 by default does not support analogue triggers)
- Supports analog stick dead zones and configurable joystick inversion.

## Components

### Hardware

- **PS2 Steering Wheel**: Connected using 4 pins (ATT, CMD, DAT, CLK). I'm using a Logic3 PS411.
- **Two Potentiometers**: Connected as analog inputs for the triggers (connected to pins `A0` and `A1`). In this case I'm using V3FX pedals connected directly to arduino.
- **Arduino Leonardo**: Handles communication between the PS2 controller and the computer.

### Hardware Setup

1. Connect the PS2 controller pins to your Arduino as follows:
   - **ATT (Attention)** → Pin 9
   - **CMD (Command)** → Pin 10
   - **DAT (Data)** → Pin 11
   - **CLK (Clock)** → Pin 12
![img](https://github.com/user-attachments/assets/b4396942-dcff-4d04-933a-61eb63facc11)
2. Connect two potentiometers to pins A0 (left trigger) and A1 (right trigger) on the Arduino.
3. Connect the Arduino to your computer via USB.

### Software Setup

1. **Install Libraries**:
   - Download and install the following libraries:
     - [PsxNewLib](https://github.com/SukkoPera/PsxNewLib)
     - [XInput Library for Arduino](https://github.com/dmadison/ArduinoXInput)\
   - Place the libraries in your Arduino `libraries` folder.
   
2. **Upload the Code**:
   - Open the provided `.ino` file in the Arduino IDE.
   - Select your board and port.
   - Upload the sketch to your Arduino.
   - Remember to use the upload procedure [here](https://github.com/dmadison/ArduinoXInput_AVR), instead of the default arduino upload procedure.

### Customization

- **Dead Zone**: The dead zone for the analog sticks can be adjusted using the `ANALOG_DEAD_ZONE` constant.
- **Joystick Inversion**: You can invert the X-axis of the left joystick by setting `InvertLeftXAxis`to `true`.

## License

This project is licensed under the **GPL v3** license. See the [LICENSE](https://www.gnu.org/licenses/gpl-3.0.en.html) file for more details.

## Acknowledgments

This project makes use of the following open-source libraries:

- [PsxNewLib](https://github.com/SukkoPera/PsxNewLib) by SukkoPera: A library to interface with PlayStation 1/2 controllers.
- [XInput Library for Arduino](https://github.com/dmadison/ArduinoXInput) by dmadison: A library for implementing Xbox 360-compatible XInput controllers on Arduino.

---

Enjoy using your PS2 Steering Wheel as an XInput device! If you encounter any issues, feel free to contribute or open an issue in the repository.

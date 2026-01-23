# mkl05z32-metronome
A digital metronome project implemented on the **NXP FRDM-KL05Z** development board (Cortex-M0+). The device allows users to set a tempo (BPM) via a serial terminal and provides both visual (LED) and audible (loudspeaker) feedback.

## Features
* **Precise Tempo Generation**: Range from 30 to 240 BPM
* **UART Communication**: Control the metronome using commands (ON, OFF, UP, DOWN, BPM value) via any serial terminal (e.g., PuTTY)
* **Dual Signaling**: Synchronized LED flashes and loudspeaker beeps
* **LCD Feedback**: Real-time display of current status and BPM value on a 16x2 LCD

## Hardware Setup
To run this project, connect the peripherals to the FRDM-KL05Z board as follows:

**LCD 16x2**:
|   FRDM-KL05Z  | LCD |
|---------------|-----|
|     PTB3      | SCL |
|     PTB4      | SDA |
|     +5V       | VCC |
|     GND       | GND |

**Loudspeaker WSR-04489**:
|   FRDM-KL05Z  | WSR-04489 |
|---------------|-----------|
|     PTB8      |    VCC    |
|     +5V       |    +5V    |
|     GND       |    GND    |

## Software Commands
Connect via Serial Terminal  (Settings: **28800 baud, 8N1**). Use the following commands:

*Note: Ensure your terminal sends a Carriage Return (CR) after each command*

* `ON` - Start the metronome
* `OFF` - Stop the metronome
* `UP` - Increase tempo by 5 BPM
* `DOWN` - Decrease tempo by 5 BPM

## Manual Tempo Entry
* `[number]` - Directly set a specific tempo (e.g., type `123` and press `Enter`)
    * **Range:** 30 to 240 BPM
    * **Validation:** The system automatically rejects non-numeric characters and values outside the safe range

## Project Structure
* `inc/` - Header files
* `src/` - C implementation files
* `project/` - Keil uVision project files

## Getting Started
1. Clone this repository
2. Open `project/*.uvprojx` in **Keil uVision 5**
3. Ensure you have the **Kinetis KLxx Series Device Family Pack** installed
4. Build the project and flash it to the board using the J-Link OpenSDA debugger
5. Open your terminal of choice and start practicing

## Debugger Configuration (J-Link OpenSDA)
This board uses **J-Link OpenSDA firmware**. To flash and debug:
1. Open **Options for Target** (magic wand icon) -> **Debug** tab
2. Select **J-Link/J-Trace Cortex** from the drop-down list
3. Click **Settings** and verify that the device is recognized in the **SW Device** list and the **Max Clock** is set to **2MHz**
4. In the **Flash Download** tab, ensure `Reset and Run` is checked
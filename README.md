# IT4210E Lab 1 submission package

Student details retained from the report guide:
- Nguyễn Đăng Dương — 202417117
- Nguyễn Quốc Tuấn — 202417213

## Package contents

Four independent STM32F429ZIT6 projects are supplied, each with its own Core,
Drivers, CubeMX .ioc configuration and STM32CubeIDE metadata:

| Project | Function |
| --- | --- |
| Exercise_3_1_LED | Four LED effects, two cycles each, then next effect, 150 ms per frame |
| Exercise_3_2_7Segment | Starts at decimal 25; B1 increments modulo 100; fast LED activity |
| Exercise_3_3_NEC | Standard NEC decoding and serial command output |
| Exercise_4_RemoteApplication | Raw command-byte display, numbered LED effects, OK toggles LED4 |

The DOCX explains the added code and answers all Exercise 3.3 questions.
The projects retain ST's original copyright/license notices. OpenAI Codex assisted
with the implementation, automated tests and report drafting. Students must
review and verify the work and declaration before submission.

## Build and flash each separate project

1. Extract this complete package to a writable directory. Do not move the
   STM32CubeIDE subdirectory away from its sibling Core and Drivers directories.
2. In STM32CubeIDE 2.x choose File > Import > General > Existing Projects into
   Workspace. Select this extracted directory and enable searching nested projects.
   Select the four projects. Leave "Copy projects into workspace" unchecked:
   their linked resources are relative to the parent of STM32CubeIDE.
3. Build each project in Debug configuration with the IDE's Arm GNU toolchain.
   No prebuilt firmware is included. The original starter .elf was deliberately
   excluded so it cannot be mistaken for an exercise build.
4. Connect the STM32F429 Discovery board through ST-LINK, choose the matching
   project and use Debug/Run to flash. The target is STM32F429ZIT6, 2 MiB flash.
5. Confirm HSE is 8 MHz, CPU 180 MHz, APB1 45 MHz, timer clock 90 MHz, TIM6
   prescaler 89 and period 99. The delivered code already configures these.

Build the delivered sources directly. CubeMX regeneration is optional and can
require preserving the added source files and their links in .project. The .ioc
files record the exercise pin selections, but regeneration has not been run here.
The application initializes its added peripherals in AppInit() after generated
initialization, inside the main.c user-code startup path.

## Wiring

Power off the board while wiring. Use the lab guide's resistor/transistor circuit.

| Function | MCU pins | Connection |
| --- | --- | --- |
| Eight LEDs | PD8 to PD15 | Bit 0 to bit 7; active high; current-limiting resistor per LED |
| Seven segments | PE8 to PE14 | a, b, c, d, e, f, g, through the guide's 330-ohm resistors |
| Decimal point | PE15 | Connected but kept off |
| Tens digit enable | PG2 | NPN transistor base through 10-kohm resistor |
| Units digit enable | PG3 | NPN transistor base through 10-kohm resistor |
| B1 | PA0 | Onboard user button; rising-edge EXTI |
| HS0038 output | PG5 | Falling-edge EXTI; idle high |
| HS0038 power | 3 V and GND | Verify the receiver module's actual pinout |
| Serial TX | PA2 | To RX of an external 3.3 V USB-UART adapter |
| Serial RX | PA3 | Optional, from adapter TX; application does not use RX |
| Serial ground | GND | Common ground with adapter |
| LED4 | PG14 | Onboard red LED; toggled only by OK |

The seven-segment circuit is common cathode with active-high segment lines and
active-high NPN digit enables, matching Figure 2 in the lab PDF. Do not substitute
common-anode displays without adjusting the code and circuit. If LED row direction
is reversed physically, reverse the physical order of the row to match PD8 on the
right and PD15 on the left.

## Calibrate the remote before Exercise 4

Use Exercise_3_3_NEC first. In Hercules or another serial terminal, select the
USB-UART adapter's port, 115200 baud, 8N1, no hardware flow control. Press each
remote key separately. Output is four bytes as eight hexadecimal characters,
followed by CR/LF, for example 00FF0CF3. The third byte is 0C.

Edit Core/Inc/lab_config.h in Exercise_4_RemoteApplication to match your remote:

| Key | Configurable example command byte |
| --- | --- |
| 1 | 0x0C |
| 2 | 0x18 |
| 3 | 0x5E |
| 4 | 0x08 |
| OK | 0x1C |

Only the first two values occur in the reference report. None of these are newly
measured hardware results. Rebuild and reflash after changing the definitions.
The decoder accepts standard NEC with complementary address and command bytes.
Extended NEC and NEC repeat packets are not decoded as new commands. Ignoring
repeat packets prevents repeated OK toggles while a key is held.

Exercise 4 shows the actual command byte in hexadecimal 00-FF because a byte can
exceed 99. For example, the button whose code is 0x0C produces "0C", not "01".
Exercise 3.2 uses decimal 00-99 and wraps 99 to 00.

## Verification performed

Run `python3 tests/run_tests.py` with clang installed. It:
- Tests the real lab_logic.c LED patterns, segment encodings, button debounce,
  all 256 NEC commands, timing variation, invalid complements, repeats, timeout
  and recovery.
- Runs app.c against a minimal host HAL stub to verify command byte display,
  selection of modes 1-4, unknown-key behavior and OK-only PG14 toggling.
- Verifies every CubeIDE linked resource resolves after unpacking.
- Checks application syntax for Cortex-M4 and compiles application and linked HAL
  C sources to real ARM objects for all four projects.

See VERIFICATION.txt for the executed test output. These checks passed here.
A final Arm GNU firmware link, CubeIDE import/regeneration, flashing, electrical
operation, display brightness/ghosting, actual remote mappings and hardware timing
were not tested here. No claim of physical execution is made.

## Required videos — still to record

The assignment requires real videos showing each exercise executing on the board,
with your student ID card clearly visible in every video. No videos were supplied
or created; the package is incomplete for final submission until you record them.
Place the four recordings in videos/ and recreate the ZIP before submitting.

Suggested checklist:

1. Exercise 3.1: Show all four effects, each repeated twice, and the return to
   mode 1. About 35 seconds captures the full programmed sequence.
2. Exercise 3.2: Show 25 after reset, individual B1 presses incrementing once,
   a held button not repeating, fast LED activity, and 99 wrapping to 00.
3. Exercise 3.3: Show the receiver/remote and serial terminal together. Press
   keys 1, 2, 3, 4 and OK and show their decoded frames.
4. Exercise 4: Show command bytes on the display, keys 1-4 selecting effects,
   and two separate OK presses turning LED4 on and off. Hold OK to confirm it
   does not repeatedly toggle from NEC repeat packets.

Take your own hardware photos/screenshots if your instructor expects experimental
images in addition to the theoretical graph and source excerpts in the report.
Update the report's verification paragraph with your actual results after testing.
Check the names/IDs and declaration, then submit the DOCX together with the ZIP.

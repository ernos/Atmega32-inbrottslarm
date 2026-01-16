**CheapSec — AVR ATmega32 Burglar Alarm**

Overview generated with copilot after instructing it to analyze source files. Original documentation which was written at the time this project was created (2010?) Will be availible in `documentations/` folder.
- **Purpose:** Simple burglar-alarm firmware for an ATmega32-based device. Provides keypad input, LCD status display, serial (USART) comms, and a simple menu system.
- **MCU:** ATmega32 (AVR)

Features
- **Keypad input:** numeric keypad handling and menu navigation.
- **LCD status:** character LCD driver for status and prompts.
- **USART:** serial logging/communication.
- **F-Bus / comm:** inter-module communication routines.
- **Menu:** on-device menu for configuration and operations.

Repository files (key ones)
- **Main:** [main.c](main.c) and [main.cpp](main.cpp) — C and C++ variants present; the active entrypoint depends on the chosen build.
- **Communication:** [comm.cpp](comm.cpp), [comm.h](comm.h)
- **F-Bus:** [F-Bus.c](F-Bus.c), [F-Bus.h](F-Bus.h), [F-Bus.cpp](F-Bus.cpp)
- **LCD:** [lcd.cpp](lcd.cpp), [lcd.h](lcd.h)
- **Keyboard:** [keyboard.h](keyboard.h)
- **Menu:** [Menu.cpp](Menu.cpp), [Menu.h](Menu.h), [Menu.txt](Menu.txt)
- **USART:** [usart.c](usart.c), [usart.h](usart.h), [usart.cpp](usart.cpp)
- **Headers:** [General.h](General.h), [Int0.h](Int0.h), [main.h](main.h)
- **Build artifacts & debug:** [Debug/Makefile](Debug/Makefile) and build outputs in `Debug/`.

Hardware assumptions
- Runs on an ATmega32 (5V) MCU clocked as configured in the Makefile/Debug build.
- Expects a character LCD (HD44780-style) connected to the MCU data and control pins used by `lcd.*`.
- Expects a matrix keypad or simple keypad input wired to pins referenced by `keyboard.h`.
- Uses USART for serial output (TX/RX pins) as implemented in `usart.*`.
- External interrupts (e.g., `Int0`) may be used for sensors — see `Int0.h`.

Build & flash
- Prerequisites: `avr-gcc`, `avr-libc`, `avr-binutils`, `avrdude` (or your preferred programmer toolchain).
- Typical build (use the provided Makefile):

```bash
cd Debug
make
```

- Flash with `avrdude` (example for an USBasp programmer; adjust MCU and programmer):

```bash
avrdude -c usbasp -p m32 -U flash:w:CheapSec\ v0.1.srec:i
```

Notes and configuration
- There are both C and C++ source files; pick the variant supported by the provided `Makefile` in `Debug/`.
- Configuration constants (pin assignments, timing) are defined in header files such as [General.h](General.h) — edit those to adapt to your hardware.

Where to look to modify behavior
- Key handling and menu: [keyboard.h](keyboard.h), [Menu.cpp](Menu.cpp), [Menu.h](Menu.h)
- Display text and layout: [lcd.cpp](lcd.cpp), [lcd.h](lcd.h)
- Communication protocols: [comm.cpp](comm.cpp), [comm.h](comm.h), [F-Bus.c](F-Bus.c)
- Serial debug: [usart.c](usart.c), [usart.h](usart.h)

Testing & debugging
- Use a serial console at the configured baud rate to observe logs from `usart`.
- If you change pin mappings, re-check `General.h` and `main.*` wiring code.

License & authorship
- This repository does not include an explicit license file; assume "all rights reserved" unless you add a `LICENSE`.

Contact / next steps
- If you want, I can:
- - Run a quick code scan to produce a concise wiring diagram.
- - Update the Makefile to build only the C or C++ variant.

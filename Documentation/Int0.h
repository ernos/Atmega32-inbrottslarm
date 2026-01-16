/*
The External Interrupts are triggered by the INT0, INT1, and INT2 pins. Observe that, if enabled,
the interrupts will trigger even if the INT0..2 pins are configured as outputs. This feature provides
a way of generating a software interrupt. The external interrupts can be triggered by a falling or
rising edge or a low level (INT2 is only an edge triggered interrupt). This is set up as indicated in
the specification for the MCU Control Register – MCUCR – and MCU Control and Status Register
– MCUCSR. When the external interrupt is enabled and is configured as level triggered (only
INT0/INT1), the interrupt will trigger as long as the pin is held low. Note that recognition of falling
or rising edge interrupts on INT0 and INT1 requires the presence of an I/O clock, described in
“Clock Systems and their Distribution” on page 24. Low level interrupts on INT0/INT1 and the
edge interrupt on INT2 are detected asynchronously. This implies that these interrupts can be
used for waking the part also from sleep modes other than Idle mode. The I/O clock is halted in
all sleep modes except Idle mode.
Note that if a level triggered interrupt is used for wake-up from Power-down mode, the changed
level must be held for some time to wake up the MCU. This makes the MCU less sensitive to
noise. The changed level is sampled twice by the Watchdog Oscillator clock. The period of the
Watchdog Oscillator is 1 ?s (nominal) at 5.0V and 25°C. The frequency of the Watchdog Oscillator
is voltage dependent as shown in “Electrical Characteristics” on page 287. The MCU will
wake up if the input has the required level during this sampling or if it is held until the end of the
start-up time. The start-up time is defined by the SUT fuses as described in “System Clock and
Clock Options” on page 24. If the level is sampled twice by the Watchdog Oscillator clock but
disappears before the end of the start-up time, the MCU will still wake up, but no interrupt will be
generated. The required level must be held long enough for the MCU to complete the wake up to
trigger the level interrupt.
 */ 
# MSP430 PWM Frequency Generator

This repository contains MSP430 C code for generating a PWM output signal using Timer A.

The program configures the MSP430 to output a selected frequency on pin P1.6. It was written as helper code for testing MSP430-based tag switching frequencies.

## File

- `msp430_freq.c`: MSP430 C program that configures Timer A to generate a PWM signal.

## What the Code Does

The code:

- Stops the watchdog timer
- Sets the MSP430 DCO clock to 16 MHz
- Configures P1.6 as a Timer A output pin
- Uses Timer A to generate a PWM signal
- Calculates the timer period for a desired frequency
- Selects a Timer A input divider if the timer count is too large
- Sets the duty cycle to approximately 50%
- Enters low-power mode while Timer A continues running

## How the Frequency Is Set

The function `setPWMFrequency()` takes a desired frequency in Hz.

Example:

`setPWMFrequency(87912);`

This tells the MSP430 to generate a PWM signal near 87.912 kHz, but can be changed for any desired frequency.

The timer period is based on the 16 MHz SMCLK clock:

`PWM frequency = SMCLK / (divider * (TACCR0 + 1))`

The code calculates `TACCR0` from the desired frequency and uses a divider of 1, 2, 4, or 8 depending on whether the timer count fits within the 16-bit Timer A limit.

## Duty Cycle

The code sets `TACCR1` to approximately half of the timer period:

`TACCR1 = (TACCR0 + 1) / 2`

This creates an approximately 50% duty-cycle PWM signal, meaning the output is high for about half of each period and low for about half of each period.

## Notes

This repository focuses only on MSP430 timer configuration and PWM generation. It does not include the frequency-selection code or the full experimental system.

The actual output frequency should be checked on hardware with an oscilloscope.

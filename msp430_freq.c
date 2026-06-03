#include <msp430.h>

#define SMCLK_FREQ 16000000UL

void setPWMFrequency(unsigned long desiredFreq)
{
    unsigned int dividerBits = ID_0;
    unsigned long dividerValue = 1;
    unsigned long ccr0;

    if (desiredFreq == 0)
        return;   // avoid divide by zero

    // Try /1 divider
    ccr0 = (SMCLK_FREQ / (dividerValue * desiredFreq)) - 1;

    // If TACCR0 is too large, try /2
    if (ccr0 > 65535)
    {
        dividerBits = ID_1;
        dividerValue = 2;
        ccr0 = (SMCLK_FREQ / (dividerValue * desiredFreq)) - 1;
    }

    // If still too large, try /4
    if (ccr0 > 65535)
    {
        dividerBits = ID_2;
        dividerValue = 4;
        ccr0 = (SMCLK_FREQ / (dividerValue * desiredFreq)) - 1;
    }

    // If still too large, try /8
    if (ccr0 > 65535)
    {
        dividerBits = ID_3;
        dividerValue = 8;
        ccr0 = (SMCLK_FREQ / (dividerValue * desiredFreq)) - 1;
    }

    // If frequency is too low, clamp to slowest possible
    if (ccr0 > 65535)
    {
        ccr0 = 65535;
    }

    TACCR0 = (unsigned int)ccr0;              // Set PWM period
    TACCR1 = (unsigned int)((ccr0 + 1) / 2); // 50% duty cycle

    TACTL = TASSEL_2 | dividerBits | MC_1 | TACLR;
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;     // Stop watchdog timer

    // Set DCO to calibrated 16 MHz
    DCOCTL = 0;
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;

    P1DIR |= BIT6;                // P1.6 output
    P1SEL |= BIT6;                // Timer A controls P1.6

    TACCTL1 = OUTMOD_7;           // Reset/set PWM mode

    setPWMFrequency(87912);       // Desired frequency in Hz

    __bis_SR_register(LPM0_bits); // Sleep while timer runs
}
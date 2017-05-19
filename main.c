#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

static inline void initInterrupt(void)
{
    GIMSK |= (1 << PCIE);   // pin change interrupt enable
    PCMSK |= (1 << PCINT2); // pin change interrupt enabled for PCINT2
    sei();                  // enable interrupts
}


/*Confusing below but there is only one pinchange interrupt vector, called PCINT0_vect
this covers all pinchange interrupts. Not just PCINT0 or something
*/

ISR(PCINT0_vect)
{   uint8_t x;
    PORTB = 0b00000000;
    _delay_ms(100);
    for(x=1; x<10; x++){
        PORTB = 0b00001100;
        _delay_ms(11);
        PORTB ^= (1 << PB3) | (1 << PB4);
        _delay_ms(11);
    }  
}

int main(void)
{
    DDRB = 0b00011000;   // enable PB3,PB4 as outputs
    PORTB |= (1 << PB2); // enable pullup on pushbutton output and PCINT2 interrupt (be careful moving forward with changing PB2!!)
    initInterrupt();
    while(1){
        PORTB |= (1 << PB3) | (1 << PB4);
        _delay_ms(250);
        PORTB ^= (1 << PB3) | (1 << PB4);
        _delay_ms(250);
    }
    return 0;
}

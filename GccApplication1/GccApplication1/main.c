    #define F_CPU 8000000UL
    
    #include <avr/io.h>
    #include <stdio.h>
    #include <util/delay.h>
    #include <avr/interrupt.h>
    
    #define led_ddr DDRD
    #define led_prt PORTD
    #define led_pin PORTD4
    
    int main (void) {
	    
	    led_ddr |= (1 << led_pin);
	    led_prt |= (1 << led_pin); 
		
    }
    
   
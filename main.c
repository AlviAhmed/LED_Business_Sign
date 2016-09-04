    #define F_CPU 8000000UL
    
    #include <avr/io.h>
    #include <stdio.h>
    #include <util/delay.h>
    #include <avr/interrupt.h>
    
    #define tot_ddr DDRD
    #define tot_prt PORTD
    #define data_pin PD5 
	#define clock_pin PD6 
	#define latch_pin PD7 
	int count = 0;
	
    
    int main (void) { 
		tot_ddr |= ((1 << data_pin) | (1 << clock_pin) | (1 << latch_pin));
		tot_prt &= ~ ((1 << data_pin) | (1 << clock_pin) | (1 << latch_pin));
		TCCR1B |= (1 << WGM12); // ctc mode configuration
		TIMSK1 |= (1 << OCIE1A); // enable interrupt 
		sei();
		OCR1A = 6249; //setting compare value
		TCCR1B |= (1 << CS12 ); //prescaler 256
	    while (1)
	    {  
		 if (count == 0){ 
			tot_prt |= (1 << data_pin); 
			tot_prt |= (1 << latch_pin);	 
		 } 
		 if (count > 0 ){
			 tot_prt &= ~ (1 << data_pin); 
			 tot_prt |= (1 << latch_pin);
		 }
		 }
		
    }
    
	
	ISR (TIMER1_COMPA_vect){
		tot_prt ^= (1 << clock_pin);
		tot_prt &= ~ (1 << latch_pin); 
		if (count > 33){
			count = -1;
		}
		count ++;
		
	}
	
	
   
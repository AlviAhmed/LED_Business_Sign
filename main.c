    #define F_CPU 8000000UL
    
    #include <avr/io.h>
    #include <stdio.h>
    #include <util/delay.h>
    #include <avr/interrupt.h>
    
    #define tot_ddr DDRD
    #define tot_prt PORTD

	
	//Pins
	#define	lst_led	PD2
    #define data_pin PD5 
	#define clock_pin PD6 
	#define latch_pin PD7    
	#define sec1 	PD4 
	#define sec2   PD3

	//Pins on
	#define sec1_on  		tot_prt &= ~ (1 << PD4) 
	#define sec2_on			tot_prt &= ~ (1 << PD3)
	#define	lst_led_on		tot_prt |= (1 << PD2)  
	#define	data_pin_on		tot_prt |= (1 << PD5)  
	#define	clock_pin_on	tot_prt |= (1 << PD6)  
	#define clock_pin_tog 	tot_prt ^= (1 << PD6) 
	#define	latch_pin_on	tot_prt |= (1 << PD7) 
	
	//Pins off 
	#define sec1_off  		tot_prt |= (1 << PD4) 
	#define sec2_off 		tot_prt |= (1 << PD3)
	#define	lst_led_off	 	tot_prt &= ~ (1 << PD2)  
	#define	data_pin_off	 tot_prt &= ~ (1 << PD5)  
	#define	clock_pin_off	 tot_prt &= ~ (1 << PD6)  
	#define	latch_pin_off 	tot_prt &= ~ (1 << PD7) 
	
	int count = 0, sec_num = 0;
	
	void ioinit();
    void timersettings(); 
    void latchpulse();

    int main (void) {   

    	ioinit(); 
    	timersettings(); 
    	clock_pin_on;
		
		while (1)
	    { 
	    	if ( sec_num == 0 ){ 
	    		sec1_on; 
	    		sec2_off; 
	    	} 
	    	else { 
	    		sec2_on; 
	    		sec1_off;
	    	}
	   	


			if ( PIND & (1<<PD6) ){ 
				if (count == 0){ 
					data_pin_on; 
					lst_led_off;
				} 

				if (count > 0){ 
					data_pin_off;
				}
			} 
			else { latchpulse(); count++; }

			if ( (count > 32) && (PIND & (1<<PD6)) ){ 
				lst_led_on; 
				count = 0; 
				if (sec_num == 0){ 
					sec_num = 1;
				} 
				else{sec_num = 0;}
			}	


		}
			
    
    }
    
	
	ISR (TIMER1_COMPA_vect){
		clock_pin_tog;
	} 

	
	void ioinit () { 
		tot_ddr |= ((1 << data_pin) | (1 << clock_pin) | (1 << latch_pin) | (1 << sec1) | (1 << sec2) | (1 << lst_led) ); 
		tot_prt &= ~ ((1 << data_pin) | (1 << clock_pin) | (1 << latch_pin) | (1 << lst_led)); 
	} 
	void timersettings () {  
		TCCR1B |= (1 << WGM12); // ctc mode configuration
		TIMSK1 |= (1 << OCIE1A); // enable interrupt 
		sei();
		OCR1A = 6249; //setting compare value
		TCCR1B |= (1 << CS12 ); //prescaler 256

	} 

	
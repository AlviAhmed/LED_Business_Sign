   #define F_CPU 8000000UL
   
   #include <avr/io.h>
   #include <stdio.h>
   #include <util/delay.h>
   #include <avr/interrupt.h>
   
   
   #define ddr1 DDRD
   #define prt1 PORTD  
   #define ddr2 DDRC 
   #define prt2 PORTC

   #define data_pin (1 << PD5)
   #define clock_pin (1 << PD6) 
   #define latch_pin (1 << PD7)
   #define sec1 (1 << PD4)
   #define sec2 (1 << PD3) 
   #define sec3 (1 << PC5) 
   #define sec4 (1 << PC4)
   #define lst_led (1 << PD2) 
	  
   int clk_index = 0, on_led = 4, num1 = 32, num2 = 65, y = 0; 
   int sec_array[] = {0,14,32,47,65};
   // Section 1 : LEDS  0-14 
   // Section 2 : LEDS 15-32 
   // Section 3 : LEDS 33-47 
   // Section 4 : LEDS 48-65
   
   int main (void) { 
   	//Initialization of Ports
	ddr1 |= ( data_pin | clock_pin | latch_pin | sec1 | sec2 | lst_led);
	ddr2 |= ( sec3 | sec4 ); 

	//Initial Values of Variables before main function 

	   prt1 &= ~ ( data_pin | clock_pin | latch_pin | lst_led);  
	   prt2 &= ~ ( sec3 | sec4);
	   prt1 |= ( sec1 | sec2 ); 
	   
	   TCCR1B |= (1 << WGM12); // ctc mode configuration
	   TIMSK1 |= (1 << OCIE1A); // enable interrupt
	   sei();
	   OCR1A = 6249; //setting compare value
	   TCCR1B |= (1 << CS12 ); //prescaler 256
	   while (1)
	   {  
		    for (y = 0; y <= on_led; y ++){ 
		    	switch ( sec_array[y] ){ 
		    		case 14:  
		    			prt1 |= sec1;  
		    			prt2 &= ~ sec3; 
		    		case 32:  
		    			lastled();
		    			prt1 |= lst_led;  
		    			prt2 ^= sec3; 
		    			prt1 ^= sec1;  
		    		case 47: 
		    			prt2 |= sec4;
		    			prt1 &= ~ sec2; 
		    		case 65: 
		    			lastled(); 
		    			prt1 ^= sec1;
		    			prt2 ^= sec3; 
		    			clk_index = 0; 
		    			y = 0;}}}}
   
   
   ISR (TIMER1_COMPA_vect){  
	  if (clk_index < on_led){ 
	  	prt1 |= data_pin;
	  } 
	  else { prt1 |= data_pin;}  
	  latchpulse();
	  clk_index++;
	   
   } 
   
   void latchpulse (){
	   prt1 |= latch_pin;  
	   prt1 &= ~ latch_pin;
	} 

	void lastled() {  

		prt1 |= lst_led; 
		if (clk_index == (num1 + 4)){  
			prt1 &= ~ lst_led;
		} 
		if ( clk_index == ( (num2 - (num2 + 1)) + 4) ){ 
			prt1 &= ~ lst_led;
		}

	}

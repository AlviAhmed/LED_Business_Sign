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
   int index = 0, on_led = 4, lst_lednum = 32, lst_led2num = 65;
   // Section 1 : LEDS  0-14 
   // Section 2 : LEDS 15-32 
   // Section 3 : LEDS 33-47 
   // Section 4 : LEDS 48-65
   
   int main (void) { 
   	//Initialization of Ports
	ddr1 |= ( data_pin | clock_pin | latch_pin | sec1 | sec2 | lst_led);
	ddr2 |= ( sec3 | sec4 ); 

	//Initial Values of Variables before main function 

	   prt1 &= ~ ( data_pin | clock_pin | latch_pin | sec2 );  
	   prt2 &= ~ ( sec3 | sec4 )
	   prt1 |= (1 << sec1); 
	   
	   TCCR1B |= (1 << WGM12); // ctc mode configuration
	   TIMSK1 |= (1 << OCIE1A); // enable interrupt
	   sei();
	   OCR1A = 6249; //setting compare value
	   TCCR1B |= (1 << CS12 ); //prescaler 256
	   while (1)
	   {  
		     if (sec_num == 0 && bark == 1){
			     tot_prt &= ~ (1 << sec2); //Turn on the next section of leds
			     tot_prt |= (1 << sec1); //Turn off the next section of leds
			     sec_num = 1; 
				 mark = 0; 
				 bark = 0;
		     }
		     if (sec_num == 1 && bark == 1){
			     tot_prt &= ~ (1 << sec1);
			     tot_prt |= (1 << sec2);
			     sec_num = 0; 
				 mark = 0; 
				 bark = 0;
		     }
		  
		   
	   }
	   
   }
   
   
   ISR (TIMER1_COMPA_vect){  
	   //
	   if (count == 0){
		   tot_prt |= (1 << data_pin); 
		   tot_prt |= (1 << clock_pin);
		}
	   if (count > 0 ){
		   tot_prt &= ~ (1 << data_pin);
		   tot_prt |= (1 << clock_pin); 
		}   
		//
		tot_prt &= ~ (1 << clock_pin);
		latchpulse();
		count ++; 
		tot_prt &= ~ (1 << lst_led); 
		if (mark == 1){
			bark = 1;
		}
		if (count > 32){ 
		   tot_prt |= (1 << lst_led); 
		 count = 0; 
		  mark = 1;
		}
	   
   } 
   
   void latchpulse (){
	   tot_prt |= (1 << latch_pin);
	   tot_prt &=~ (1 << latch_pin);
	}

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
   #define sec1 PD4
   #define sec2 PD3 
   #define lst_led PD2
   int count = 0,sec_num = 0,mark=0,bark = 0;
   
   
   int main (void) {
	   tot_ddr |= ((1 << data_pin) | (1 << clock_pin) | (1 << latch_pin) | (1 << sec2) | (1 << sec1) | (1 << lst_led));
	   tot_prt &= ~ ((1 << data_pin) | (1 << clock_pin) | (1 << latch_pin) | (1 << sec1) ); 
	   tot_prt |= (1 << sec2); 
	   
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
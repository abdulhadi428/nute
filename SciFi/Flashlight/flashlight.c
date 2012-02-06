/* -----------------------------------------------------------------------
 * Title: randomly-switching-off-LED v2.1.5 program PWM, ADC-based random PCINT, for P-channel transistor
 * Hardware: ATtiny13
 * Software: WinAVR 20060421
 -----------------------------------------------------------------------*/
 
 #define PWM_ON 4 //PWM-high-level-time in ms
 #define PWM_OFF 1 //PWM-low-level-time in ms. PWM_ON and PWM_OFF summ should be equal to 5
 #define LED_ON_MIN 5 //min LED-glowing-time in seconds
 #define LED_ON_RANGE 10 //in seconds max LED-glowing-time = LED_ON_MIN+LED_ON_RANGE
 #define JOLTS_MIN 5 //min number of jolts requiered to restart the LED. 
 #define JOLTS_RANGE 6 //max number of jolts will be JOLTS_MIN+JOLTS_RANGE 
 #define GOODCYCLES 5 //first cycles while the LED brights without any interruption
 #define BLINKCYCLES 5 //next cycles while the LED blinks after each one
 #define BLINKTIME 500 //ms, time, while the LED is off

 #define F_CPU 1000000 // clock 1MHz, internal oscillator
 
 #include <avr/io.h>
 #include <util/delay.h>
 #include <avr/interrupt.h> 
 #include <avr/eeprom.h>
 
 #define LCRST PB0 // LifeCycles ReSeT pin
 #define LED PB1 //(can be changed to OC0B) main LED output
 #define ADC PB2 //pin n.7 for getting random number from ADC noise
 #define gLED PB3 //jolt-indicator output
 #define SWT PB4 //(PCINT4) jolt-sensor input
 #define EE_ADDRESS  0x010  /* Define address constant for EEPROM data*/ 

 
 uint16_t time;
 char jolt;
 char i; 
 uint16_t cycles;
 

 int randomBitRaw() {
  uint8_t copyAdmux, copyAdcsra, copyAdcsrb, copyPortb, copyDdrb;
  uint16_t i;
  uint8_t bit;
  volatile uint8_t dummy;
  
  // Store all the registers we'll be playing with
  copyAdmux = ADMUX;
  copyAdcsra = ADCSRA;
  copyAdcsrb = ADCSRB;
  copyPortb = PORTB;
  copyDdrb = DDRB;
  
  // Perform a conversion on Analog0, using the Vcc reference
  ADMUX |= (0 << REFS0);
  ADMUX |= (0 << MUX1) | (1 << MUX0); // ADC1 PB.2

  ADCSRA |= (1 << ADEN); // Enable ADC   

  // Autotriggering disabled
  ADCSRB = 0;

  // Pull Analog input ADC to ground
  PORTB &=~_BV(ADC);
  DDRB |= _BV(ADC);
  // Release Analog input, apply internal pullup
  DDRB &= ~_BV(ADC);
  PORTB |= _BV(ADC);
  // Immediately start a sample conversion on Analog input ADC
  ADCSRA |= _BV(ADSC);
  // Wait for conversion to complete
  while (ADCSRA & _BV(ADSC)) PORTB ^= _BV(ADC);
  // Xor least significant bits together
  bit = ADCL;
  // We're ignoring the high bits, but we have to read them before the next conversion
  dummy = ADCH;

  // Restore register states
  ADMUX = copyAdmux;
  ADCSRA = copyAdcsra;
  ADCSRB = copyAdcsrb;
  PORTB = copyPortb;
  DDRB = copyDdrb;

  return bit & 1;
 }

int randomBitRaw2(void) {
  // Software whiten bits using Von Neumann algorithm
  //
  // von Neumann, John (1951). "Various techniques used in connection
  // with random digits". National Bureau of Standards Applied Math Series
  // 12:36.
  //
  for(;;) {
    int a = randomBitRaw() | (randomBitRaw()<<1);
    if (a==1) return 0; // 1 to 0 transition: log a zero bit
    if (a==2) return 1; // 0 to 1 transition: log a one bit
    // For other cases, try again.
  }
}

int randomBit(void) {
  // Software whiten bits using Von Neumann algorithm
  //
  // von Neumann, John (1951). "Various techniques used in connection
  // with random digits". National Bureau of Standards Applied Math Series
  // 12:36.
  //
  for(;;) {
    int a = randomBitRaw2() | (randomBitRaw2()<<1);
    if (a==1) return 0; // 1 to 0 transition: log a zero bit
    if (a==2) return 1; // 0 to 1 transition: log a one bit
    // For other cases, try again.
  }
}

uint16_t randomWord(void) {
  uint16_t result;
  uint16_t i;
  result = 0;
  for (i=16; i--;) result += result + randomBit();
  return result;
}

 
 int main(void) {
    
  time=LED_ON_MIN+LED_ON_RANGE; //ok, for the first time it will be the maximum possible value :)
  jolt=JOLTS_MIN; 
  
//  srand(123); // init randomizer
 
  PCMSK |= (1<<PCINT4); //  tell pin change mask to listen to pin6
  GIMSK  |= (1<<PCIE); // enable PCINT interrupt in the general interrupt mask
   
  DDRB &= ~(1<<SWT); // Set input direction on SWT (INT0)
  PORTB |= (1<<SWT); //pull-up resisitor on switch

  DDRB &= ~(1<<LCRST); // Set input direction on LCRST
  PORTB |= (1<<LCRST); //pull-up resisitor on LCRST    

  DDRB |= (1 << LED); // Set direction register output
  DDRB |= (1 << gLED); // Set direction register output
  
  if (bit_is_clear(PINB, LCRST)){
    
   cycles=GOODCYCLES+BLINKCYCLES;
   eeprom_write_word(EE_ADDRESS,cycles);

    for (i=1; i<10; i++){  //reinit blinking
     PORTB &= ~(1 << LED);   
     _delay_ms(500);     
     PORTB |= (1 << LED); 
     _delay_ms(500);    
    }


  }else{
   cycles=eeprom_read_word(EE_ADDRESS);   
  }
   
  for (;;){
  
   if (time>0){
    for (i=1; i<200; i++){  //A cycle of one hungred PWM-periods.
     PORTB |= (1 << LED); //so we can use seconds to define glowing time.
     _delay_ms(PWM_OFF);    //
     PORTB &= ~(1 << LED);   //As each PWM-period takes ~5ms (PWM_ON+PWM_OFF)
     _delay_ms(PWM_ON);     //the whole cycle is executed in ~1s
    }
  
    time--;                 //decreasing time-to-glow
   }else{
    
    //current cycle is over, let's see what to do now    
               
        
    if (cycles==0){
     PORTB |= (1 << LED);   
     sei(); // no more EEPROM, only interrupts.  
     //worst case
     if ((PORTB >> gLED) & 1){  //PB3 is over jolt-marker. It it's on, so a jolt was cptured
      jolt--;                  //so we decrease the number of jolts to restart the LED
      _delay_ms(100);          // and wait a little, so the gLED glowing could be seen,
      PORTB &= ~(1 << gLED);   //then we turn gLED off.
      if (jolt==0){            //If no more jolts requiered
       time=LED_ON_MIN+(randomWord() % LED_ON_RANGE); //the whole process
       jolt=JOLTS_MIN+(randomWord() % JOLTS_RANGE);   //restarts with a new random count betwean JOLTS_MIN and JOLTS_MIN+JOLTS_RANGE 
      }
     }                
    }else{        
        
     //best and middle cases
        
     if (cycles<BLINKCYCLES){ 
       //BLINK TIME!        
       
       PORTB |= (1 << LED);
       _delay_ms(BLINKTIME);              
     }
     
     time=LED_ON_MIN+(randomWord() % LED_ON_RANGE);
     
     cycles--;
     eeprom_write_word(EE_ADDRESS,cycles); 

    }    
   }   
  } 
 }
 
 ISR (PCINT0_vect){              //when a jolt occures                       
   if (~bit_is_clear(PINB, LED)){ //if the main LED is switched off
    PORTB |= (1 << gLED);      //the gLED should be swiched on.
   }
 }
 
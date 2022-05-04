#include "device_config.h"
#include <stdint.h>
#include <math.h>
#include <xc.h>

////+++++++++++++++++++++++++++++++++++++| LIBRARIES / HEADERS |+++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++| DIRECTIVES |+++++++++++++++++++++++++++++++++++++
#define _XTAL_FREQ 8000000
#define SWEEP_STEP 1

//+++++++++++++++++++++++++++++++++++++| DATA TYPES |+++++++++++++++++++++++++++++++++++++
enum por_ACDC {digital, analog};          // digital = 0, analog = 1
uint32_t num = 0x10101010;
////+++++++++++++++++++++++++++++++++++++| ISRs |+++++++++++++++++++++++++++++++++++++
//// ISR for high priority
//void __interrupt ( high_priority ) high_isr( void );
//// ISR for low priority
//void __interrupt ( low_priority ) low_isr( void ); 

//+++++++++++++++++++++++++++++++++++++| FUNCTION DECLARATIONS |+++++++++++++++++++++++++++++++++++++
void portsInit(void);
uint8_t char_to_seg(uint8_t);
void send_to_disp(uint32_t);
char key_scanner(void);

//+++++++++++++++++++++++++++++++++++++| MAIN |+++++++++++++++++++++++++++++++++++++
void main(void){
    portsInit();
      //uint32_t num = 0x0A0B0C0D;
   // send_to_disp(num);
    while(1){
  
        uint32_t key = key_scanner();
        
        while((~PORTB & 0x0F)!=0){
            num = num<<8; 
            num = num+key;
            __delay_ms(750);
            
        }
        send_to_disp(num);
    }
}

//+++++++++++++++++++++++++++++++++++++| FUNCTIONS |+++++++++++++++++++++++++++++++++++++
void portsInit(void){
    ANSELB = digital;   // Set port A as Digital for keypad driving
    TRISB  = 0x0F;      // For Port A, set pins 4 to 7 as outputs (rows), and pins 0 to 3 as inputs (cols)
    WPUB = 0x0F;        // pull-up enable for PORTB pins
    INTCON2bits.RBPU=0;
    ANSELA = digital;   // Set port B as Digital for 7 segment cathode selection (only 4 pins used)  
    TRISA  = 0x00;      // For Port B, set pins as outputs for cathode selection
    ANSELD = digital;   // Set port D as Digital for 7 segment anodes
    TRISD  = 0x00;      // for Port D, set all pins as outputs for 7 segment anodes
    OSCCON = 0b01110100;// Set the internal oscillator to 8MHz and stable
}

void send_to_disp(uint32_t disp_word){
    for (char i = 0; i < 4; i++){
        int internal_sweep = (int) pow(2, i);
        int sweep = 0x0F & ~internal_sweep;
        LATA = (char) sweep;
        uint8_t num_disp = 0x000000FF & (disp_word >> i*8);
        LATD = char_to_seg(num_disp);
        __delay_ms(SWEEP_STEP);
    }
}

uint8_t char_to_seg(uint8_t num){
    uint8_t segments;
    switch(num){
        case 0:  segments = 0b00111111; break;
        case 1:  segments = 0b00000110; break;
        case 2:  segments = 0b01011011; break;
        case 3:  segments = 0b01001111; break;
        case 4:  segments = 0b01100110; break;
        case 5:  segments = 0b01101101; break;
        case 6:  segments = 0b01111101; break;
        case 7:  segments = 0b00000111; break;
        case 8:  segments = 0b01111111; break;
        case 9:  segments = 0b01100111; break;
        case 10: segments = 0b01110111; break;
        case 11: segments = 0b01111100; break;
        case 12: segments = 0b01011000; break;
        case 13: segments = 0b01011110; break;
        case 14: segments = 0b01111001; break;
        case 15: segments = 0b01110001; break;
        default: segments = 0b00000000; break;
    }
    return segments;
}

char key_scanner(){
    
    LATB=0b01110000;    
    if(PORTBbits.RB3==0)
    { 
        return 1;
    }
    if(PORTBbits.RB2==0)
    {
        return 2;
    }    
    if(PORTBbits.RB1==0)
    {
        return 3;
    }
    if(PORTBbits.RB0==0)
    {
        return 10;
    }
        LATB = 0; 
        
    LATB=0b10110000;    
    if(PORTBbits.RB3==0)
    { 
        return 4;
    }
    if(PORTBbits.RB2==0)
    {
        return 5;
    }    
    if(PORTBbits.RB1==0)
    {
        return 6;
    }
    if(PORTBbits.RB0==0)
    {
        return 11;
    }
    LATB = 0;
    
    LATB=0b11010000;    
    if(PORTBbits.RB3==0)
    { 
        return 7;
    }
    if(PORTBbits.RB2==0)
    {
        return 8;
    }    
    if(PORTBbits.RB1==0)
    {
        return 9;
    }
    if(PORTBbits.RB0==0)
    {
        return 12;
    }
    LATB = 0; 
    
    LATB=0b11100000;    
    if(PORTBbits.RB3==0)
    {
        return 15;
    }
    if(PORTBbits.RB2==0)
    {
        return 0;
    }    
    if(PORTBbits.RB1==0)
    {
        return 14;
    }
    if(PORTBbits.RB0==0)
    {
        return 13;
    }
    
}
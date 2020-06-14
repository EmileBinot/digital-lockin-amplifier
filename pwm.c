

#include "p30f4011.h"
#include <p30f4011.h>
#include <libpic30.h>
#include "common.h"
#include <xc.h>

void impulsion(void);
// Sets the duty cycle on PWM channels 1 & 2
void set_pwm_duty_cycle(double duty_cycle)
{
    PDC1 = duty_cycle * (2 * PTPER);
    PDC2 = (1 - duty_cycle) * (2 * PTPER);
}
 
// This function sets up digital i/o, analog input, PWM,
// UART and timer interrupt
void configure_pins()
{
    // Configure all four port D pins (RD0, RD1, RD2, RD3)
    // as digital outputs
    LATD = 0;
    TRISD = 0b1111111111110000;
 
    // Configure all three port C pins (RC13, RC14, RC15)
    // as digital inputs
    TRISC = 0b1111111111111111;
 
    // Configure PWM for free running mode
    //
    //   PWM period = 2 * Tcy * prescale * PTPER = 0.66ns * 1 * PTPER
    //   PWM pulse width = Tcy * prescale * PDC
    
    PWMCON1 = 0b00110011; // Enable channels 1 & 2 in complementary mode
    PTCON = 0;
    _PTCKPS = 0;      // prescale=1:1 (0=1:1, 1=1:4, 2=1:16, 3=1:64)
    _PTMOD = 0b10;    // Select up-down counting for centre-aligned PWM
    PTPER = 600;      // 25kHz in up/down counting mode
    PDC1 = PTPER;     // 50% duty cycle on PWM channel 1
    PDC2 = PTPER;     // 50% duty cycle on PWM channel 2
    PTMR = 0;         // Clear 15-bit PWM timer counter
    _PTEN = 1;        // Enable PWM time base
}

/*
CONTROLE DE GAIN AUTOMATIQUE
On définit des seuils qui permettent de changer le gain de l'AOP en entrée en fonction de AmplitudePWM. Au total, 4 niveaux permettent de cahnger le gain de l'AOP.
*/
void controlgain(void){
    
    // Make RD0 a digital output
    _TRISD0=0;
    _TRISD1=0;
    _TRISD2=0;
    _TRISD3=0;
    // Blink LED on RD0
  
        if((AmplitudePWM<1.0)){
        _LATD0 = 1;
        _LATD1 = 0;
        _LATD2 = 0;
        _LATD3 = 0;
       
    }
    __delay32(1500000);
    
    
    if((AmplitudePWM<46000)&&(AmplitudePWM>46100)){
        _LATD0 = 0;
        _LATD1 = 1;
        _LATD2 = 0;
        _LATD3 = 0;
        
    }
    __delay32(1500000);
    
    if((AmplitudePWM<46200) && (AmplitudePWM>46100) ){
        _LATD0 = 0;
        _LATD1 = 0;
        _LATD2 = 1;
        _LATD3 = 0;
        
    
    }
    __delay32(1500000);
    
    if((AmplitudePWM<46300.0)&&(AmplitudePWM>46100)){
        _LATD0 = 0;
        _LATD1 = 0;
        _LATD2 = 0;
        _LATD3 = 1;
        
  
    }
    __delay32(1500000);
}

void impulsion(void){
     TRISF = 0b1111111111111111;
    
    _TRISF0=0;
    
    _LATF0=1;
    __delay_ms(50);
    _LATF0=0;
    __delay_ms(50);
    
}

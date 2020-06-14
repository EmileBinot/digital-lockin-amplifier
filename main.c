/* LA CONVERSION SE FAIT SUR AN3*/
/*PWM 0*/

#include "p30f4011.h"
#include <libpic30.h>
#include <p30f4011.h>
#include "common.h"
#include <xc.h>
#include <stdint.h>


 
_FOSC(CSW_FSCM_OFF & FRC_PLL16); // Fosc=16x7.5MHz, Fcy=30MHz
_FWDT(WDT_OFF);                  // Watchdog timer off
_FBORPOR(MCLR_DIS);              // Disable reset pin

float AmplitudePWM;

int main (void)
{
    
    float satValue=46000.0;     //SatValue est la valeur a laquelle la sortie n'est plus proportionelle a l'entrée
        ADC_Init();             //Initialize the A/D converter
        Timer_Init();           //init Timer
        configure_pins();       //init pwm

        // Configure RD0 and RD1 as outputs -- pour TESTER le timing
        TRISD = 0b1100;
        while (1){
            controlgain();
            if(Amplitude>satValue){
                AmplitudePWM=1.0;
            }
            else{
                AmplitudePWM=(Amplitude-41000)/(satValue-41000);
               // AmplitudePWM=0.3;
            }
            //set_pwm_duty_cycle(0.75);
            set_pwm_duty_cycle(AmplitudePWM);
        
            impulsion();
        }
        
        return 0;
}
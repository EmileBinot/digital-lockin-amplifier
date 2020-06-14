
#include "p30f4011.h"
#include <p30f4011.h>
#include <math.h>
#include "common.h"
#include <stdint.h>

unsigned short compteurTimer=0;

unsigned short MultResI=0;
unsigned short MultResQ=0;

long ResI_filtered=0;
long ResQ_filtered=0;

long SortieI;
long SortieQ;

float Amplitude;

signed char TabI[8]={-1,-1,-1,-1,+1,+1,+1,+1};
signed char TabQ[8]={+1,+1,-1,-1,-1,-1,+1,+1};

int ATabI[8]={0,0,0,0,1,1,1,1};
int ATabQ[8]={1,1,0,0,0,0,1,1};

void Timer_Init(void){

    // Configure Timer 1.
    // PR1 and TCKPS are set to call interrupt every 1.25usec 
    // Period = PR1 * prescaler * Tcy = 469 * 8 * 33.33ns = 1.2505uSec
    //FOR A SAMPLING RATE OF 8Khz
    
    T1CON = 0;            // Clear Timer 1 configuration
    T1CONbits.TCKPS = 2;  // Set timer 1 prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)
    PR1 = 117;          // Set Timer 1 period (max value is 65535)
    _T1IP = 1;            // Set Timer 1 interrupt priority
    _T1IF = 0;            // Clear Timer 1 interrupt flag
    _T1IE = 1;            // Enable Timer 1 interrupt
    T1CONbits.TON = 1;    // Turn on Timer 1
}


void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void)
{
    unsigned short K=10;
    int i;
    // Clear Timer 1 interrupt flag
    _T1IF = 0;
 
    //Turn on the A/D converter LANCEMENT CONVERSION A/D
    ADCON1bits.ADON = 1;
    
    //On gere le compteur d'intterruptions du Timer (Pour la multiplication)
    compteurTimer++;
    //Ainsi, TabI et TabQ sont basiquement des signaux carrés de même frequence que le sinus que nous voulons capter.
    if (compteurTimer == 8){
        compteurTimer=0;
    }
    
    MultResI=ADResult*TabI[compteurTimer];//MULTIPLICATION DES I ET Q
    MultResQ=ADResult*TabQ[compteurTimer];//en long

    ResI_filtered=MultResI+ResI_filtered-(ResI_filtered>>K);//Filtrage I et Q (voir equation de filtrage)
    ResQ_filtered=MultResQ+ResQ_filtered-(ResQ_filtered>>K);
    
    SortieI=(ResI_filtered>>K);//Voir equation de filtrage 
    SortieQ=(ResQ_filtered>>K);
    
   /* if(i==800){*/
        Amplitude = sqrtf(SortieI*SortieI+SortieQ*SortieQ);
      /*  i=0;//POUR NE FAIRE L'OPERATION QUE 10 FOIS /SEC
    }
    i++;*/
            
            
    //_LATD1 = ATabI[compteurTimer];
    //_LATD0 = ATabQ[compteurTimer];
    
}
/* 
 * File:   lcd.c
 * Author: ae063157
 *
 * Created on 16 novembre 2021, 14:35
 */


#define FCY 32000000
#include <libpic30.h>

#include <xc.h>
//#include "lcd.h"

#include <stdio.h>
#include <stdlib.h>

#include <time.h>


#define LCD_EN PORTDbits.RD4
#define LCD_RS PORTBbits.RB15




void delay(){
    unsigned int i;
    for(i=0;i<5000;i++);
}

//unsigned char ligne1[]={'T','e','m','p','e','r','a','t','u','r','e',' ','r','e',' ',' '};
//unsigned char ligne2[]={'e','s','t',' ','d','e',' ',' ',' ',' ',' ',' ',' ',' ',' ','|'};

void sendCommand(unsigned char command){
    LCD_RS = 0;
    delay();
    LCD_EN = 1;
    delay();
    PORTE = command;
    delay();
    LCD_EN = 0;
    delay();
}

void sendData(unsigned char data){
    LCD_RS = 1;
    delay();
    LCD_EN = 1;
    delay();
    PORTE = data;
    delay();
    LCD_EN = 0;
    delay();
}

char ligne1[16];
char ligne2[16];

int main(int argc, char** argv){

    unsigned char i, s;
    TRISE = 0x00;
    TRISDbits.TRISD4 = 0;
    TRISBbits.TRISB15 = 0;
    
    for(i=0;i<16;i++){
        ligne1[i] = ' ';
    }
    for(i=0;i<16;i++){
        ligne2[i] = ' ';
    }
    
    int tempsensor = 8;
    TRISBbits.TRISB4 = 1;    //mettre en input RB4
    ANSBbits.ANSB4 = 1;      //mettre en analogue RB4 -> AN4 
    
    AD1CON1 = 0;
    AD1CON1bits.MODE12 = 0; 
    AD1CON1bits.FORM = 0;
    AD1CON1bits.SSRC = 0b0111; 
    AD1CON1bits.ASAM = 1; 
    AD1CON2 = 0; 
    AD1CON3 = 0;
    AD1CON3bits.ADRC = 1; 
    AD1CON3bits.SAMC = 2; 
    AD1CON3bits.ADCS = 2; 
    AD1CON5 = 0;
    AD1CHS = 0;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ADON = 1; 
    
    sendCommand(0x38);
    sendCommand(0x0E);
    sendCommand(0x01);
    
    while(1){
        
        
       
        tempsensor = ADC1BUF0;
        double coeff = 0.004882;
        double tempamb = ((tempsensor*coeff)*125)/5;
        
        sprintf(ligne1, "temp : %.1f C    ", tempamb);
        
        
        sprintf(ligne2, "time: %s    ",__TIME__);
        
        /*
        RCFGCALbits.RTCPTR = 3;;    
        asm volatile("MOV #NVMKEY, W1");    
        asm volatile("MOV #0x55, W2");    
        asm volatile("MOV W2, [W1]");    
        asm volatile("MOV #0xAA, W3");    
        asm volatile("MOV W3, [W1]");    
        asm volatile("BSET RCFGCAL, #13");       
        RTCVAL  = 0x0010;      
        RTCVAL = 0x0930;       c
        RTCVAL = 0x0510;      
        RTCVAL = 0x0509; 
        */
        
        
        sendCommand(0x80);
        for(i=0;i<16;i++){
            s = ligne1[i];
            sendData(s);
        }
        delay();
        
        //sprintf(ligne2, "time : ");
        sendCommand(0xc0);
        for(i=0;i<16;i++){
            s = ligne2[i];
            sendData(s);
        }
        delay();
        
        
    }
    return (EXIT_SUCCESS);
}



/*
 
 sendcommand(0x38);
 sendcommand(0x0E);
 sendcommand(0x01);
 * 
void InitLCD_b( void)
{
    AD1PCFG = 0xFFE0;
    TRISE = 0x0000;
    TRISDbits.TRISD4=0;
    TRISDbits.TRISD5=0;
    TRISB = 0x001F;

    PORTDbits.RD5=0;
    PORTBbits.RB15=0;
    PORTDbits.RD4=0;

    // init TMR1
    T1CON = 0x8030;

    PMDATA = 0b00111000;
    PORTDbits.RD4=1;
    TMR1 = 0; while( TMR1<3);   // 3 x 16us = 48us
    PORTDbits.RD4=0;



    PMDATA = 0b00001100;
    PORTDbits.RD4=1;
    TMR1 = 0; while( TMR1<3);   // 3 x 16us = 48us
    PORTDbits.RD4=0;



    PMDATA = 0b00000001;
    PORTDbits.RD4=1;
    TMR1 = 0; while( TMR1<3);   // 3 x 16us = 48us
    PORTDbits.RD4=0;

    TMR1 = 0; while( TMR1<100);   // 3 x 16us = 48us


    PMDATA = 0b00000110;
    PORTDbits.RD4=1;
    TMR1 = 0; while( TMR1<3);   // 3 x 16us = 48us
    PORTDbits.RD4=0;

   // TMR1 = 0; while( TMR1<200);   // 3 x 16us = 48us

} // InitLCD
//
void PutLCD_b(char kar)
{
    PORTDbits.RD5=0;
    PORTBbits.RB15=1;
    PORTDbits.RD4=0;
    //TMR1 = 0; while( TMR1<1);   // 3 x 16us = 48us
    PMDATA = kar;
    //TMR1 = 0; while( TMR1<1);   // 3 x 16us = 48us
    PORTDbits.RD4=1;
    TMR1 = 0; while( TMR1<3);   // 3 x 16us = 48us
    PORTDbits.RD4=0;

}
//
void putsLCD_b( char *s)
{
    while( *s) PutLCD_b( *s++);
} //putsLCD


void newLine()
{
    PORTDbits.RD5=0;
    PORTBbits.RB15=0;
    PORTDbits.RD4=0;

    PMDATA = 0xC0;
    PORTDbits.RD4=1;
    TMR1 = 0; while( TMR1<3);   // 3 x 16us = 48us
    PORTDbits.RD4=0;
}

void LCDHome()
{
    PORTDbits.RD5=0;
    PORTBbits.RB15=0;
    PORTDbits.RD4=0;

    PMDATA = 0x02;
    PORTDbits.RD4=1;
    TMR1 = 0; while( TMR1<3);   // 3 x 16us = 48us
    PORTDbits.RD4=0;
    TMR1 = 0; while(TMR1<60);
}
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 *  for(i=0;i<16;i++){
        array[i] = ' ';
    }
    int ADCValue =7;
    TRISBbits.TRISB4 = 1;    //mettre en input RB4
    ANSBbits.ANSB4 = 1;      //mettre en analogue RB4 -> AN4 
    AD1CON1 = 0;
    AD1CON1bits.MODE12 = 0; 
    AD1CON1bits.FORM = 0;
    AD1CON1bits.SSRC = 0b0111; 
    AD1CON1bits.ASAM = 1; 
    AD1CON2 = 0; 
    AD1CON3 = 0;
    AD1CON3bits.ADRC = 1; 
    AD1CON3bits.SAMC = 2; 
    AD1CON3bits.ADCS = 2; 
    AD1CON5 = 0;
    AD1CHS = 0;
    AD1CHSbits.CH0SA = 8; 
    sprintf(array, "v =  %d\r\n",ADCValue); 
    AD1CON1bits.ADON = 1; 
    
 * 
 * sendCommand(0xc0);
        for(i=0;i<16;i++){
            s = array[i];
            sendData(s);
        }
        delay();

*/

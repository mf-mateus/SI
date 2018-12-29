/*
 * File:   main.c
 * Author: maran
 *
 * Created on December 27, 2018, 10:38 PM
 */

#include <xc.h>
#include "config.h"
#include <libpic30.h>
#include <stdio.h>
#include <math.h>

//======== Functions Initialize ========//
inline void ConfigADC(void); // Configures Analog To Digital
inline void ConfigUART1(unsigned long int brg); // Serial Link Via Uart
inline void ConfigTMR1(void);
unsigned int readADC(unsigned int ch); // Read ADC values input[PIN]

//======== Global Vars =========//
unsigned int acq0, acq1, acq2, acq3;

int main(void){
    // Configurar oscillador para 32MHz
    CLKDIVbits.DOZE = 0; // 1:1
    CLKDIVbits.RCDIV = 0; // 8 MHz
    
    ANSA = ~0x0; // RA0 RA1 analog e os outros todos
    ANSB = ~0x084;         // RX1, TX1, digitais, RB0,RB1 analog e restantes analogicos;
    TRISA= 0XFFFF; // Porta A inputs
    TRISB= 0xFFFF; // Porta B inputs
    ConfigUART1(19200);
    ConfigADC();
    ConfigTMR1();
    
    while(1){
        acq0=readADC(0); // Pin 2 canal AN0
        acq1=readADC(1); // Pin 3 canal AN1
        acq2=readADC(2); // Pin 4 canal AN2
        acq3=readADC(3); // Pin 5 canal AN3
        //values_array=[acq0,acq1,acq2,acq3];
        printf("<%u,%u,%u,%u>\n",acq0,acq1,acq2,acq3);
        __delay_ms(500);
    }
}

//======== Functions Code ========//

//==== Read Value From Pin Convert to Digital 12bits ====//
unsigned int readADC(unsigned int ch){
    AD1CHS = ch;            // Select analog input channel
    AD1CON1bits.SAMP = 1;   // start sampling, then go to conversion

    while (!AD1CON1bits.DONE); // conversion done?
    return(ADC1BUF0);       // yes then get ADC value
}

//==== Timer Config 1s ====//
inline void ConfigTMR1(void){
    T1CON = 0x8030; // activa TMR1, prescale 1:256, relogio interno
    TMR1 = 0; // inicializa o registo do timer 1
    PR1 = 62500 - 1;   // 1.000 ms Tick (625000 not working?)
    _T1IF = 0;
    _T1IE = 1; //Activa Interrupts
}

//==== Serial Link Comms ====//
inline void ConfigUART1(unsigned long int brg){
    U1BRG = (FCY / (16 * brg)) -1; // 19200 bps @ 16 MHz -> BRG = 51
    U1MODE= 0x8000; // UARTEN
    U1STA = 0x0400; // enable transmission
}// UART1Init

//==== Analog to Digital Config ====//
inline void ConfigADC(void){
    AD1CON1 = 0x0470; // 12-bit A/D operation (MODE12=1)
                      // SSRC<3:0> = 0111 Internal counter ends sampling
                      // and starts conversion (auto-convert)
    AD1CON2 = 0x0000; // Configure A/D voltage reference and buffer fill modes.
                      // Vr+ and Vr- from AVdd and AVss(PVCFG<1:0>=00, NVCFG=0),
                      // Inputs are not scanned,
                      // Buffer in FIFO mode (BUFM=0)
    AD1CON3 = 0x1003; // Sample time = 16Tad, Tad = 4Tcy (= 250ns)
    AD1CSSL = 0; // No inputs are scanned.
    _AD1IF = 0; // Clear A/D conversion interrupt.
    _AD1IE = 0; // Disable A/D conversion interrupt
    AD1CON1bits.ADON = 1; // Turn on A/D
}

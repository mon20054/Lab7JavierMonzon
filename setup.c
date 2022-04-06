/* 
 * File:   setup.c
 * Author: josej
 *
 * Created on 28 maart 2022, 16:32
 */

#include <xc.h>

void setup(void){
    ANSEL = 0;
    ANSELH = 0;                 // I/O digitales
    
    TRISA = 0x00;               // PORTA como salida
    PORTA = 0;                  // Limpiamos PORTA
    TRISC = 0x00;               // PORTC como salida
    PORTC = 0;                  // Limpiamos PORTC
    TRISD = 0x00;               // PORTD como salida
    PORTD = 0;                  // Limpiamos PORTD
    TRISB = 3;                  // RB0 y RB1 como entradas
    PORTB = 0;                  // Limpiamos PORTB
    
    OSCCONbits.IRCF = 0b0110;   // 4MHz
    OSCCONbits.SCS = 1;         // Oscilador interno
    
    OPTION_REGbits.nRBPU = 0;   // Habilitamos resistencias de pull-up del PORTB
    WPUBbits.WPUB0 = 1;         // Habilitamos resistencia de pull-up de RB0
    WPUBbits.WPUB1 = 1;         // Habilitamos resistencia de pull-up de RB1
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones globales
    INTCONbits.RBIE = 1;        // Habilitamos interrupciones del PORTB
    INTCONbits.T0IE = 1;        // Habilitamos interrupciones del TMR0
    IOCBbits.IOCB0 = 1;         // Habilitamos interrupción por cambio de estado para RB0
    IOCBbits.IOCB1 = 1;         // Habilitamos interrupción por cambio de estado para RB1
    INTCONbits.RBIF = 0;        // Limpiamos bandera de interrupción
    
    
    OPTION_REGbits.T0CS = 0;    // TIMR0 como temporizador
    OPTION_REGbits.T0SE = 0;  
    OPTION_REGbits.PSA = 0;     // Prescaler asignado a TMR0
    OPTION_REGbits.PS2 = 0;     // Prescaler = 2
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 0;
    TMR0 = 6;                 // Delay de 0.5 mS
}


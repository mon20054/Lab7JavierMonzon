/* 
 * File:   main.c
 * Author: josej
 *
 * Created on April 4, 2022, 3:13 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdint.h>
#include "setup.h"

/*------------------------------------------------------------------------------
 * PROTOTIPO DE FUNCIONES 
 ------------------------------------------------------------------------------*/
void setup(void);
void R_TMR0(void);
void obtener_valor(void);
void set_display(void);
void mostrar_valor(void);

/*------------------------------------------------------------------------------
 * CONSTANTES 
 ------------------------------------------------------------------------------*/
#define _XTAL_FREQ  1000000     
#define B0 PORTBbits.RB0
#define B1 PORTBbits.RB1


/*------------------------------------------------------------------------------
 * VARIABLES 
 ------------------------------------------------------------------------------*/
 uint8_t cont1 = 0;     // Declarada e inicializada
 uint8_t cont2 = 0;     // Declarada e inicializada
 uint8_t valores[3];    // Declarada
 uint8_t display[3];    // Declarada
 uint8_t valor;         // Declarada
 uint8_t i;             // Declarada
 uint8_t banderas = 0;  // Declarada e inicializada
 uint8_t tabla[10] = {0b11101101,
                      0b01100000,
                      0b11001110,
                      0b11101010,
                      0b01100011,
                      0b10101011,
                      0b10101111,
                      0b11100000,
                      0b11101111,
                      0b11100011};         
 

 void __interrupt() isr (void){
    if(INTCONbits.RBIF){            // Fue interrupción del PORTB
        if(!B0){                    // Verificamos si fue RB0 quien generó la interrupción
            cont1++;                 // Incremento del contador 
        }
        if(!B1){                    // Verificamos si fue RB1 quien generó la interrupción
            cont1--;                 // Decremento del contador
        }

        INTCONbits.RBIF = 0;        // Limpiamos bandera de interrupción
    }
    
    if(INTCONbits.T0IF){
        mostrar_valor();
        R_TMR0();                 // Reset de TMR0
    }
    
    return;
}
 
 void main(void) {
    setup();
    while(1){
        PORTA = cont1;
        PORTC = cont2;
        valor = cont1;
        obtener_valor();
        set_display();
    }
    return;
 }
 
 void R_TMR0(void){
     TMR0 = 6;                // Delay de 0.5 mS
     INTCONbits.T0IF = 0;
     return;
 }
 
 void obtener_valor(void){
     valores[0] = valor/100;                            // Obtener centenas del valor
     valores[1] = (valor-valores[0]*100)/10;            // Obtener decenas del valor
     valores[2] = valor-valores[0]*100-valores[1]*10;   // Obtener unidades del valor
 }
 
 void set_display(void){
     i = valores[0];
     display[0] = tabla[i];      // Display con centenas
     i = valores[1];
     display[1]= tabla[i];       // Display con decenas
     i = valores[2];
     display[2] = tabla[i];      // Display con unidades
 }
 
 void mostrar_valor(void){
     //PORTD = 0;
     switch (banderas){
         case 0:
             PORTC = display[0];
             PORTDbits.RD0 = 1;
             PORTDbits.RD1 = 0;
             PORTDbits.RD2 = 0;
             banderas = 1;
             return;
         case 1:
             PORTC = display[1];
             PORTDbits.RD0 = 0;
             PORTDbits.RD1 = 1;
             PORTDbits.RD2 = 0;
             banderas = 2;
             return;
         case 2: 
             PORTC = display [2];
             PORTDbits.RD0 = 0;
             PORTDbits.RD1 = 0;
             PORTDbits.RD2 = 1;
             banderas = 0;
             return;
         default: 
             PORTC = 0;
             banderas = 0;
             return;
            
     }
 }
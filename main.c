/*!
\file   seguridad.c
\date   2022-06-28
\author Fulvio Vivas <fyvivas@unicauca.edu.co>
\brief  Security House.

\par Copyright
Information contained herein is proprietary to and constitutes valuable
confidential trade secrets of unicauca, and
is subject to restrictions on use and disclosure.

\par
Copyright (c) unicauca 2022. All rights reserved.

\par
The copyright notices above do not evidence any actual or
intended publication of this material.
******************************************************************************
*/
#include <xc.h>
#include <pic16f887.h>  /*Header file PIC16f887 definitions*/
#include <stdio.h>
#include <string.h>
#include "fuses.h"
#include "lcd.h"
#include "keypad.h"
#include "sensor.h"
//Fila salida del micro entrada del teclado
//Columna entrada del micro salida del teclado

#define LED_GREEN RE0
#define LED_RED RE1 
#define LED_BLUE RE2


void funcion_seg(void);
void funcion_moni(void); 
void funcion_block(void); 
void funcion_alarma(void); 


enum State
{
seguridad, 
monitoreo,
bloqueo,
alarma
};

enum State estado; 

const char password[5] ={'1','2','3','4',0};
char pass_user[5];
unsigned char idx = 0;
unsigned char contador = 0;
unsigned char tiempo = 0;

    
unsigned char var_sensor_ir = 0;
unsigned char var_sensor_hall = 0;
unsigned char var_sensor_metal = 0;

char buffer [17];
void main() {

    //Configura Fosc = 8Mhz interno, Fuente de Fosc del sistema = interno
    OSCCON = 0x71; //Configura oscilador interno (FOSC = 8Mhz)
    TRISE=0;
    LCD_Init(); //Inicializa el LCD

    keypad_init(); //Inicializa el keypad
    TRISC=0x07;
    
    var_sensor_ir = SENSOR_IR;
    var_sensor_hall = SENSOR_HALL;
    var_sensor_metal = SENSOR_METAL;
    
    estado = seguridad; 
    
   
    while(1){
        if(estado == seguridad){
        funcion_seg();
        }
        else if(estado == monitoreo){
            funcion_moni(); 
        
        }
        else if(estado == bloqueo){
            funcion_block(); 
        
        }
        else if(estado == alarma){
            funcion_alarma();
        
        }
    }
}

void funcion_seg(void){
    char key = 0;
 LCD_String_xy(0,0,"password:");
        LCD_Command(0xC0); 
        do{
            key = keypad_getkey();
            if(key != 0){
                LCD_Char('*');/* display pressed key on LCD16x2 */
                //LCD_Char(key);/* display pressed key on LCD16x2 */
                pass_user[idx++] = key;
            }
            __delay_ms(100);
        }while(idx < 4);
        
        if(strncmp(pass_user,password,4)==0){
            LCD_Clear();
            LCD_String_xy(0,0,"bienvenido");
            __delay_ms(2000); 
            LED_GREEN = 0x02;
            
            estado = monitoreo; 
        }
        else if(contador<3){
           
            LCD_Clear();
            LCD_String_xy(0,0,"intente de nuevo");
            LED_BLUE = 0x01;
            contador++;
            }
        else if(contador==3){
        LCD_Clear();
        idx = 0;
        estado = bloqueo; 
        
        }
}

void funcion_moni(void){
if(monitoring_sensor() == 1){
            estado = alarma;
        int n=sprintf(buffer,"sir=%d,sh1=%d,smt=%d",var_sensor_ir,var_sensor_hall,var_sensor_metal);
        LCD_String_xy(0,0,buffer); 
        __delay_ms(2000);
        }

}



void funcion_block(void){
            LCD_Clear();
            LCD_String_xy(0,0,"bloqueado");
            __delay_ms(2000);
            LED_RED = 1; 
        
        __delay_ms(2000);
        estado = seguridad; 
}
 

void funcion_alarma(void){
    
    while(tiempo < 5){
        LED_RED = 1;
        __delay_ms(1000);
        LED_RED = 0;
        tiempo++;
}
}
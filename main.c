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

int Count_Seg=0;
int Count_Mon1=0;
int Count_Mon2=0;
int Count_MonA=0;
int Count_Alrmaseg=0;

void funcion_seg(void);
void funcion_moni(void);
void funcion_block(void);
void funcion_alarma(void);

void adc_int(void);
int adc_leer (unsigned char Selecciona);
int convertir_temp(int temperature);


enum State
{
seguridad,
monitoreo,
monitoreo_ambiental,
bloqueo,
alarma,
ambiental,
alarma_ambiental
};

enum State estado;

const char password[5] ={'1','2','3','4',0};
char pass_user[5];
unsigned char idx = 0;
unsigned char contador = 0;
unsigned char tiempo = 0;
unsigned char flag_timeout=0;

   
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
   
   sensor_anal_init();
   
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
        else if(estado == ambiental){
            func_ambietal();
       
        }
    }
}

void sensor_anal_init(){
    TRISA0 = 1;
    TRISA1 = 1;
    TRISA4 = 1;
    ANSEL = 0x13; //  Configura el Puerto como Entrada Analógica.
   
    adc_int();
   
}
//  FUNCION MONITOREO
void func_ambietal(){
     int value_potenciometro= adc_leer(0);
        __delay_ms(10);
        int value_temperature= adc_leer(1);
        int tempcelsius= convertir_temp(value_temperature);
        __delay_ms(10);
        int value_protocell= adc_leer(4);
         int n=sprintf (buffer,"pot=%d,tem=%d,L=%d",value_potenciometro,value_temperature,value_protocell);
        LCD_String_xy(1,0,buffer);
        __delay_ms(2000);
        LCD_Clear();
}
//  FUNCION SEGURIDAD
void funcion_seg(void){
    char key = 0;
 LCD_String_xy(0,0,"contraseña:");
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
//  FUNCION MONITOREO
void funcion_moni(void){
if(monitoring_sensor() == 1){
            estado = alarma;
        int n=sprintf(buffer,"sir=%d,sh1=%d,smt=%d",var_sensor_ir,var_sensor_hall,var_sensor_metal);
        LCD_String_xy(0,0,buffer);
        __delay_ms(2000);
        }

}


//  FUNCION BLOQUEO
void funcion_block(void){
            LCD_Clear();
            LCD_String_xy(0,0,"bloqueado");
            __delay_ms(2000);
           
           
            LED_RED = 1;
           
            if(flag_timeout){
            funcion_seg(void);
            flag_timeout=0;
            }
            //__delay_ms(2000);
            //estado = seguridad;
}
 
//  FUNCION ALARMA SEGURIDAD
void funcion_alarma(void){
    LCD_String_xy(0,0,"no puedes");
    LCD_Clear();
    for(int tiempo=0; tiempo<5; tiempo++){
        LED_RED = 1;
        Timer_Init(30);
        LED_RED = 0;
    }}
//  FUNCION ALARMA SEGURIDAD
    void funcion_alarma_ambiental(void){
    LCD_String_xy(0,0,"Temperatura alta");
    LCD_Clear();
    for(int tiempo=0; tiempo<4; tiempo++){
        LED_RED = 1;
        Timer_Init(40);
        LED_RED = 0;
}
}
void funcion_moniA(void){
    int value_potenciometro= adc_leer(0);
        __delay_ms(10);
        int value_temperature= adc_leer(1);
        int tempcelsius= convertir_temp(value_temperature);
    if(tempcelsius>36){
        __delay_ms(10);
         Timer_Init(40);
    }
}

 int Timer_Init(int delay) {
       
        if(delay==50 && delay==30){
            estado=monitoreo;
        }
        if(delay==40 && delay==20){
            estado=monitoreo_ambiental;
        }
        if(delay==10){
            estado=seguridad;
        }
        }

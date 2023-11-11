#include    <xc.h>
#include    <pic16f887.h>
#include    <math.h>
#include    "config.h"
#include    "canal.h"


void adc_int (void) {
   
    ADCON1bits.ADFM = 1; //  Justificación Izquierda (modo-8bits).
    ADCON1bits.VCFG0 = 0; //  Selecciona Voltajes de Referencia (5v-0v).
    ADCON1bits.VCFG1 = 0; //  Selecciona Voltajes de Referencia (5v-0v).
    ADCON0bits.ADCS = 0b01; //  Tiempo de Conversión Fosc/8.
   
}

int adc_leer(unsigned char Selecciona) {
    if (Selecciona==0)
            ADCON0bits.CHS = 0b0000; //  Selecciona el Canal Analógico AN0.
    else if (Selecciona==1)
            ADCON0bits.CHS = 0b0001; //  Selecciona el Canal Analógico AN0.
     else  
            ADCON0bits.CHS = 0b0100; //  Selecciona el Canal Analógico AN0.
               
    ADCON0bits.ADON = 1; //  Habilita el Módulo AD.
        __delay_us(30);
        ADCON0bits.GO = 1; //  Inicia la COnversió AD.
        while (ADCON0bits.GO); //  Espera a que termine la conversión AD.
        ADCON0bits.ADON = 0; //  Habilita el Módulo AD.
        return ((ADRESH<< 8) | ADRESL) ;
}

int convertir_temp(int temperature){
    long a=1023 - temperature;
    float tempC=(float)(beta / (log((1025.0 * 10/ a - 10) / 10)+ beta/298.0)-273.0);
    float tempF=(float)(tempC+273.15);
    return (int)tempC;

}

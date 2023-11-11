/*
 * File:   canal.h
 * Author: Logicos
 *
 * Created on 18 de octubre de 2023, 10:28 AM
 */

#ifndef CANAL_H
#define CANAL_H

#ifdef __cplusplus
extern "C" {
#endif

#define beta 4090
#define resistance 10
   
void adc_int(void);
int adc_leer (unsigned char Selecciona);
int convertir_temp(int temperature);

#ifdef __cplusplus
}
#endif

#endif /* CANAL_H */

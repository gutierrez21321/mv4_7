#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipos.h"

void leerArchivo(int RAM[],vecPunterosInt registros[]){ //segmento de codigo
  int i = 0;
  int largoLinea = 33;
  FILE *archivo;
  char linea[largoLinea];
  char *filename = "C:\\Users\\Gutie\\Desktop\\test.txt";

  if((archivo = fopen(filename,"r")) == NULL) return 1;

  while(fgets(linea,largoLinea,archivo) != NULL){
    printf("%s\n",linea);
    RAM[i++] = stringToInt(linea,largoLinea-2);
  }

  registros[IP] = RAM[0]; //instruction pointer arranca en 0
  registros[DS] = RAM[i]; //arranca segmento de datos

  fclose(archivo);
}

void leerInstruccion(int num,int *mnemo,int *opA,int *a,int *opB,int *b){
  *mnemo = num >> 28;          //leo el mnemonico
  *mnemo = *mnemo & 0x0000000f;
  if(*mnemo<=12){ //2 operandos
    *opA = num >> 26;
    *opA = *opA & 0x00000003;
    *a = num>>12;
    if(*opA==1)//registro
      *a = *a & 0x0000003f;
    else
      *a = *a & 0x00000fff;
    *opB = num >> 24;
    *opB = *opB & 0x00000003;
    *b = num & 0x00000fff;
    if(*opB==1)//registro
      *b = *b & 0x0000003f;
    else
      *b = *b & 0x00000fff;
    //printf("2 operandos\n");
    //printf("operando A: %d  operando B: %d\n",*opA,*opB);
    //printf("valor A: %d valor B: %d\n",*a,*b);
  }else if(*mnemo == 0x0000000f){ //1 operando o ninguno
     *mnemo = (*mnemo<<4) & 0x000000f0;
     *mnemo = *mnemo | (0x0000000f & (num >> 24));
     *mnemo = *mnemo & 0x000000ff;
     if((*mnemo & 0x0000000f) <= 12){ //1 operando
      *opA = (num >> 22) & 0x00000003;
      *opB = *b = 0b11;
      if(*opA==1)
        *a = num & 0x0000003f;
      else
        *a = num & 0x0000ffff;
      //printf("1 operando\n");
      //printf("operando A: %d\n",*opA);
      //printf("valor A: %d\n",*a);
     }else if(*mnemo==0x000000ff){ //no operandos
      *opA = *a = *opB = *b = 0b11;
      //printf("0 operandos\n");
     }
  }
  //printf("mnemonico: %d\n",*mnemo);
}


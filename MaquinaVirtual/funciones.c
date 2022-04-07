#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipos.h"

//2 operandos

void setCC(int a){ //setea bit de signo
  int reg = registros[CC];
  if(a == 0){
    reg = reg & 0x000000001;
  }else{
    reg = reg & 0x000000000;
  }
  if(a < 0){
    reg = reg & 0x800000001;
  }else{
    reg = reg & 0x000000001;
  }
  registros[CC] = reg;
}

void modificarRegistro(int a,int valor){ //testear
  int esp,aux,reg;
  esp = a & 0xf;  //accedo registro
  aux = (a >> 4) & 0x3;  //sector de registro
  switch(aux){
      case 0: registros[esp] = registros[esp] & (~E);
              reg = valor;
              break;
      case 1: registros[esp] = registros[esp] & (~L);
              reg = valor & 0x000f;
              break;
      case 2: registros[esp] = registros[esp] & (~H);
              reg = (valor & 0x000f)<<8;
              break;
      case 3: registros[esp] = registros[esp] & (~X);
              reg = valor & 0x00ff;
              break;
    }
  registros[esp] = registros[esp] | reg;
}

int valor(int tipo,int operador){ //podria ser void y modificar operador//devuelve el valor para operar
  int valor,reg,aux;
  if(tipo==1 && operador>=10 && operador<=16){ //registro
    aux = operador & 0xf; //accedo registro
    valor = registros[aux];

    aux = (operador >> 4) & 0x3; //sector de registro

    switch(aux){
      case 0: valor = valor & X;
              break;
      case 1: valor = valor & L;
              break;
      case 2: valor = (valor & H)>>8;
              break;
      case 3: valor = valor & X;
              break;
    }

  }else if(tipo==2) //memoria
    valor = memoria[registros[DS] + operador];
  else //inmediato
    valor = operador;
  return valor;
}

void mov(int opA,int a,int opB,int b){
  int valorB,aux;
  if(opA!=3 && opA!=0 && opB!=3){//si es valido
   valorB = valor(opB,b);
   if(opA==1)//registro
     modificarRegistro(a,valorB);
   else//memoria
     memoria[registros[DS]+a] = valorB;
  }
}

void add(int opA,int a,int opB,int b){
  int suma;
  if(opA!=3 && opA!=0 && opB!=3){//si es valido
    suma = valor(opA,a) + valor(opB,b);
    if(opA==1)  //registro
      modificarRegistro(a,suma);
    else //memoria
      memoria[registros[DS+a]] = suma;
    setCC(suma);
  }
}

void sub(int opA,int a,int opB,int b){
  int resta;
  if(opA!=3 && opA!=0 && opB!=3){//si es valido
    resta = valor(opA,a) - valor(opB,b);
    if(opA==1)  //registro
      modificarRegistro(a,resta);
    else //memoria
      memoria[registros[DS+a]] = resta;
    setCC(resta);
  }
}

void mul(int opA,int a,int opB,int b){
  int multiplicacion;
  if(opA!=3 && opA!=0 && opB!=3){//si es valido
    multiplicacion = valor(opA,a) * valor(opB,b);
    if(opA==1)  //registro
      modificarRegistro(a,multiplicacion);
    else //memoria
      memoria[registros[DS+a]] = multiplicacion;
    setCC(multiplicacion);
  }
}

void division(int opA,int a,int opB,int b){
  if(opA!=3 && opA!=0 && opB!=3){//si es valido
    div_t resultado = div(valor(opA,a),valor(opB,b));
    if(opA==1) //registro
      modificarRegistro(a,resultado.quot); //resultado.quot cociente entero
    else      //memoria
      memoria[registros[DS+a]] = resultado.quot;
    registros[AC] = abs(resultado.rem);    //resto entero positivo
    setCC(resultado.quot);
}

void swap(int opA,int a,int opB,int b){
  int aux,valorB;
  if(opA!=3 && opA!=0 && opB!=3 && opB!=0){//si es valido
    aux = valor(opA,a);
    valorB = valor(opB,b);
    if(opA==1){   //registro
      modificarRegistro(a,valorB);
    }else  //memoria
      memoria[registros[DS+a]] = valorB;

    if(opB==1){   //registro
      modificarRegistro(b,aux);
    }else  //memoria
      memoria[registros[DS+b]] = aux;
  }
}

void cmp(int opA,int a,int opB,int b){ //a puede ser inmediato?? aca supongo que si
  int aux;
  if(opA!=3 && opB!=3)
    aux = valor(opA,a) - valor(opB,b);
  setCC(aux);
}

void and(int opA,int a,int opB,int b){
  int andBit;
  if(opA!=3 && opA!=0 && opB!=3){//si es valido
    andBit = valor(opA,a) & valor(opB,b);
    if(opA==1) //registro
      modificarRegistro(a,andBit);
    else       //memoria
      memoria[registros[DS+a]] = andBit;
  }
  setCC(andBit);
}

void or(int opA,int a,int opB,int b){
  int orBit;
  if(opA!=3 && opA!=0 && opB!=3){//si es valido
    orBit = valor(opA,a) & valor(opB,b);
    if(opA==1) //registro
      modificarRegistro(a,orBit);
    else       //memoria
      memoria[registros[DS+a]] = orBit;
  }
  setCC(orBit);
}

void xor(int opA,int a,int opB,int b){
  int xorBit;
  if(opA!=3 && opA!=0 && opB!=3){//si es valido
    xorBit = valor(opA,a) & valor(opB,b);
    if(opA==1) //registro
      modificarRegistro(a,xorBit);
    else       //memoria
      memoria[registros[DS+a]] = xorBit;
  }
  setCC(xorBit);
}

void shl(int opA,int a,int opB,int b){
  int corrimiento;
  if(opA!=3 && opA!=0 && opB!=3){//si es valido
    corrimiento = valor(opA,a) << valor(opB,b);
    if(opA==1)  //registro
      modificarRegistro(a,corrimiento);
    else        //memoria
      memoria[registros[DS+a]] = corrimiento;
  }
  setCC(corrimiento);
}

void shr(int opA,int a,int opB,int b){
  int corrimiento;
  if(opA!=3 && opA!=0 && opB!=3){//si es valido
    corrimiento = valor(opA,a) >> valor(opB,b);
    if(opA==1) //registro
      modificarRegistro(a,corrimiento);
    else    //memoria
      memoria[registros[DS+a]] = corrimiento;
  }
  setCC(corrimiento);
}

//1 operando

void jmp(int opA,int a){
  if(opA!=3)//si es valido
    registros[IP] = valor(opA,a);
}

void jp(int opA,int a){
  if(opA!=3)//si es valido
    if((registros[CC] & 0x800000000)>>31) == 0x0)
      registros[IP] = valor(opA,a);
}

void jn(int opA,int a){
  if(opA!=3)//si es valido
    if((registros[CC] & 0x800000000)>>31) == 0x1)
      registros[IP] = valor(opA,a);
}

void jz(int opA,int a){
  if(opA!=3)//si es valido
    if(registros[CC] & 0x000000001 == 0x1)
      registros[IP] = valor(opA,a);
}

void jnz(int opA,int a){
  if(opA!=3)//si es valido
    if(registros[CC] & 0x800000000 == 0x0)
      registros[IP] = valor(opA,a);
}

void ldh(int opA,int a){
  int operando,aux;
  operando = valor(opA,a) & 0x00ff;
  aux = registros[CC] & 0x00ff;
  registros[CC] = (operando << 8) | aux;
}

void ldl(int opA,int a){
  int operando,aux;
  operando = valor(opA,a) & 0x00ff;
  aux = registros[CC] & 0xff00;
  registros[CC] = operando | aux;
}

void rnd(int opA,int a,int opB,int b){
  if(opA!=3 && opA!=0 && opB!=3){
    int random = rand() % valor(opB,b);
    if(opA==1)//registro
      modificarRegistro(a,random);
    else
      memoria[registros[DS+a]] = random;
  }
}

void not(int opA,int a){
  int negacion;
  if(opA!=3 && opA!=0){
    negacion = ~valor(opA,a);
    if(opA==1) //registros
      modificarRegistro(a,negacion);
    else //memoria
      memoria[registros[DS+a]] = negacion;
  }
  setCC(negacion);
}

//0 operandos

void stop(){ //detiene programa
  registros[IP] == registros[DS];
}


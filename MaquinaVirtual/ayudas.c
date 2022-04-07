/*
int anyToInt(char *s,char **out){
  char *BASES = {"**$*****@*#*****%"};
  int base = 10;
  char *bp = strchr(BASES,*s);

  //if(bp!=NULL){
    base = bp - BASES;
    s++;
  //}

  return strol(s,out,base); //a cualquier base
}
*/

int stringToInt(char linea[],int ind){
  int i,k,cant,num;
  k = cant = num = 0;
  //printf("%s\n",linea);
  for(i=ind;i>=0;i--){
   k = linea[i] - '0';
   //printf("%d\n",k);
   k = k << cant++;
   num = num | k;
  }
  //printf("%d\n",num);
  return num;
}

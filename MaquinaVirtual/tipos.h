//tamaños
#define MAXREGISTRO 16
#define MAXMEMORIA 4096
//registros
#define DS 0
#define IP 5
#define CC 8
#define AC 9
//mascaras para registros
#define E 0xffff
#define L 0x000f
#define H 0x00f0
#define X 0x00ff

int registros[MAXREGISTRO];
int memoria[MAXMEMORIA];

//headers
void leerInstrucciones(int [],vecPunterosInt);
int stringToInt(char [],int);

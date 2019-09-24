/* 
Authors: Darren, Wei Cheng

File Name: myencodehdr.c 
All Files: myencode.c, myencodehdr.h, myencodesrc.c 
To compile (without gdb), type "gcc myencode.c myencodesrc.c -o myencode -Wall" */

// import the stdio.h library 
#include <stdio.h>

// definitions 
#define A 10
#define B 11
#define C 12
#define D 13
#define E 14
#define F 15
#define red "\x1b[31m"
#define green "\x1b[32m"
#define yellow "\x1b[33m"
#define reset "\x1b[0m"

// function prototypes 
void pUsage(); 
char valFileContent(FILE * , char * , char * , char * , char * , char * , char * ); 
int* storeRetrFileContent(char * , FILE * , char * , int * );
int writeFileContent(char * , FILE * , char * );
void string(char * , char * , char * , char * ); 
void enHexArray(char * , char * );
void endeDigitArray(char * , char * );
void enFinalArray(char * , char * ); 
void deFinalArray(char * , char * );
void deCharArray(char * , char * );




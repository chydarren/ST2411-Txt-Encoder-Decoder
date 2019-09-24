/* 
Authors: Darren, Wei Cheng

File Name: myencodesrc.c 
All Files: myencode.c, myencodehdr.h, myencodesrc.c 
To compile (without gdb), type "gcc myencode.c myencodesrc.c -o myencode -Wall" */

// import the libraries 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "myencodehdr.h" 

/* function: program usage 
Display instructions on how to use the program.
Input Parameter(s):	cmdArgument
Return Value:				- */ 
void pUsage(char *cmdName){
	printf("Usage : ./myencode <-e|-d> <source file>\n");
	printf("	-e Option for encode\n");
	printf("	-d Option for decode\n");
	printf("	<source file> : target file name\n");
}

/* function: string 
Change the output / to be used strings depending on whether if its encoding/decoding. 
Input Parameter(s):	option(-e/-d), string1, string2, string3 
Return Value:				- */ 
void string(char *option, char *string1, char *string2, char *string3){
	
	// assign string depending on option(-e/-d)
	if(strncmp(option, "-e", strlen("-e")) == 0){
		strcpy(string1,"encoding");
		strcpy(string2,"encoded");
	}else{
		strcpy(string1,"decoding");
		strcpy(string2,"decoded");
		strcpy(string3,".xeh"); 
	}
}

/* function: validate file content
Validate if the file is a valid file for performing encoding/decoding.
Input Parameter(s): input file, file name, file extension, validation value, option(-e/-d), ascii array, final array 
Return Value:				val = 'f' IF fail or 0 */ 
char valFileContent(FILE *fp, char *file, char *fileExt, char *val, char *option, char *ascii, char *final){
	
	// declare and initialize string variables 
	int dcounter[1] = {0};
	char string1[9], string2[8], string3[5];
	string(option, string1, string2, string3); //function: string 

	// validate the input file 
	// case a -  file does not exists or file name is invalid 
	if(fp == NULL || ((strncmp(fileExt, string3, strlen(string3)) != 0) && (strncmp(option, "-d", strlen("-d")) == 0))){
		printf("%sInvalid file name '%s' for %s. Please try again.%s\n", red, file, string1, reset);
		return val[0] = 'f';
	}else{
		// for encoding 
		if(strncmp(option, "-e", strlen("-e")) == 0){
			storeRetrFileContent(ascii, fp, option, dcounter); 	// function: store retrieved file content 
			// case b - file content is empty 
			if(ascii[0] == '\0'){
				printf("%sEmpty file content, encoding is aborted.%s\n", red, reset);
				fclose(fp);
				return val[0] = 'f';
			}
		// for decoding 
		}else{
			storeRetrFileContent(final, fp, option, dcounter); 	// function: store retrieved file content 
			// case b - file content is empty 
			if(final[0] == '\0'){
				printf("%sEmpty file content, decoding is aborted.%s\n", red, reset);
				fclose(fp);
				return val[0] = 'f';
			// case c - file content contains non-hex or new line digits 
			}else if(final[strspn(final, "0123456789ABCDEF\n")] != 0){
				printf("%sInvalid file content, decoding is aborted.%s\n", red, reset);
				fclose(fp);
				return val[0] = 'f';
			// case d - hex characters are not even 
			}else if((dcounter[0]%2)!=0){
				printf("%sUneven number of hex characters, decoding is aborted.%s\n", red, reset);
				fclose(fp);
				return val[0] = 'f'; 
			}
		}
	} 
	return 0; 
}

/* function: store retrieved file content 
Store the retrieved file content from the file into the initial array. 
Input Parameter(s):	ascii OR digit array, input file, option, decode counter 
Return Value:				decode counter */ 
int* storeRetrFileContent(char *result, FILE *fp, char *option, int *dcounter){
	
	// declare and initialize variables 
	int i, chrtr;
	
	// store each character into the array 
	for (i=0; i<10000 && (chrtr=getc(fp))!=EOF; ++i){
		result[i] = chrtr;
		
		// if decoding, count characters 
		if(strncmp(option, "-d", strlen("-d")) == 0){
			if(chrtr != '\n' && chrtr != ' ' && chrtr !='	'){
				dcounter[0]++;
			}
		}
  }
	return dcounter; 
}

/* function: write file content
Write the content of the final array into the file. 
Input Parameter(s):	digit OR ascii array, input file, option(-e/-d)
Return Value:				-1 (error type) or 0 */ 
int writeFileContent(char *write, FILE *fp, char *option){
	
	// declare and initialize variables 
	char string1[9], string2[8], string3[5];
	string(option, string1, string2, string3);	// function: string 
	
	// write into output file 
	int result = fputs(write,fp);
	if(result == EOF){
		printf("%sAn error occured when %s the file. Please try again.%s\n", red, string1, reset);
		return (-1);
	}else{
		printf("%sThe file was successfully %s.%s\n",green, string2, reset);
		return 0;
	}
}

/* function: convert characters to hex
For encoding. The ASCII characters will be converted to HEX. 
Input Parameter(s):	hex array, ascii array 
Return Value:				- */ 
void enHexArray(char *hex, char *ascii){
	int i, j; 
	for(i=0,j=0; i<(strlen(ascii)); i++, j+=2){
		// ensure ascii is printable	
		if((ascii[i] < 32 || ascii[i] > 126) && ascii[i] != 0 && ascii[i] != 9 && ascii[i] != 10){
			printf("%sNon-printable detected. Encoding is aborted.%s\n", red, reset);
			exit(0);
		} else {
			sprintf((char*)hex+j,"%02X",ascii[i]);
		}
	}
	
	// debug feature 
	#ifdef DEBUG
	printf("%sContent of file after being converted to hex:\n%s%s\n\n", yellow, hex, reset);
	#endif
} 

/* function: swap hex or swap (swapped) hex 
For encoding/decoding. The HEX characters are swapped to form the digit array or vice versa.
Input Parameter(s):	digit array OR hex array, hex array OR digit array
Return Value:				- */ 
void endeDigitArray(char *result, char *swap){
	
	// declare and initialize variables 
	char tmp;  
	int j; 
	
	// perform the swap 
	for(j=0; j<(strlen(swap)); j+=2){
		tmp = swap[j];
		result[j] = swap[j+1];
		result[j+1] = tmp; 
	}
	
	// debug feature  
	#ifdef DEBUG
	printf("%sContent of file after swap:\n%s%s\n\n", yellow, result, reset);
	#endif
}

/* function: replace 'A0' with new line 
For encoding. The 'A0's will be converted to new lines. 
Input Parameter(s):	digit array 
Return Value:				- */ 
void enFinalArray(char *final, char *digit){
	
	// declare and initialize variables 
	int i, j; 
	
	// perform the replacement 
	for(j=i=0; j<strlen(digit); i++, j+=2){
    if((digit[j] == 'A') && (digit[j+1] == '0')){
			final[i] = '\n';  
    }else{
      final[i] = digit[j];
      final[i+1] = digit[j+1]; 
      i++; 
    }
	}
	
	// debug feature 
	#ifdef DEBUG
	printf("%sContent of file after encryption:\n%s%s\n\n", yellow, final, reset);
	#endif
}

/* function: replace new line with 'A0' 
For decoding. The new lines will be converted to 'A0's. 
Input Parameter(s):	final array 
Return Value:				- */ 
void deFinalArray(char *digit, char *final){
	
	// declare and initialize variables 
	int i, j; 
	
	// perform the replacement 
	for(j=i=0; i<strlen(final); i++, j+=2){
    if(final[i] == '\n'){
			digit[j] = 'A'; 
			digit[j+1] = '0'; 
    }else{
      digit[j] = final[i];
			digit[j+1] = final[i+1];  
			i++; 
    }
	}
	
	// debug feature 
	#ifdef DEBUG
	printf("%sHex values before decryption:\n%s%s\n\n", yellow, digit, reset);
	#endif
}

/* function: convert hex to characters 
For decoding. The HEX will be converted to ASCII characters. 
Input Parameter(s):	ascii array, hex array 
Return Value:				- */ 
void deCharArray(char *ascii, char hex[]){
	
	// declare and initialize variables 
	int i, j, integers[10000];
	char tmp[2];
	
	// convert two hexadecimal digits into one hex value 
	for(i=j=0; i<((strlen(hex))/2); i++, j+=2){
		tmp[0] = hex[j];
		tmp[1] = hex[j+1];
		sscanf(tmp,"%x",&integers[i]);
		// ensure ascii is printable	
		if((integers[i] < 32 || integers[i] > 126) && integers[i] != 0 && integers[i] != 9 && integers[i] != 10){
			printf("%sNon-printable detected. Decoding is aborted.%s\n", red, reset);
			exit(0);
		}
	}
	
	// convert hex value into character
	for(i=0; i<((strlen(hex))/2); i++){
		sprintf((char*)ascii+i,"%c",integers[i]);
	}
	
	// debug feature 
	#ifdef DEBUG
	printf("%sContent of file after decryption:\n%s%s\n\n", yellow, ascii, reset);
	#endif
}



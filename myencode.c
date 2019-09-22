/* 
Authors: Darren, Wei Cheng

File Name: myencode.c 
All Files: myencode.c, myencodehdr.h, myencodesrc.c 
To compile (without gdb), type "gcc myencode.c myencodesrc.c -o myencode -Wall" */

/* references 
Certain algorithms have been adapted/modified from the following sites: 
1. Extract single character - https://stackoverflow.com/questions/7040501/get-a-single-character-from-a-char-in-c
2. Convert characters to hex - https://stackoverflow.com/questions/28596974/convert-to-ascii-to-hex-without-using-print
3. Convert characters to hex - http://www.includehelp.com/c-programs/convert-string-into-hexadecimal.aspx
4. Swap two elements in array - https://stackoverflow.com/questions/30792781/swap-two-elements-of-an-array-function
5. Convert hex to string - https://stackoverflow.com/questions/4770616/converting-hex-into-a-string-using-sprintf
6. Store char as %x hexa - https://stackoverflow.com/questions/27788790/storing-a-hexadecimal-number-in-an-int
7. Store array into string - https://stackoverflow.com/questions/30234363/how-can-i-store-an-int-array-into-string
8. Compare char array with string - https://stackoverflow.com/questions/37859457/c-compare-a-char-array-with-string
9. Fputs array into file - https://stackoverflow.com/questions/14576429/writing-character-array-in-a-file-in-c
10. Compare last few characters of string - https://stackoverflow.com/questions/5297248/how-to-compare-last-n-characters-of-a-string-to-another-string-in-c
11. Remove last few characters of string - https://stackoverflow.com/questions/14544920/how-do-i-remove-last-few-characters-from-a-string-in-c
12. Read lines from text file and store as char array - https://stackoverflow.com/questions/29111336/how-to-read-lines-from-a-text-file-and-store-as-char-array
13. Validating hex values correctly - https://stackoverflow.com/questions/8393389/inputing-validating-hex-values-correctly
14. String tokenization - //www.tutorialspoint.com/c_standard_library/c_function_strtok.htm 
15. Count characters - http://www.opentechguides.com/how-to/article/c/72/c-file-counts.html
16. Color output - https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c */   

/* learning resources
Learning resources and related notes were read from the following sites:
1. Fget function - http://www.dummies.com/programming/c/how-to-use-the-fgets-function-for-text-input-in-c-programming/
2. Fget function - https://stackoverflow.com/questions/6282198/reading-string-from-input-with-space-character
3. Character count - https://www.tutorialspoint.com/c_standard_library/c_function_strlen.htm
4. Open file - https://www.tutorialspoint.com/cprogramming/c_input_output.htm
5. Copy string - https://www.tutorialspoint.com/c_standard_library/c_function_strcpy.htm
6. Pointers - https://www.tutorialspoint.com/cprogramming/c_pointers.htm
7. Return char array - https://stackoverflow.com/questions/31060404/how-can-i-return-a-character-array-from-a-function-in-c
8. Pointers to char array - https://stackoverflow.com/questions/7280379/using-pointer-to-char-array-values-in-that-array-can-be-accessed
9. Pass and return char array - http://www.cplusplus.com/forum/beginner/107573/
10. Sprintf - https://www.tutorialspoint.com/c_standard_library/c_function_sprintf.htm
11. Format specifier (%02X) - https://stackoverflow.com/questions/18438946/format-specifier-02x
17. Format specifiers - https://www.le.ac.uk/users/rjm1/cotter/page_30.htm
18. Format specifiers - http://www.crasseux.com/books/ctutorial/Formatted-output-conversion-specifiers.html
19. Printf function - https://www.tutorialspoint.com/c_standard_library/c_function_printf.htm 
20. https://cboard.cprogramming.com/c-programming/93123-ascii-hex.html */

// import the libraries 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "myencodehdr.h" 

// main function 
int main(int argc, char* argv[]){
	
	// declare and/or initialize global variables 
	char option[2], file[10000], ascii[10000], digit[10000], hex[10000], final[10000], val[1];
	char *fileExt;
	int len;
	FILE *fp = NULL;
  
  // program usage  
	if(argc != 3){
		pUsage();	// function: program usage 
		return 0; 
	}
	
	// store cmd arguments 
	strcpy(option, argv[1]);
	strcpy(file, argv[2]);
	
	// count number of file name characters 
	len = strlen(file);
	
	// retrieve file extension
	fileExt = &file[len-4];
  
	// option conditional statement 
	// encode 
	if(strncmp(option, "-e", strlen("-e")) == 0){
		
    // retrieve file content 
    fp = fopen(file,"r");
		valFileContent(fp, file, fileExt, val, option, ascii, final);
		if(val[0] == 'f') return(-1);
    
    // function calls
		enHexArray(hex, ascii);				// function: convert characters to hex 
		endeDigitArray(digit, hex);   // function: swap hex 
		enFinalArray(final, digit); 	// function: replace 'A0' with new line 
		
    // write file content 
    fp = fopen((strncat(file,".xeh",4)),"w");
		writeFileContent(final, fp, option);	// function: write file content 
    fclose(fp); 
		
	// decode 
	}else if(strncmp(option, "-d", strlen("-d")) == 0){
		
		// retrieve file content 
    fp = fopen(file,"r");
    valFileContent(fp, file, fileExt, val, option, ascii, final); 
		if(val[0] == 'f') return(-1);
		
		// function calls 
		deFinalArray(digit, final); 	// function: replace new line with 'A0' 
		endeDigitArray(hex, digit); 	// function: swap (swapped) hex
		deCharArray(ascii, hex);			// function: convert hex to characters  
		
		// remove file extension 
		file[len-4] = 0;
		
		 // write file content 
    fp = fopen(file,"w");
		writeFileContent(ascii, fp, option);	// function: write file content 
    fclose(fp); 
		
	// else 
	} else{
		pUsage();
	}
	
	// debug feature 
	#ifdef DEBUG
	printf("%s\nThe filename is:\n%s%s\n\n", yellow, file, reset);
	#endif 
	
	return 0;
}
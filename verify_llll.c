#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[]) {
//	FILE *inFile;
	FILE *outFile;
//	char inbuffer[1024];
	char string[1024];
	char outbuffer[1024];
	int ptr;
/*
	inFile = fopen("input.txt","r");
	if(inFile == NULL) {
		printf("InputFile Open Failure");
	}
*/	
	outFile = fopen(argv[1],"r");
	if(!outFile) {
		printf("The file : %s is not found!\n",argv[1]);
		return;
	}
	else {
		while(gets(string) != NULL){
		fgets(outbuffer, 1024, outFile);
//			fread(buffer, 1024, 1, inFile);
			ptr = strcmp(string,outbuffer);
			//printf("%d\n",ptr);
			if((ptr+10) == 0)
				printf("OK!\n");
			else
				printf("Fail\n");
		}
	}
//	fclose(inFile);
	fclose(outFile);
	return 0;
}

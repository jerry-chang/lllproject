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
	int flag1=0;
	int flag2=0;
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
	do{
		
		if(fgets(string,1024,stdin) == NULL) {
			flag1 = 1;
		} else {
		}
		if(fgets(outbuffer,1024,outFile) == NULL) {
			flag2 = 1;
		} else {
		}
		if(flag1 == 1 && flag2 == 1) {
			printf("OK\n");
			break;
		}
		ptr = strcmp(string,outbuffer);
		//printf("%d\n",ptr);
		if((ptr-22) != 0) {
			printf("Fail\n");
			printf("input = %soutput = %s\n",string,outbuffer);
			break;
		}
	} while(1);
	fclose(outFile);
	return 0;
}

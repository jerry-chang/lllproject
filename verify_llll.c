#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[]) {
	FILE *outfile;
	char *string;
	char *outbuffer;
	int compare;
	int input_flag = 0;
	int output_flag = 0;
	size_t in_len = 0;
	size_t out_len = 0;
	ssize_t input_read;
	ssize_t output_read;
	outfile = fopen(argv[1],"r"); // open output file
	if(!outfile) {
		printf("The file : %s is not found!\n",argv[1]);
		return;
	}
	do{
		
		if((input_read = getline(&string,&in_len,stdin)) == -1) {
			input_flag = 1;
		} else {
			char *in_end = strchr(string,'\n'); // find new line of input
			strncpy(in_end,"\0",1);
		}
		if((output_read = getline(&outbuffer,&out_len,outfile)) == -1) {
			output_flag = 1;
		} else {
			char *out_end = strchr(outbuffer,'\n'); // find new line of output
			strncpy(out_end,"\0",1); 
		}
		if(input_flag == 1 && output_flag == 1) { // while finish reading
			printf("OK\n");
			break;
		}
		compare = strcmp(string,outbuffer);
		if(compare != 0) {
			printf("Fail\n");
			printf("input = %s\noutput = %s\n",string,outbuffer);
			break;
		}
	} while(1);
	fclose(outfile);
	return 0;
}

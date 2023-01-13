#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

using namespace std;




int main(int argc, char* argv[])
{
	int m=0;
	char c[256];
	char d[256];
	long pos;
	char* k,*l;
	FILE *out = fopen("/tmp/outdata.txt", "w");
	FILE *in = fopen("/tmp/indata.txt", "r");
	while ((k = fgets(c,150,in)) != NULL){
			fputs(k,out);
	}
	fclose(in);
	fclose(out);
	pid_t id = fork();
	if (id == 0)
	{
		while(1){
			FILE *in = fopen("/tmp/indata.txt", "r");
			FILE *out = fopen("/tmp/outdata.txt", "r");
		    while (((k = fgets(c,256,in)) != NULL)&&(m==0)){
		    	l = fgets(d,256,out);
				if (strcmp(c,d) != 0){
					fclose(out);
					fseek(in,0,SEEK_SET);
					FILE *out = fopen("/tmp/outdata.txt", "w");

					while ((k = fgets(c,256,in)) != NULL){					
						fputs(k,out);
					}
					m+=1;
				}
		  
		    }
		    m=0;
		    fclose(in);
		    fclose(out);
			
		}
			

	} else{
		system("less /tmp/outdata.txt");
		kill(id, SIGKILL);
		
	}

	if (id > 0){
		wait(NULL);
		remove("/tmp/outdata.txt");
	    printf("Удалил outdata.txt\n");
	}


    return 0;
}   

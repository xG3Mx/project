#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>



using namespace std;

int data()
{
		string line;
		ofstream out;
		out.open("/tmp/outdata.txt");
		ifstream in;
		in.open("/tmp/indata.txt");
		if (out.is_open())
    	{
    		printf("Создал outdata.txt\n");
    		sleep(1);
    	}
    	else {
    		printf("ОШИБКА\n");
    		exit(0);
    	}
		
		printf("Открываю outdata.txt\n");
	    sleep(1);


		pid_t p1 = fork();
		if (p1 == 0){
			if (out.is_open())
				    	{
				      	 while (getline(in, line))
				       	 {
				            out << line << endl;
				      	  }
				      	}

			system("less /tmp/outdata.txt");
			printf("Закрыл outdata.txt\n");
			out.close();
			return 0;

	    }


		
		pid_t w_pid1 = waitpid(p1, NULL, 0);
		if (w_pid1 == -1) {exit(0);}
		if (w_pid1 > 0){
			sleep(1);
			remove("/tmp/outdata.txt");
			printf("Удалил outdata.txt\n");
		}
		in.close();
		return 0;		
} 

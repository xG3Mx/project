#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

using namespace std;

//задаёт размерность массива
int number(){
	string line;
	ifstream in;
	in.open("/tmp/indata.txt");
	int n=0;
	while (getline(in, line))
			{
				n++;		
			}		
	return n;

}
//увеличивает размерность массива
void push_back(string *&arr, int &size)
{
	string *newArray = new string[size];
	for (int i = 0; i<size; i++)
	{
		newArray[i]=arr[i];                 
	}

	delete[] arr;

	arr = newArray;
}




int main(int argc, char* argv[])
{
	string line;
	ofstream out;
	out.open("/tmp/outdata.txt");
	ifstream in;
	in.open("/tmp/indata.txt");
	int p=0;
	int n = number();
	string *mass = new string[n];

	pid_t p1 = fork();
	if (p1 == 0)
	{
		
		while(1)
		{						
			in.open("/tmp/indata.txt");
			int i=0;
			while (getline(in, line))
			{

				if (mass[i] != line){
					mass[i] = line;
					out << line << endl;				
				}
				i++;		
			}

			in.close();
			n = number();
			push_back(mass,n);
								       	  
		}


	} else{
		system("less /tmp/outdata.txt");
		sleep(1);
		kill(p1, SIGKILL);
	}
	if (p1 > 0){
		wait(NULL);
		remove("/tmp/outdata.txt");
		printf("Удалил outdata.txt\n");
	}
		

	delete [] mass;
    return 0;
}  

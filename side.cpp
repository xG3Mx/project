#include <iostream>
#include <fstream>
#include "main.cpp"

using namespace std;


int main(int argc, char** argv)
{
	ofstream in("/tmp/indata.txt", ios::app);
	if (in.is_open())
    {
    	for(int i = 1; i < argc; i++){
    		in << argv[i] << endl;
    	}
    	printf("Вписал в indata.txt\n");
        sleep(1);
    }
   else {
            printf("ОШИБКА\n");
            exit(0);
        }

   

   in.close();

   data();
   return 0;
}  

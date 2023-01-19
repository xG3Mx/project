#include <iostream>

#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/inotify.h>

using namespace std;


#define MAX_EVENT_MONITOR 2048
#define NAME_LEN 32
#define MONITOR_EVENT_SIZE (sizeof(struct inotify_event))
#define BUFFER_LEN MAX_EVENT_MONITOR*(MONITOR_EVENT_SIZE+NAME_LEN)

int main(int argc, char* argv[])
{

	FILE *fin;
	FILE *fout;
	const int BLOCK_SIZE = 256;
	char in_buffer[BLOCK_SIZE];

	//заполняет outdata
	if(!(fin = fopen("/tmp/indata.txt", "rb")) == NULL &&
	   !(fout = fopen("/tmp/outdata.txt", "wb")) == NULL) 
	{

		fseeko64(fin, 0, SEEK_END);
		long long file_size = ftello64(fin);
		fseeko64(fin, 0, SEEK_SET);

		for(size_t i = 0; i <= file_size / BLOCK_SIZE; ++i) 
		{
			int readed = fread(in_buffer, sizeof(unsigned char), BLOCK_SIZE, fin);
			fwrite(in_buffer, sizeof(unsigned char), readed, fout);
		}
		fclose(fin);
		fclose(fout);

	}




	pid_t id = fork();
	if (id == 0)
	{
		char *FULLNAME;
		int fd, watch_desc;
		char buffer[BUFFER_LEN];

		fd = inotify_init();

		watch_desc = inotify_add_watch(fd,"/tmp",IN_MODIFY);
		
		int i = 0;
		while(1){
			i = 0;
			int total_read = read(fd,buffer,BUFFER_LEN);
			while(i<total_read)
			{
				struct inotify_event *event=(inotify_event*)&buffer[i];
				if(event->len)
				{
					if(event->mask & IN_MODIFY)
					{
						if(event->mask & IN_ISDIR)
						{						
						}
						else{
							FULLNAME = event->name;
							if(strcmp(FULLNAME,"indata.txt")==0)
							{							
								if(!(fin = fopen("/tmp/indata.txt", "rb")) == NULL &&
								   !(fout = fopen("/tmp/outdata.txt", "wb")) == NULL) 
								{

								    fseeko64(fin, 0, SEEK_END);
								    long long file_size = ftello64(fin);
								    fseeko64(fin, 0, SEEK_SET);

								    for(size_t i = 0; i <= file_size / BLOCK_SIZE; ++i) 
								    {
								        int readed = fread(in_buffer, sizeof(unsigned char), BLOCK_SIZE, fin);
								        fwrite(in_buffer, sizeof(unsigned char), readed, fout);
								    }
								    fclose(fin);
								    fclose(fout);
								}
							}
						}
					}
					i+=MONITOR_EVENT_SIZE+event->len;
				}
			}

		}	


	} else{
		system("less /tmp/outdata.txt");
		kill(id, SIGKILL);
		
	}

	if (id > 0){
		wait(NULL);
		remove("/tmp/outdata.txt");
	}


    return 0;
		
}   

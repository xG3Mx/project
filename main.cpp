#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/inotify.h>

using namespace std;

class File{
private:
    FILE* fin;
    FILE* fout;
    static const int BLOCK_SIZE = 256;
    char in_buffer[BLOCK_SIZE];

public:
    File(const string& filein, const string& fileout){
        fin = fopen(filein.c_str(), "rb");
        fout = fopen(fileout.c_str(), "wb");
        if (fin == nullptr){
            throw runtime_error("cannot open "+ filein);
        }
        if (fout == nullptr){
            throw runtime_error("cannot open "+ fileout);
        }
    }

    void Write(){

        fseeko64(fin, 0, SEEK_END);
        long long file_size = ftello64(fin);
        fseeko64(fin, 0, SEEK_SET);

        for(size_t i = 0; i <= file_size / BLOCK_SIZE; ++i) {
            int readed = fread(in_buffer, sizeof(unsigned char), BLOCK_SIZE, fin);
            fwrite(in_buffer, sizeof(unsigned char), readed, fout);
        }
    }

    ~File(){
        fclose(fin);
        fclose(fout);
    }

};


#define MAX_EVENT_MONITOR 2048
#define NAME_LEN 32
#define MONITOR_EVENT_SIZE (sizeof(struct inotify_event))
#define BUFFER_LEN MAX_EVENT_MONITOR*(MONITOR_EVENT_SIZE+NAME_LEN)


int main(int argc, char* argv[]){ 
    try {
      File f("/tmp/indata.txt","/tmp/outdata.txt");
      f.Write();
    } catch(...){
        cout << "Cannot open file\n";
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
                                try {
                                  File f("/tmp/indata.txt","/tmp/outdata.txt");
                                  f.Write();
                                } catch(...){
                                    cout << "Cannot open file\n";
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

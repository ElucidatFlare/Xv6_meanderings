#include "kernel/types.h"
//#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[]) {

    //    write(1, "Greetings my good Fellow!\n", 26);
    //printf("Greetings my good Fellow!\n\n");

    int pid;
    //int xstatus;
    char *s;
    s = "OwO";
    int st = 1234567;
    st++;
    int nfork;
    nfork = 20/4;
    printf("Initiating Program\n\n");

    printf("[ PID ]:%d Main Process!\n\n", (int) getpid() );
    sleep(10);
    printf("Por cada 1 segundo, un nuevo proceso se creara. \n");
    printf("Este Dormira hasta que todos los procesos se creen\n\n");

    for (int i=0; i<nfork; i++){
        
        pid = fork();
        if(pid < 0){printf("%s: fork failed\n", s); exit(1);
        }

        if(pid == 0){
            for(;;){  
                printf("Ejecutando proceso possum [ PID ]:%d\n\n", (int) getpid() );

            sleep(nfork*10);
            }
            exit(0);
        } else {
            sleep(10);
            //return 0;
        }

    }

    return 0;
}


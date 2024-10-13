#include "kernel/types.h"
//#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

    // writing print statement to print hello world
    //printf("Hello World");
    //    write(1, "Greetings my good Fellow!\n", 26);
    printf("Greetings my good Fellow!\n\n");


    int pid;
    //int xstatus;
    char *s;
    s = "String";
    int st = 1000;

    pid = fork();
    if(pid < 0){
        printf("%s: fork failed\n", s);
        exit(1);
    }
    if(pid == 0){
        printf("%s: Fork Process\n", s);
    } else {
        int rc_wait = wait(&st);

        printf("%s: Original Process\n", s);
        printf("Well... Wait:%d\n", rc_wait);
    }
    return 0;




    return 0;
}
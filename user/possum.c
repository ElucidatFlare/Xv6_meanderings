#include "kernel/types.h"
//#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

    // writing print statement to print hello world
    //printf("Hello World");
    write(1, "Greetings my good Fellow!\n", 100);
    return 0;
}
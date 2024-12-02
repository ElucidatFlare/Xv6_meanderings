#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "kernel/fs.h"
#include "user/user.h"

//#include "user/user.h"

int main(int argc, char *argv[]){

    printf("Hellow world\n" );
    char *buff = (char *)malloc(128 *sizeof(char) );
    strcpy( buff, "Test Buffer OwO");
    printf("Buffer: %p\n", buff );
    send("Test Sending");
//    char *buffer2 = (char *)malloc(128 *sizeof(char) );
    //receive(buff);
//    printf("Buffer: %s\n", buffer2 );
    
    
    return 0;
}
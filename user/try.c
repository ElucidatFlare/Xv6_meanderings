#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "kernel/fs.h"
#include "user/user.h"

//#include "user/user.h"

int main(int argc, char *argv[]){

    printf("Hellow world\n\n" );
    char *buff = (char *)malloc(128 *sizeof(char) );
    strcpy( buff, "Test Buffer OwO");
    printf("Test #0: Buffer Exists\n\n" );

    printf("Buffer: %s\n", buff );
    printf("\n");
    
    printf("Test #1: Recieve Empty\n\n" );

    receive(buff);
    printf("\n");
    printf("Test #2: Sending Multiple\n\n" );
    
    send("Call me maybe");
    send("Phone me Perhaps");
//    char *buffer2 = (char *)malloc(128 *sizeof(char) );
    receive(buff);
//    printf("Buffer: %s\n", buffer2 );
    printf("Get Buff : %s\n", buff );
    receive(buff);
    printf("Get Buff : %s\n", buff );
    
    
    return 0;
}
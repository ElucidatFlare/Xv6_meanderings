#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"


int main(int argc, char* argv[]){
char *addr = sbrk(0);
/*
uint q;
long w;
printf("Addr text  : %p\n", addr);
printf("Size of p %ld Bytes\n", sizeof(addr) );
printf("Size of p %ld Bytes\n", sizeof(*addr) );
printf("Size of p %ld Bytes\n", sizeof(q) );
printf("Size of p %ld Bytes\n", sizeof(w) );
printf("location of text  : %p\n", (void*) main);
static int sttc = 2;
printf("location of data  : %p\n", (void*) &sttc);
static int sttc2 = 2;
printf("location of data  : %p\n", (void*) &sttc2);
printf("location of heap  : %p\n", (void*) malloc( sizeof(q)));
printf("location of heap  : %p\n", (void*) malloc( sizeof(q)));
printf("location of heap  : %p\n", (void*) malloc( sizeof(q)));
uint owo= 1UL;
printf("location of heap  : %d\n", ((owo*15)/(owo*17)) );

int x = 3;
printf("location of stack : %p\n", (void*) &x);
printf("location of stack : %p\n",  &x);
*/
//sbrk(4096);
char *myMem = sbrk(4096);
printf("Addr Mem Addr   : %p\n", addr);
printf("My mem Addr  : %p\n", myMem);
addr[0] = 'J';
printf("Addr Mem Addr   : %s\n", addr);
printf("My mem Addr  : %p\n", addr);

return 0;
}
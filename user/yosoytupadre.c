#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

//
// Tests para una xv6 system calls. 
// basada en usertest
// Reducida a solamente getppid()
//

 
void
testproces(char *s)
{

}


int
main(int argc, char *argv[])
{
 
  if (testproc("w") ) {
    exit(1);
  }
 
  printf("ALL TESTS PASSED\n");
  exit(0);
}

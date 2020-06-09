#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv)
{
    int rtn = 0;
    printf("rtn = %d : system call \n", rtn);
    rtn = system("./prng"); 

    printf("result = %d\n ", rtn);
    printf("result = %d\n ", WEXITSTATUS(rtn));

    return 0;
}

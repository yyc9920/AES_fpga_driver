//Only works on Unix based OS.

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
        int i;
        unsigned char buffer[2];
        int fd = open("/dev/urandom", O_RDONLY);
        read(fd, buffer, 2);
        //buffer now contains the random data
        close(fd);
                printf("%d", buffer[0]);
        printf("\n");
        return 0;
}

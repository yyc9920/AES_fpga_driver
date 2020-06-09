//Only works on Unix based OS.

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
        int i;
        int buffer;
        int fd = open("/dev/urandom", O_RDONLY);
        read(fd, &buffer, 1);
        //buffer now contains the random data
        close(fd);
        //printf("%d", buffer);
        //printf("\n");
        return buffer;
}

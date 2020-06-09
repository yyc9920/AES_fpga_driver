//Only works on Unix based OS.

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
        int buffer;
        int fd = open("/dev/urandom", O_RDONLY);
		if(buffer < 0)
			buffer = -buffer;
        read(fd, &buffer, 1);
        //buffer now contains the random data
        close(fd);
		printf("%d", buffer);
        printf("\n");
        return buffer;
}

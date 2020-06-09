//Only works on Unix based OS.

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
        int buffer;
        int fd = open("/dev/urandom", O_RDONLY);
        read(fd, &buffer, 1);
		if(buffer < 0)
			buffer = -buffer;
        //buffer now contains the random data
        close(fd);
		printf("\n");
		printf("%d", buffer);
		printf("\n");
        return buffer;
}

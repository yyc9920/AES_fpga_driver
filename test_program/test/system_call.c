#include <stdio.h>
#include <stdlib.h>

int main(){
        int buffer;
		buffer = system("prng");
		printf("%d", buffer);
        printf("\n");
        return 0;
}

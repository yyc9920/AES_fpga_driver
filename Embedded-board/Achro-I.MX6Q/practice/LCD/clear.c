/* FILENAME : fbmranrect.c */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <linux/fb.h>
void swap(int *swapa, int *swapb);
short random_pixel(void);

int main(int argc, char** argv) {
	int check, frame_fd;
	short pixel;
	int offset, posx1, posy1, posx2, posy2;
	int repx, repy, count = 1000;
	short* pfbdata;
	struct fb_var_screeninfo fvs;
	int x=0, y=0,r = 500;
	if((frame_fd = open("/dev/fb0",O_RDWR))<0) {
		perror("Frame Buffer Open Error!");
		exit(1);
	}

	if((check=ioctl(frame_fd,FBIOGET_VSCREENINFO,&fvs))<0) {
		perror("Get Information Error - VSCREENINFO!");
		exit(1);
	}

	pfbdata = (short *) mmap(0, fvs.xres*fvs.yres*(sizeof(pixel)), PROT_READ| \
		PROT_WRITE, MAP_SHARED, frame_fd, 0); 
	if((unsigned)pfbdata == (unsigned)-1) {
		perror("Error Mapping!\n");
	}
	
	for(repy = 0; repy < 600; repy++)
	{
		offset = repy * fvs.xres;
		for(repx = 0; repx < 1024; repx++)
		{
			x = repx - 511;
			y = repy - 599;
			
			*(pfbdata+offset+repx) = 0;
		
		}
	}
	munmap(pfbdata,fvs.xres*fvs.yres*(sizeof(pixel))); // 맵핑된 메모리 해제
	close(frame_fd);
	return 0;
}
short random_pixel(void) {
	return (int)(65536.0*rand()/(RAND_MAX+1.0));
}
void swap(int *swapa, int *swapb) {
	int temp;
	if(*swapa > *swapb) {
		temp = *swapb;
		*swapb = *swapa;
		*swapa = temp;
	}
}

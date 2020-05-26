/* FILENAME : fbmranrect.c */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <linux/fb.h>
#include <math.h>
#define pi 3.14159265358979323846
int warn[45] = {0,0,0,0,0,0,90000,0,0,0,0,0,0,22500,0};
void swap(int *swapa, int *swapb);
short random_pixel(void);

int main(int argc, char** argv) {
	int check, frame_fd;
	short pixel;
	int offset, posx1, posy1, posx2, posy2;
	int repx, repy,count = 0 ;
	short* pfbdata;
	struct fb_var_screeninfo fvs;
	int x=0, y=0,r = 500,i;
	int slide = 0;
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
	//left circle
	for(i = 0; i < 45;i++)
	{
		for(repy = 599; repy >= 100; repy--)
		//for(repy = 599; repy >= 599-(r*sin((count + 2)*(pi/180))); repy--)
		{
			offset = repy * fvs.xres;
			for(repx = 512; repx < 1012; repx++)
			//for(repx = 512; repx < 512 + ((r*list_x[i])/10000); repx++)
			{
				x = repx - 512;
				y = 599 - repy;
				
				if((x*x) + (y*y) == warn[i])
						*(pfbdata+offset+repx) = 0;
				else if((x)*(x) + (y)*(y) <= r*r)
				{	
					*(pfbdata+offset+repx) = 992;
				//}//else
					//*(pfbdata+offset+repx) = 0;
				}	
			}
		}
		//usleep(1);
		count+=2;
	}
	//left circle
	for(repy = 0; repy < 600; repy++)
	{
		offset = repy * fvs.xres;
		for(repx = 0; repx < 512; repx++)
		{
			x = repx - 511;
			y = repy - 599;
			
			if((x)*(x) + (y)*(y) <= r*r)
				*(pfbdata+offset+repx) = 992;
			else
				*(pfbdata+offset+repx) = 0;
				
			usleep(1);
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

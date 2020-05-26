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

int tan_list[16] = {0, 524, 1051, 1583, 2125, 2679, 3249, 3838, 4452, 5095, 5773, 6494, 7265, 8097, 9004, 10000};
int main() {
	int check, frame_fd;
	short pixel;
	int offset, posx1, posy1, posx2, posy2;
	int repx, repy,count = 0 ;
	short* pfbdata;
	struct fb_var_screeninfo fvs;
	int x=0, y=0;
	int i;
	int slide = 0, temp;
	int r[61];
	r[0] = 400;
	for(i = 1; i < 61;i++)
	{	
		r[i] = 400;
	}
	if((frame_fd = open("/dev/fb1",O_RDWR))<0) {
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

	//edge circle
	

	for(repy = 599; repy >= 0; repy--)
	{
		offset = repy * fvs.xres;
		for(repx = 0; repx < 1024; repx++)
			{
				x = repx - 512;
				y = 599 - repy;
				if((x)*(x) + (y)*(y) <= 250000 + 10000 && (x*x)+(y*y) > 250000){
					*(pfbdata+offset+repx) = 0xffff;
				}
			}
	}
	//right circle
	for(i = 0; i <= 15; i++){
		for(repy = 599; repy >= 100 ; repy--)
		{
			offset = repy * fvs.xres;
			for(repx = 513; repx < 1013; repx++)
			{
				x = repx - 512;
				y = 599 - repy;
				temp = (int) (y*10000)/x;
				if(i == 0){
					if(temp <= tan_list[i]){
						if((x)*(x) + (y)*(y) < r[i]*r[i])
							*(pfbdata+offset+repx) = 992;
						else if((x)*(x) + (y)*(y) < 500*500)
							*(pfbdata+offset+repx) = 0;
					}
				}
				else{
					if(temp <= tan_list[i] && temp >= tan_list[i-1]){
						if((x)*(x) + (y)*(y) < r[i]*r[i])
							*(pfbdata+offset+repx) = 992;
						else if((x)*(x) + (y)*(y) < 500*500)
							*(pfbdata+offset+repx) = 0;
					}
				}
			}
		}
		printf("%d\n",i);
	}

	for(i = 15; i > 0; i--){
		for(repy = 598; repy >= 100 ; repy--)
		{
			offset = repy * fvs.xres;
			for(repx = 513; repx < 1013; repx++)
			{
				x = repx - 512;
				y = 599 - repy;
				temp = (int) (x*10000)/y;
				if(i == 0){
					if(temp <= tan_list[i]){
						if((x)*(x) + (y)*(y) < r[15 + (16 - i)]*r[15 + (16 - i)])
							*(pfbdata+offset+repx) = 992;
						else if((x)*(x) + (y)*(y) < 500*500)
							*(pfbdata+offset+repx) = 0;
					}
				}
				else{
					if(temp <= tan_list[i] && temp >= tan_list[i-1]){
						if((x)*(x) + (y)*(y) < r[15 + (16 - i)]*r[15 + (16 - i)])
							*(pfbdata+offset+repx) = 992;
						else if((x)*(x) + (y)*(y) < 500*500)
							*(pfbdata+offset+repx) = 0;
					}
				}

			}
		}
		printf("%d\n",i);
	}
	//left circle
	for(repy = 599 ; repy > 100; repy--)
	{
		if(repy > 599 - r[30]){
			offset = repy * fvs.xres;
			*(pfbdata+offset+512) = 992;
		} else {
			offset = repy * fvs.xres;
			*(pfbdata+offset+512) = 0;
		}
	}
	for(i = 1; i <= 15; i++){
		for(repy = 598; repy >= 100 ; repy--)
		{
			offset = repy * fvs.xres;
			for(repx = 0; repx < 512; repx++)
			{
				x = repx - 512;
				x = -1 * x;
				y = 599 - repy;
				temp = (int) (x*10000)/y;
				if(i == 0){
					if(temp <= tan_list[i]){
						if((x)*(x) + (y)*(y) < r[30 + i]*r[30 + i])
							*(pfbdata+offset+repx) = 992;
						else if((x)*(x) + (y)*(y) < 500*500)
							*(pfbdata+offset+repx) = 0;
					}
				}
				else{
					if(temp <= tan_list[i] && temp >= tan_list[i-1]){
						if((x)*(x) + (y)*(y) < r[30 + i]*r[30 + i])
							*(pfbdata+offset+repx) = 992;
						else if((x)*(x) + (y)*(y) < 500*500)
							*(pfbdata+offset+repx) = 0;
					}
				}
			}
		}
		printf("%d\n",i);
	}
	for(i = 15; i > 0; i--){
		for(repy = 598; repy >= 100 ; repy--)
		{
			offset = repy * fvs.xres;
			for(repx = 0; repx < 512; repx++)
			{
				x = repx - 512;
				x = -1*x;
				y = 599 - repy;
				temp = (int) (y*10000)/x;				
				if(i == 0){
					if(temp <= tan_list[i]){
						if((x)*(x) + (y)*(y) < r[46 + (15 - i)]*r[46 + (15 - i)])
							*(pfbdata+offset+repx) = 992;
						else if((x)*(x) + (y)*(y) < 500*500)
							*(pfbdata+offset+repx) = 0;
					}
				}
				else{
					if(temp <= tan_list[i] && temp >= tan_list[i-1]){
						if((x)*(x) + (y)*(y) < r[46 + (15 - i)]*r[46 + (15 - i)])
							*(pfbdata+offset+repx) = 992;
						else if((x)*(x) + (y)*(y) < 500*500)
							*(pfbdata+offset+repx) = 0;
					}
				}//else
			}
		}
		printf("%d\n",i);
	}

	//left circle 
	for(i = 60 ; i >=0; i--)
		r[i] = 150;	
	for(i = 0; i <= 15; i++){
		for(repy = 598; repy >= 100 ; repy--)
		{
			offset = repy * fvs.xres;
			for(repx = 0; repx < 512; repx++)
			{
				x = repx - 512;
				x = -1*x;
				y = 599 - repy;
				temp = (int) (y*10000)/x;
				if(i == 0){
					if(temp <= tan_list[i]){
						if((x)*(x) + (y)*(y) < r[45 + (15 - i)]*r[45 + (15 - i)])
							*(pfbdata+offset+repx) = 992;
						else if((x)*(x) + (y)*(y) < 500*500)
							*(pfbdata+offset+repx) = 0;
						}
				}
				else{
					if(temp <= tan_list[i] && temp >= tan_list[i-1]){
						if((x)*(x) + (y)*(y) < r[45 + (15 - i)]*r[45 + (15 - i)])
							*(pfbdata+offset+repx) = 992;
						else if((x)*(x) + (y)*(y) < 500*500)
							*(pfbdata+offset+repx) = 0;
					}
				}
			}
		}
		printf("%d\n",i);
	}

	for(i = 15; i >= 1; i--){
		for(repy = 598; repy >= 100 ; repy--)
		{
			offset = repy * fvs.xres;
			for(repx = 0; repx < 512; repx++)
			{
				x = repx - 512;
				x = -1*x;
				y = 599 - repy;
				temp = (int) (x*10000)/y;
				if(i == 0){
					if(temp <= tan_list[i]){
						if((x)*(x) + (y)*(y) < r[45 - (15 - i)]*r[45 + (15 - i)])
							*(pfbdata+offset+repx) = 992;
						else if((x)*(x) + (y)*(y) < 500*500)
							*(pfbdata+offset+repx) = 0;
						}
				}
				else{
					if(temp <= tan_list[i] && temp >= tan_list[i-1]){
						if((x)*(x) + (y)*(y) < r[45 - (15 - i)]*r[45 - (15 - i)])
							*(pfbdata+offset+repx) = 992;
						else if((x)*(x) + (y)*(y) < 500*500)
							*(pfbdata+offset+repx) = 0;
					}
				}
			}
		}
		printf("%d\n",i);
	}
	//right circle
	for(repy = 599 ; repy > 100; repy--)
	{
		if(repy > 599 - r[30]){
			offset = repy * fvs.xres;
			*(pfbdata+offset+512) = 992;
		} else {
			offset = repy * fvs.xres;
			*(pfbdata+offset+512) = 0;
		}
	}
	for(i = 1; i<=  15; i++){
		for(repy = 598; repy >= 100 ; repy--)
		{
			offset = repy * fvs.xres;
			for(repx = 513; repx < 1013; repx++)
			{
				x = repx - 512;
				y = 599 - repy;
				temp = (int) (x*10000)/y;
				if(i == 0){
					if(temp <= tan_list[i]){
						if((x)*(x) + (y)*(y) < r[31 -  i]*r[31 - i])
							*(pfbdata+offset+repx) = 992;
						else if((x)*(x) + (y)*(y) < 500*500)
							*(pfbdata+offset+repx) = 0;
					}
				}
				else{
					if(temp <= tan_list[i] && temp >= tan_list[i-1]){
						if((x)*(x) + (y)*(y) < r[31 -  i]*r[31 - i])
							*(pfbdata+offset+repx) = 992;
						else if((x)*(x) + (y)*(y) < 500*500)
							*(pfbdata+offset+repx) = 0;
					}
				}

			}
		}
		printf("%d\n",i);
	}

	for(i = 15; i >= 0; i--){
		for(repy = 599; repy >= 100 ; repy--)
		{
			offset = repy * fvs.xres;
			for(repx = 513; repx < 1013; repx++)
			{
				x = repx - 512;
				y = 599 - repy;
				temp = (int) (y*10000)/x;
				if(i == 0){
					if(temp <= tan_list[i]){
						if((x)*(x) + (y)*(y) < r[i]*r[i])
							*(pfbdata+offset+repx) = 992;
						else if((x)*(x) + (y)*(y) < 500*500)
							*(pfbdata+offset+repx) = 0;
					}
				}
				else{
					if(temp <= tan_list[i] && temp >= tan_list[i-1]){
						if((x)*(x) + (y)*(y) < r[i]*r[i])
							*(pfbdata+offset+repx) = 992;
						else if((x)*(x) + (y)*(y) < 500*500)
							*(pfbdata+offset+repx) = 0;
					}
				}
			}
		}
		printf("%d\n",i);
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

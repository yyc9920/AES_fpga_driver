/*
 * display
 * The objective of this C program is to merely display text, lines and rectangles
 * on a screen using basic Linx Framebuffer routines. 
 * This is "text the hard way", where the characters are defined initially as arrays
 * of 1 and 0. While this is tedious, it allows much freedom for quick and dirty display
 * on the screen. The code can be pared back, removing characters that are never used by the program.
 * With arrays of 1 or 0 they can be easily modififed, and the color can be decided at print time. 
 * Note there is a "pre color " routine and a memory move version to display characters.  
 * These special routines might have some side effects. 
 * To show how to do smooth animation with number counting or display, some animation via "page flipping" 
 * is done, as derived from the original demo that this demo is built upon. 
 * There are wayt to put graphics to arrays, usually by reading monochrome bitmaps. So that makes it 
 * possible to build custom characters or fonts. The fonts in this demo are actually influenced from 
 * the sentry gun displays from Aliens https://www.youtube.com/watch?v=HQDy-5IQvuU
 *
 * compile with 'gcc -O2 -o display display.c'
 * run with './display'
 *
 * Additonal notes: 
 * - Take note of the "pixel depth" value. In this demo it's set at 8, meaning that 
 * it uses a color "range" defined by 0 to 15 in value. Be watchful of the system you are 
 * using and what it's capable of. If you go to a greater depth, you can use a larger variable
 * to carry a larger color value. It will slow the program down. This program was made with 
 * simpler systems in mind that might use a simpler and less capable TFT LCD or something 
 * of that nature. 
 * - It's important that this program restore the screen settings on exit. 
 * - This program demonstrates some minor animation effects merely to show that it's 
 * a possibility. But there is no "edge" checking routine for the screen buffer array. 
 * So as usual with programs like this, you can cause a segfault if you go out of bounds. 
 * - While a "space" character in both array sizes exists, the system on which this was 
 * tested was doing odd things with the array of 0s, and strange artifacts were appearing
 * in displayed text that had spaces. So the drawing routines for strings instead just 
 * move over instead of drawing a space. Performance on other sytems may vary. 
 * - This program was tested on a Raspberry Pi using a small HDMI screen. If using an extra
 * LCD screen such as the sort connected via SPI the buffer number may differ, such as 
 * "fb1" instead of "fb0".  
 * - The characters are hard-coded in a block and the arrays of pointers that point to them
 * put the pointer to the character in the respective ASCII value. This means they are 128
 * elements, but not all are occupied. This allows atoi conversions to be quicker but 
 * if less characters are needed other more efficient ways are possible. 
 * 
 * This demo is based on riginal work by J-P Rosti (a.k.a -rst- and 'Raspberry Compote')
 * http://raspberrycompote.blogspot.com/2015/01/low-level-graphics-on-raspberry-pi-part.html
 * http://raspberrycompote.blogspot.com/2015/01/low-level-graphics-on-raspberry-pi-part_27.html
 *
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/input.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <linux/ioctl.h>
#include <signal.h>
#include "display.h"
// These are the sizes of the individual character arrays
#define CHAR_ARR__29x24 696
#define CHAR_ARR__10x14 168
#define MAINSTEP 0
#define SELECTSTEP 1
#define CHECKBALANCESTEP 2
#define CHECKHISTORYSTEP 3
#define CHECKSENDSTEP 4


int step = 0;

unsigned char *ascii_characters_BIG[128];	// Store the ASCII character set, but can have some elements blank
unsigned char *ascii_characters_SMALL[128]; // Store the ASCII character set, but can have some eleunsigned char *c2[128];
unsigned char *numbers_BIG[10];				// For quicker number display routines, these arrays of pointers to the numbers
unsigned char *numbers_small[10];
// 'global' variables to store screen info and take the frame buffer.
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
// helper function to 'plot' a pixel in given color
// This is the heart of most of the drawing routines except where memory copy or move is used.
// application entry point

void getTouch(int *x, int *y, int start_x, int start_y, int end_x, int end_y)
{
	int fd, ret,i;
	int cnt = 0;
	pid_t pid;
	const char* evdev_path = "/dev/input/by-path/platform-imx-i2c.2-event";
	// TODO : Find this file in host PC.
	struct input_event iev[3];
	fd = open(evdev_path, O_RDONLY);
	if(fd < 0) {
		perror("error: could not open evdev");
		return;
	}

	pid = fork();
	if(pid>0){
		printf("parents is alive\n");
		printf("parent process is %d\n", getpid());
		sleep(50);
	}
	else if(pid == 0){//0 = children pross
		printf("child process\n");
		printf("child process is %d\n", getpid());

		while(1)
		{
			ret = read(fd, iev, sizeof(struct input_event)*3);
			if(ret < 0) {
				printf("error: could not read input event\n");
				perror("error: could not read input event");
				break;
			}

			if(iev[0].type == 1 && iev[1].type == 3 && iev[2].type == 3)
			{
				printf("touch!!!!\n");
				printf("x = %d, y = %d \n",iev[1].value,iev[2].value);	
				*x = iev[1].value;
				*y = iev[2].value;
				if(*x>=start_x && *x<=end_x && *y>=start_y && *y<=end_y)
					break;
			}
			else if(iev[0].type == 0 && iev[1].type == 1 && iev[2].type == 0)
			{
				printf("hands off!!!\n");
			}
			else if(iev[0].type == 0 && iev[1].type == 3 && iev[2].type == 0 ||\
					iev[0].type == 3 && iev[1].type == 3 && iev[2].type == 0)
			{
				printf("touching...\n");
			}
		}
	}
	else
	{
		printf("can't fork, erro\n");
		exit(0);
	}
}


int main(int argc, char *argv[])
{
	int x, y;
	struct fb_var_screeninfo orig_vinfo;
	long int screensize = 0;
	// The actual glyphs here. Discard that which is not used to save memory
	{
		// Elements actually correspond to the ASCII chart
		ascii_characters_BIG[32] = SPACE__29x24;
		ascii_characters_BIG[48] = AR0__29x24;
		ascii_characters_BIG[49] = AR1__29x24;
		ascii_characters_BIG[50] = AR2__29x24;
		ascii_characters_BIG[51] = AR3__29x24;
		ascii_characters_BIG[52] = AR4__29x24;
		ascii_characters_BIG[53] = AR5__29x24;
		ascii_characters_BIG[54] = AR6__29x24;
		ascii_characters_BIG[55] = AR7__29x24;
		ascii_characters_BIG[56] = AR8__29x24;
		ascii_characters_BIG[57] = AR9__29x24;
		ascii_characters_BIG[58] = COLON__29x24;
		ascii_characters_BIG[65] = A__29x24;
		ascii_characters_BIG[66] = B__29x24;
		ascii_characters_BIG[67] = C__29x24;
		ascii_characters_BIG[68] = D__29x24;
		ascii_characters_BIG[69] = E__29x24;
		ascii_characters_BIG[70] = F__29x24;
		ascii_characters_BIG[71] = G__29x24;
		ascii_characters_BIG[72] = H__29x24;
		ascii_characters_BIG[73] = I__29x24;
		ascii_characters_BIG[74] = J__29x24;
		ascii_characters_BIG[75] = K__29x24;
		ascii_characters_BIG[76] = L__29x24;
		ascii_characters_BIG[77] = M__29x24;
		ascii_characters_BIG[78] = N__29x24;
		ascii_characters_BIG[79] = O__29x24;
		ascii_characters_BIG[80] = P__29x24;
		ascii_characters_BIG[81] = Q__29x24;
		ascii_characters_BIG[82] = R__29x24;
		ascii_characters_BIG[83] = S__29x24;
		ascii_characters_BIG[84] = T__29x24;
		ascii_characters_BIG[85] = U__29x24;
		ascii_characters_BIG[86] = V__29x24;
		ascii_characters_BIG[87] = W__29x24;
		ascii_characters_BIG[88] = X__29x24;
		ascii_characters_BIG[89] = Y__29x24;
		ascii_characters_BIG[90] = Z__29x24;

		ascii_characters_SMALL[32] = SPACE__10x14;
		ascii_characters_SMALL[48] = AR0__10x14;
		ascii_characters_SMALL[49] = AR1__10x14;
		ascii_characters_SMALL[50] = AR2__10x14;
		ascii_characters_SMALL[51] = AR3__10x14;
		ascii_characters_SMALL[52] = AR4__10x14;
		ascii_characters_SMALL[53] = AR5__10x14;
		ascii_characters_SMALL[54] = AR6__10x14;
		ascii_characters_SMALL[55] = AR7__10x14;
		ascii_characters_SMALL[56] = AR8__10x14;
		ascii_characters_SMALL[57] = AR9__10x14;
		ascii_characters_SMALL[58] = COLON__10x14;
		ascii_characters_SMALL[65] = A__10x14;
		ascii_characters_SMALL[66] = B__10x14;
		ascii_characters_SMALL[67] = C__10x14;
		ascii_characters_SMALL[68] = D__10x14;
		ascii_characters_SMALL[69] = E__10x14;
		ascii_characters_SMALL[70] = F__10x14;
		ascii_characters_SMALL[71] = G__10x14;
		ascii_characters_SMALL[72] = H__10x14;
		ascii_characters_SMALL[73] = I__10x14;
		ascii_characters_SMALL[74] = J__10x14;
		ascii_characters_SMALL[75] = K__10x14;
		ascii_characters_SMALL[76] = L__10x14;
		ascii_characters_SMALL[77] = M__10x14;
		ascii_characters_SMALL[78] = N__10x14;
		ascii_characters_SMALL[79] = O__10x14;
		ascii_characters_SMALL[80] = P__10x14;
		ascii_characters_SMALL[81] = Q__10x14;
		ascii_characters_SMALL[82] = R__10x14;
		ascii_characters_SMALL[83] = S__10x14;
		ascii_characters_SMALL[84] = T__10x14;
		ascii_characters_SMALL[85] = U__10x14;
		ascii_characters_SMALL[86] = V__10x14;
		ascii_characters_SMALL[87] = W__10x14;
		ascii_characters_SMALL[88] = X__10x14;
		ascii_characters_SMALL[89] = Y__10x14;
		ascii_characters_SMALL[90] = Z__10x14;

		numbers_small[0] = AR0__10x14; // For number displays
		numbers_small[1] = AR1__10x14;
		numbers_small[2] = AR2__10x14;
		numbers_small[3] = AR3__10x14;
		numbers_small[4] = AR4__10x14;
		numbers_small[5] = AR5__10x14;
		numbers_small[6] = AR6__10x14;
		numbers_small[7] = AR7__10x14;
		numbers_small[8] = AR8__10x14;
		numbers_small[9] = AR9__10x14;

		numbers_BIG[0] = AR0__29x24;
		numbers_BIG[1] = AR1__29x24;
		numbers_BIG[2] = AR2__29x24;
		numbers_BIG[3] = AR3__29x24;
		numbers_BIG[4] = AR4__29x24;
		numbers_BIG[5] = AR5__29x24;
		numbers_BIG[6] = AR6__29x24;
		numbers_BIG[7] = AR7__29x24;
		numbers_BIG[8] = AR8__29x24;
		numbers_BIG[9] = AR9__29x24;
	}

	// Open the framebuffer file for reading and writing
	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd == -1)
	{
		printf("Error: cannot open framebuffer device.\n");
		return (1);
	}

	// Get variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo))
	{
		printf("Error reading variable information.\n");
	}

	// Store for reset (copy vinfo to vinfo_orig)
	memcpy(&orig_vinfo, &vinfo, sizeof(struct fb_var_screeninfo));
	// Change variable info
	vinfo.bits_per_pixel = 32;
	// Can change res here, or leave what was found originally
	vinfo.xres = 1024;
	vinfo.yres = 600;
	vinfo.xres_virtual = vinfo.xres;
	vinfo.yres_virtual = vinfo.yres * 2;
	if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo))
	{
		printf("Error setting variable information.\n");
	}

	// hide cursor
	char *kbfds = "/dev/tty";
	int kbfd = open(kbfds, O_WRONLY);
	if (kbfd >= 0)
	{
		ioctl(kbfd, KDSETMODE, KD_GRAPHICS);
	}
	else
	{
		printf("Could not open %s.\n", kbfds);
	}

	// Get fixed screen information
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo))
	{
		printf("Error reading fixed information.\n");
	}

	page_size = finfo.line_length * vinfo.yres;

	while(1) {
		if(step == 0){
			screensize = finfo.smem_len;
			fbp = (char *)mmap(0,
					screensize,
					PROT_READ | PROT_WRITE,
					MAP_SHARED,
					fbfd,
					0);
			if ((int)fbp == -1)
			{
				printf("Failed to mmap\n");
			}
			else
			{
				// draw...
				//-----------------------------------------------------------graphics loop here

				//	draw();

				int fps = 60;
				int secs = 10;
				int xloc = 1;
				int yloc = 1;
				for (int i = 1; i < 3; i++)
				{
					// change page to draw to (between 0 and 1)
					cur_page = (cur_page + 1) % 2;
					// clear the previous image (= fill entire screen)
					clear_screen(0);
					drawline(100, 400, xloc + 222, 555);
					draw_string(650, 20, (char *)"AES FINAL PROJECT", 17, 65535, 0, 10, 2);
					draw_string(850, 80, (char *)"YECHAN YUN", 10, 6, 0, 10, 1);
					draw_string(850, 100, (char *)"KIDEOK KIM", 10, 6, 0, 10, 1);
					draw_string(805, 140, (char *)"B A S S", 7, 6, 0, 10, 2);
					draw_string(880, 200, (char *)"START", 5, 6, 9, 10, 2);

					// switch page
					vinfo.yoffset = cur_page * vinfo.yres;
					ioctl(fbfd, FBIOPAN_DISPLAY, &vinfo);
					// the call to waitforvsync should use a pointer to a variable
					// https://www.raspberrypi.org/forums/viewtopic.php?f=67&t=19073&p=887711#p885821
					// so should be in fact like this:
					__u32 dummy = 0;
					ioctl(fbfd, FBIO_WAITFORVSYNC, &dummy);
					// also should of course check the return values of the ioctl calls...
					if (yloc >= vinfo.yres / 2)
						yloc = 1;
					if (xloc >= 100)
						yloc = 1;
					yloc++;
					xloc++;
				}
				//-----------------------------------------------------------graphics loop here
			}

			// unmap fb file from memory
			munmap(fbp, screensize);
			// reset cursor
			if (kbfd >= 0)
			{
				ioctl(kbfd, KDSETMODE, KD_TEXT);
				// close kb file
				close(kbfd);
			}
			// reset the display mode
			if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &orig_vinfo))
			{
				printf("Error re-setting variable information.\n");
			}

			//step forwarwd to  step 1
			getTouch(&x, &y, 430, 400, 520, 460);
			printf("After Touch\nx = %d, y = %d", x, y);
			if(x>=430 && x<=520 && y>=400 && y<=460){
				step = 1;
			}
		}

		/*--------------------------Get Touch And Redraw Display Here-------------------------*/

		// draw...
		//-----------------------------------------------------------graphics loop here

		//	draw();
		if(step == 1) {
			screensize = finfo.smem_len;
			fbp = (char *)mmap(0,
					screensize,
					PROT_READ | PROT_WRITE,
					MAP_SHARED,
					fbfd,
					0);
			if ((int)fbp == -1)
			{
				printf("Failed to mmap\n");
			}
			else
			{
				int fps = 60;
				int secs = 10;
				int xloc = 1;
				int yloc = 1;
				for (int i = 1; i < 3; i++)
				{
					// change page to draw to (between 0 and 1)
					cur_page = (cur_page + 1) % 2;
					clear_screen(0);
					// clear the previous image (= fill entire screen)
					drawline(100, 400, xloc + 222, 555);
					draw_string(880, 40, (char *)"CHECK BALANCE", 13, 6, 9, 10, 2);
					draw_string(880, 120, (char *)"CHECK TRANSACTION HISTORY", 25, 6, 9, 10, 2);
					draw_string(880, 200, (char *)"SEND", 4, 6, 9, 10, 2);
					draw_string(400, 50, (char *)"B", 1, 6, 9, 10, 2);
					draw_string(400, 100, (char *)"A", 1, 6, 9, 10, 2);
					draw_string(400, 150, (char *)"S", 1, 6, 9, 10, 2);
					draw_string(400, 200, (char *)"S", 1, 6, 9, 10, 2);
					draw_string(1650, 10, (char *)"BACK TO MAIN", 12, 6, 9, 10, 1);
					// switch page
					vinfo.yoffset = cur_page * vinfo.yres;
					ioctl(fbfd, FBIOPAN_DISPLAY, &vinfo);
					// the call to waitforvsync should use a pointer to a variable
					// https://www.raspberrypi.org/forums/viewtopic.php?f=67&t=19073&p=887711#p885821
					// so should be in fact like this:
					__u32 dummy = 0;
					ioctl(fbfd, FBIO_WAITFORVSYNC, &dummy);
					// also should of course check the return values of the ioctl calls...
					if (yloc >= vinfo.yres / 2)
						yloc = 1;
					if (xloc >= 100)
						yloc = 1;
					yloc++;
					xloc++;
				}
				//-----------------------------------------------------------graphics loop here
			}

			// unmap fb file from memory
			munmap(fbp, screensize);
			// reset cursor
			if (kbfd >= 0)
			{
				ioctl(kbfd, KDSETMODE, KD_TEXT);
				// close kb file
				close(kbfd);
			}
			// reset the display mode
			if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &orig_vinfo))
			{
				printf("Error re-setting variable information.\n");
			}
		}
	}

	/*--------------------------Get Touch And Redraw Display Here-------------------------*/


	// close fb file
	close(fbfd);

	return 0;
}


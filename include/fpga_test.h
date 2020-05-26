#ifndef __FPGA_TEST_H__
#define __FPGA_TEST_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>


#define LED_DEVICE	"/dev/fpga_led"
#define LED_MIN	0
#define LED_MAX	255
#define FND_DEVICE		"/dev/fpga_fnd"
#define FND_MAX_DIGIT	4
#define DOT_DEVICE			"/dev/fpga_dot"
#define DOT_MIN			0
#define DOT_MAX			9
#define TEXT_LCD_DEVICE		"/dev/fpga_text_lcd"
#define TEXT_LCD_MAX_BUF	32
#define TEXT_LCD_LINE_BUF	16
#define DIP_SWITCH_DEVICE "/dev/fpga_dip_switch"
#define PUSH_SWITCH_DEVICE	"/dev/fpga_push_switch"
#define IOM_PUSH_SWITCH_MAX_BUTTON	9

void assert(int cond, char *msg) {
	if (!cond) {
		printf("%s\n", msg);
		exit(1);
	}
}

void assert2(int cond, char *msg, char *src) {
	if (!cond) {
		printf("%s: %s\n", msg, src);
		exit(1);
	}
}

#endif


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

#define FND_DEVICE	"/dev/fpga_fnd"
#define FND_MAX_DIGIT	4

#define DIP_SWITCH_DEVICE	"/dev/fpga_dip_switch"

#define TEXT_LCD_DEVICE		"/dev/fpga_text_lcd"
#define TEXT_LCD_MAX_BUF	32
#define TEXT_LCD_LINE_BUF	16

#define PUSH_SWITCH_DEVICE	"/dev/fpga_push_switch"
#define PUSH_SWITCH_MAX_BUTTON	9

#define STEP_MOTOR_DEVICE	"/dev/fpga_step_motor"
#define STEP_MOTOR_ON		0
#define	STEP_MOTOR_OFF		1
#define	STEP_MOTOR_DIR_LEFT	0
#define	STEP_MOTOR_DIR_RIGHT	1
#define	STEP_MOTOR_SPDVAL_MIN	0	//min value, fastest
#define STEP_MOTOR_SPDVAL_MAX	255	//max value, slowest

#define BUZZER_DEVICE		"/dev/fpga_buzzer"
#define BUZZER_ON		0
#define BUZZER_OFF		1
#define BUZZER_TOGGLE(x)	(1-(x))

void assert(int cond, char *msg){
	if(!cond){
		printf("%s\n", msg);
		exit(1);
	}
}

void assert2(int cond, char *msg, char *src){
        if(!cond){
                printf("%s: %s\n", msg, src);
                exit(1);
        }
}

#endif

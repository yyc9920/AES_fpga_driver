#include "../include/fpga_test.h"

int dev_fnd, dev_push_switch, dev_step_motor, dev_buzzer, dev_dip_switch, dev_text_lcd;

void fnd(char* str) {
	int j;
	unsigned char data[4];
	ssize_t ret;

	memset(data, 0, sizeof(data));

	for(j = 0; j < 4; j++)
		data[j] = str[j] - '0';
	
	ret = write(dev_fnd, data, FND_MAX_DIGIT);

	sleep(1);

	memset(data, 0, sizeof(data));
	ret = read(dev_fnd, data, FND_MAX_DIGIT);

}

void reset_fnd(){
	int j;
	unsigned char data[4] = {0};
	ssize_t ret;

	memset(data, 0, sizeof(data));

	ret = write(dev_fnd, data, FND_MAX_DIGIT);

	sleep(1);

	memset(data, 0, sizeof(data));
	ret = read(dev_fnd, data, FND_MAX_DIGIT);
}

int push_switch(void) {
	unsigned char push_sw_buf[PUSH_SWITCH_MAX_BUTTON];
	int i, data, quit = 0;

	while (!quit) 
	{
		read(dev_push_switch, &push_sw_buf, sizeof(push_sw_buf));
		for(i = 0; i < PUSH_SWITCH_MAX_BUTTON; i++)
		{
			if(push_sw_buf[i])
			{
				data = i + 1;
				quit = 1;
			}
		}
	}
	return data;
}

void step_motor(unsigned char action, unsigned char dir, unsigned char speed) {
	unsigned char state[3];

	memset(state, 0, sizeof(state));
	state[0] = (unsigned char)action;
	state[1] = (unsigned char)dir;
	state[2] = (unsigned char)speed;
	
	write(dev_step_motor, state, 3);
}

void buzzer(int cnt) {
	unsigned char state;
	int count;

	state = BUZZER_OFF;
	//while (count < cnt*2) {
	for(count = 1; count <= (cnt*2)+1; count++)
	{
		state = BUZZER_TOGGLE(state);
		write(dev_buzzer, &state, 1);
		//count++;
		sleep(1);
	}
}

void text_lcd(char *first, char *second)
{
	int i;
	int str_size;
	unsigned char string[32];
	
	memset(string,0,sizeof(string));
	
	str_size=strlen(first);
	if(str_size>0) {
		strncat(string,first,str_size);
		memset(string+str_size,' ',TEXT_LCD_LINE_BUF-str_size);
	}
	
	str_size=strlen(second);
	if(str_size>0) {
		strncat(string,second,str_size);
		memset(string+TEXT_LCD_LINE_BUF+str_size,' ',TEXT_LCD_LINE_BUF-str_size);
	}
	write(dev_text_lcd,string,TEXT_LCD_MAX_BUF);
}

int otp_num(void)
{
	int rand_num = 0;
	rand_num = system("./prng");
	if(rand_num<0)
		rand_num = -rand_num;
	rand_num%=255;

	printf("%d\n", rand_num);
	return rand_num;
}

char *intToBinary(int i) 
{
	static char s[8];
	int count = 8;

	do { s[--count] = '0' + (char) (i & 1);
		i = i >> 1;
	} while (count);
	return s;
}

int dip_switch(void) {
	int dip_sw_buf = 0;
	int quit = 0;

	while (!quit) {
		if(push_switch()) quit = 1;
	}
	read(dev_dip_switch, &dip_sw_buf, 1);
	dip_sw_buf = 255 - dip_sw_buf;
	printf("%d\n", dip_sw_buf);
	return dip_sw_buf;
}

double betting(double i)
{
	double prof_los = 0;
	int rand_num = system("./prng");
	if(rand_num<0)
		rand_num = -rand_num;
	rand_num %= 100;
	if(rand_num > 97) prof_los = 10;
	else if(rand_num < 97 && rand_num > 95) prof_los = 9;
	else if(rand_num < 95 && rand_num > 93) prof_los = 8;
	else if(rand_num < 93 && rand_num > 91) prof_los = 7;
	else if(rand_num < 91 && rand_num > 89) prof_los = 6;
	else if(rand_num < 89 && rand_num > 87) prof_los = 5;
	else if(rand_num < 87 && rand_num > 85) prof_los = 4;
	else if(rand_num < 85 && rand_num > 83) prof_los = 3;
	else if(rand_num < 83 && rand_num > 80) prof_los = 2;
	else if(rand_num < 80 && rand_num > 50) prof_los = 1;
	else if(rand_num < 50 && rand_num > 30) prof_los = 0.9;
	else if(rand_num < 30 && rand_num > 25) prof_los = 0.8;
	else if(rand_num < 25 && rand_num > 22) prof_los = 0.7;
	else if(rand_num < 22 && rand_num > 18) prof_los = 0.6;
	else if(rand_num < 18 && rand_num > 15) prof_los = 0.5;
	else if(rand_num < 15 && rand_num > 12) prof_los = 0.4;
	else if(rand_num < 12 && rand_num > 9) prof_los = 0.3;
	else if(rand_num < 9 && rand_num > 5) prof_los = 0.2;
	else prof_los = 0.1;

	printf("배팅금액 : %.1f\n", i);
	i *= prof_los;

	printf("배팅결과 : %.1f\n", i);

	return i;
}
void main(void)
{
	int data[5] = {0}, i, otp_int, dip_int;
	char data_str[5] = {'0', '0', '0', '0', '0'};
	char value[4];
	char otp_bi[16], dip_bi[16];
	char *mat = "MATCH!";
	char *dis_mat = "DISMATCH!";
	
	dev_fnd = open(FND_DEVICE, O_RDWR);
	dev_push_switch = open(PUSH_SWITCH_DEVICE, O_RDONLY);
	dev_step_motor = open(STEP_MOTOR_DEVICE, O_WRONLY);
	dev_buzzer = open(BUZZER_DEVICE, O_RDWR);
	dev_text_lcd = open(TEXT_LCD_DEVICE, O_WRONLY);
	dev_dip_switch = open(DIP_SWITCH_DEVICE, O_RDONLY);
	/*
	for(i = 0; i < 5; i++)
	{
		data[i] = push_switch();
		sprintf(data_str, "%d%d%d%d%d", data[0], data[1], data[2], data[3], data[4]);
		fnd(data_str);
	}
	for(i = 0; i < 4; i++)
		sprintf(value, "%d%d%d%d",data[0], data[1], data[2], data[3]);
	printf("%s\n", value);
	reset_fnd();

	//step_motor(1, 1, 5);
	//sleep(3);
	//step_motor(0, 1, 5);

	//buzzer(4);
	*/
	otp_int = otp_num();
	sprintf(otp_bi, "%s", intToBinary(otp_int));
	text_lcd(otp_bi, "");
	
	dip_int = dip_switch();
	sprintf(dip_bi, "%s", intToBinary(dip_int));
	if(dip_int == otp_int)	
	{
		text_lcd(mat, dip_bi);
		step_motor(1, 1, 5);
		sleep(3);
		step_motor(0, 1, 5);
	}
	else 
	{
		text_lcd(dis_mat, dip_bi);
		buzzer(1);
	}
	for(i = 0; i<50; i++)	
		betting(100);

	close(dev_fnd);
	close(dev_push_switch);
	close(dev_step_motor);
	close(dev_buzzer);
	close(dev_text_lcd);
	close(dev_dip_switch);
}

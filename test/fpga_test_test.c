#include "../include/fpga_test.h"

void fnd(char* str) {
	int dev_fnd, j;
	unsigned char data[4];
	ssize_t ret;

	memset(data, 0, sizeof(data));

	for(j = 0; j < 4; j++)
		data[j] = str[j] - '0';
	
	dev_fnd = open(FND_DEVICE, O_RDWR);

	ret = write(dev_fnd, data, FND_MAX_DIGIT);

	sleep(1);

	memset(data, 0, sizeof(data));
	ret = read(dev_fnd, data, FND_MAX_DIGIT);

	close(dev_fnd);
}

void reset_fnd(){
	int dev_fnd, j;
	unsigned char data[4] = {0};
	ssize_t ret;

	memset(data, 0, sizeof(data));
	
	dev_fnd = open(FND_DEVICE, O_RDWR);

	ret = write(dev_fnd, data, FND_MAX_DIGIT);

	sleep(1);

	memset(data, 0, sizeof(data));
	ret = read(dev_fnd, data, FND_MAX_DIGIT);

	close(dev_fnd);
}

int push_switch(void) {
	unsigned char push_sw_buf[PUSH_SWITCH_MAX_BUTTON];
	int dev_push_switch, i, data, quit = 0;

	dev_push_switch = open(PUSH_SWITCH_DEVICE, O_RDONLY);

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
	close(dev_push_switch);
	return data;
}

void step_motor(unsigned char action, unsigned char dir, unsigned char speed) {
	unsigned char state[3];
	int dev_step_motor;

	memset(state, 0, sizeof(state));
	state[0] = (unsigned char)action;
	state[1] = (unsigned char)dir;
	state[2] = (unsigned char)speed;
	
	dev_step_motor = open(STEP_MOTOR_DEVICE, O_WRONLY);

	write(dev_step_motor, state, 3);

	close(dev_step_motor);
}

void buzzer(int cnt) {
	unsigned char state;
	int dev_buzzer, count;

	dev_buzzer = open(BUZZER_DEVICE, O_RDWR);

	state = BUZZER_OFF;
	//while (count < cnt*2) {
	for(count = 1; count <= (cnt*2)+1; count++)
	{
		state = BUZZER_TOGGLE(state);
		write(dev_buzzer, &state, 1);
		//count++;
		sleep(1);
	}
	close(dev_buzzer);
}

void text_lcd(char *first, char *second)
{
	int i;
	int dev_text_lcd;
	int str_size;
	unsigned char string[32];
	
	memset(string,0,sizeof(string));
	
	dev_text_lcd = open(TEXT_LCD_DEVICE, O_WRONLY);
	
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
	close(dev_text_lcd);
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
	unsigned char dip_sw_buf = 0;
	int dev_dip_switch, quit = 0;

	dev_dip_switch = open(DIP_SWITCH_DEVICE, O_RDONLY);

	while (!quit) {
		read(dev_dip_switch, &dip_sw_buf, 1);
		if(push_switch()) quit = 1;
	}
	close(dev_dip_switch);
	return dip_sw_buf;
}

void main(void)
{
	int data[5] = {0}, i, otp_int, dip_int;
	char data_str[5] = {'0', '0', '0', '0', '0'};
	char value[4];
	char otp_bi[16], dip_bi[16];
	char *mat = "MATCH!";
	char *dis_mat = "DISMATCH!";
	
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
}

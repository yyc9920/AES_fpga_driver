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

void main(void)
{
	int data[5] = {0}, i;
	char data_str[5] = {'0', '0', '0', '0', '0'};
	char value[4];
	for(i = 0; i < 5; i++)
	{
		data[i] = push_switch();
		sprintf(data_str, "%d%d%d%d%d",data[0], data[1], data[2], data[3], data[4]);
		fnd(data_str);
	}
	for(i = 0; i < 4; i++)
		sprintf(value, "%d%d%d%d",data[0], data[1], data[2], data[3]);
	printf("%s\n", value);
	reset_fnd();

	step_motor(1, 1, 5);
	sleep(3);
	step_motor(0, 1, 5);
}

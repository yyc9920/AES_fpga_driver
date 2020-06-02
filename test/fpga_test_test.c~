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

void main(void)
{
	int data[5] = {0}, i;
	char data_str[5] = {'0', '0', '0', '0', '0'};
	for(i = 0; i < 5; i++)
	{
		data[i] = push_switch();
		sprintf(data_str, "%d%d%d%d%d",data[0], data[1], data[2], data[3], data[4]);
		fnd(data_str);
	}
	printf("%s\n", data_str);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <signal.h>

char* main()
{
	int fd, ret,i;
	int cor[2];
	char rtnstr[10];
	char tmp[5];
	int cnt = 0;
	pid_t pid;
	const char* evdev_path = "/dev/input/by-path/platform-imx-i2c.2-event";
	// TODO : Find this file in host PC.
	struct input_event iev[3];
	fd = open(evdev_path, O_RDONLY);
	if(fd < 0) {
		perror("error: could not open evdev");
		return 0;
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
				x = iev[1].value;
				y = iev[2].value;
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

	sprintf(rtnstr, "%d", x);
	sprintf(tmp, "%d", y);
	strcat(rtnstr, "p");
	strcat(rtnstr, tmp);

	kill(pid,SIGINT);
	close(fd);

	return rtnstr;



}

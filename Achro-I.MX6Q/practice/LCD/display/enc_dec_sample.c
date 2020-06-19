//Simple C program to encrypt and decrypt a string

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dec(int* key, char* str, char* dec_pw){
	int i;
	for (i = 0; i < 4; i++){
		dec_pw[i] = str[i * 5] - key[i]; //the key for encryption is 3 that is subtracted to ASCII value
		if(dec_pw[i] == '0'){
			if((key[i] % 10) == 0)
				dec_pw[i] = (key[i] % 10) + 49;
			else
				dec_pw[i] = (key[i] % 10) + 48;
		}
	}
}

int main()
{
	int i, x;
	char str[100];	//in node - passwd[100]
	char temp_str[5];
	int passwd;
	int rannum;
	int rand[4];	//in node - randNum[4]
	int temp_rand;
	char dec_passwd[5];

	passwd = system("./prng");
	if (passwd > 9999)
		passwd /= 10;

	sprintf(str, "%d", passwd);

	for(int k = 0; i < 3; i++){
		passwd = system("./prng");
		if (passwd > 9999)
			passwd /= 10;


		sprintf(temp_str, "%d", passwd);

		strcat(str, temp_str);
	}

	printf("\nStr : %s\n", str);

	rannum = system("./prng");

	if (rannum > 9999)
		rannum /= 10;

	printf("\n%d\n", rannum);

	rand[0] = (rannum % 10) * 10 + (rannum % 10);
	rand[1] = (rannum / 10)%100;
	rand[2] = (rannum / 100);
	rand[3] = (rannum / 1000) * 10 + (rannum / 1000);

	for(i = 0; i < 4; i++){
		if(rand[i] > 37)
			rand[i] %= 10;
		printf("rand is : %d ", rand[i]);
	}

	for (i = 0; (i < 100 && str[i] != '\0'); i++)
		str[i] = str[i] + rand[i % 4]; //the key for encryption is 3 that is added to ASCII value

	printf("\nEncrypted string: %s\n", str);

	while (1)
	{
		printf("\nPlease choose following options:\n");
		printf("1 = Show Encrypted Password.\n");
		printf("2 = Decrypt the Password.\n");
		scanf("%d", &x);

		//using switch case statements
		switch (x)
		{
			case 1:
				printf("\nEncrypted string: %s\n", str);
				break;

			case 2:
				dec(rand, str, dec_passwd);

				printf("\nDecrypted string: %s\n", dec_passwd);
				break;

			default:
				printf("\nError\n");
		}
	}
	return 0;
}

/*
 * lab10.c
 *
 *  Created on: Apr 13, 2016
 *      Author: yzcmf
 */

/*
 * hzklab1.c
 *
 *  Created on: Jan 28, 2016
 *      Author: zhqk6
 */
#include <stdio.h>
#include <sys/mman.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


//For Part A -- hello world program

void hello_world(void)
{
 printf("Hello world.\n");
}

//For Part B -- generate a square wave to the speaker

void square_wave(void)
{
	int fp;
	unsigned long *PFDR,*PFDDR;
	unsigned long *ptr;  //create pointer to mmap

	fp = open("/dev/mem",O_RDWR);
	if(fp == -1){
		printf("\n error\n");
		return(-1);  // failed open
	}
	ptr = (unsigned long*)mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fp,0x80840000);
	if(ptr == MAP_FAILED){
		printf("\n Unable to map memory space \n");
		return(-2);
	}  // failed mmap

	PFDR = ptr+12;
	PFDDR = ptr+13;  // according to their physical address
	*PFDDR |= 0x02;  // give output/input, pbddr 11100000, pfddr 00000010

	char string[20]={0};
	printf("please enter the 'start' as the input string:");
	scanf("%s",string);

	while(strcmp(string,"start")==0){
		printf("Here\n");
		*PFDR= 0xFF;
		msync(NULL,4096,MS_SYNC);
		usleep(100000);//1s->1000000,0.1s;
		*PFDR= 0xFD;
		msync(NULL,4096,MS_SYNC);
		usleep(100000);//1s->1000000,0.1s;
	}  //create square wave
	close(fp);
	return 0;
}


//For Part C -- morseCodeToSound--another way of understanding

void morseCodeToSound(void)

{
	int fp;
	unsigned long *PFDR,*PFDDR;
	unsigned long *ptr;  //create pointer to mmap

	fp = open("/dev/mem",O_RDWR);
	if(fp == -1){
		printf("\n error\n");
		return(-1);  // failed open
	}
	ptr = (unsigned long*)mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fp,0x80840000);
	if(ptr == MAP_FAILED){
		printf("\n Unable to map memory space \n");
		return(-2);
	}  //failed mmap

	PFDR = ptr+12;
	PFDDR = ptr+13;  // according to their physical address
	*PFDDR |= 0x02;  // give output/input, pbddr 11100000, pfddr 00000010

	char string[20]={0};
	printf("please enter the Morse Code message:");
	scanf("%s",string);

	int l=strlen(string);
	int i;

	for(i=0;i<l;i++)
	{
		if(string[i]=='.')
		{
			printf("\nThe current Morse Code is %c\n",string[i]);
			*PFDR |= 0x02;
			msync(NULL,4096,MS_SYNC);
			usleep(500000);//0.5s
			*PFDR &= 0xFFFFFFFC;
			msync(NULL,4096,MS_SYNC);
			usleep(500000);//0.5s
		}

		if(string[i]=='-')
		{
			printf("\nThe current Morse Code is %c\n",string[i]);
			*PFDR |= 0x02;
			msync(NULL,4096,MS_SYNC);
			usleep(1500000);//1.5s
			*PFDR &= 0xFFFFFFFC;
			msync(NULL,4096,MS_SYNC);
			usleep(1500000);//1.5s
		}
	}

	close(fp);

}


//For Part D -- morseCodeToLights

void morseCodeToLights(void)
{
	int fp;
	unsigned long *PBDR,*PBDDR;  //create pointer to port B/F DR/DDR
	unsigned long *ptr;  //create pointer to mmap

	fp = open("/dev/mem",O_RDWR);
	if(fp == -1){
		printf("\n error\n");
		return(-1);  // failed open
	}
	ptr = (unsigned long*)mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fp,0x80840000);
	if(ptr == MAP_FAILED){
		printf("\n Unable to map memory space \n");
		return(-2);
	}  // failed mmap
	PBDR = ptr+1;
	PBDDR = ptr+5;
	*PBDDR |= 0xE0;
	*PBDR &= 0xFFFFFF00;  // clear the states

	char string[20]={0};
	printf("please enter the Morse Code message:");
	scanf("%s",string);

	int l=strlen(string);
	int i;

	for(i=0;i<l;i++)
	{
		if(string[i]=='.')
		{
			*PBDR |= 0x20;//set the pbdr 00100000--red

			printf("Current Code message is:'.'\n");

			sleep(1);

			*PBDR &= 0xFFFFFF00;//clear the states
		}
		if(string[i]=='-')
		{
			*PBDR |= 0x40;//set the pbdr 01000000--yellow

			printf("Current Code message is:'-'\n");

			sleep(3);

			*PBDR &= 0xFFFFFF00;//clear the states
		}
	}

	close(fp);


}


int main(int argc, char **argv){

//	hello_world();//part A
//	square_wave();//part B
	printf("The following is for the part C:\n");
	morseCodeToSound();//part C
	printf("The following is for the part D:\n");
	morseCodeToLights();//part D

	return 0;
}



//For Part A -- hello world program

//int main(int argc, char ** argv)
//
//{
//
//printf("Hello world.\n");
//
//
//}

////For Part B -- generate a square wave to the speaker
//
//int main(int argc, char **argv){
//	int fp;
//	unsigned long *PFDR,*PFDDR;
//	unsigned long *PBDR,*PBDDR;  //create pointer to port B/F DR/DDR
//    unsigned long *ptr;  //create pointer to mmap
//    unsigned char state;  //create button's state
//    int button;
//
//    fp = open("/dev/mem",O_RDWR);
//	if(fp == -1){
//		printf("\n error\n");
//	    return(-1);  // failed open
//	}
//	ptr = (unsigned long*)mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fp,0x80840000);
//    if(ptr == MAP_FAILED){
//    	printf("\n Unable to map memory space \n");
//    	return(-2);
//    }  // failed mmap
//	PBDR = ptr+1;
//    PBDDR = ptr+5;
//    PFDR = ptr+12;
//    PFDDR = ptr+13;  // according to their physical address
//    *PBDDR |= 0xE0;
//    *PFDDR |= 0x02;  // give output/input, pbddr 11100000, pfddr 00000010
//
//    // For button = 0; Press button 0 to skip out of the while loop and play the sound.
//	while(state & 0x01){
//    	state = *PBDR;
//	}
//
//	char string[20]={0};
//    printf("please enter the 'start' as the input string:");
//    scanf("%s",string);
////  while(getchar()!='\n');
//
//    while(strcmp(string,"start")==0){
//    	printf("Here\n");
//        *PFDR= 0xFF;
//        msync(NULL,4096,MS_SYNC);
//        usleep(100000);//1s->1000000,0.1s;
//        *PFDR= 0xFD;
//        msync(NULL,4096,MS_SYNC);
//        usleep(100000);//1s->1000000,0.1s;
//    }  //create square wave
//    close(fp);
//    return 0;
//}


////For Part C -- morseCodeToSound--first understanding eidtion,I use the second edition later.
//
//void English_To_Morsecode(char * English)
//{
//
//	int l;
//
//	l=strlen(English);
//
//	int i;
//
//	for(i=0;i<l;i++)
//	{
//		switch(English[i])// Infer each character of the English string;
//		{
//
//		//handle the upper case;
//
//		case'A': printf(".-");break;
//		case'B': printf("-...");break;
//		case'C': printf("-.-.");break;
//		case'D': printf("-..");break;
//		case'E': printf(".");break;
//		case'F': printf("..-.");break;
//		case'G': printf("--.");break;
//		case'H': printf("....");break;
//		case'I': printf("..");break;
//		case'J': printf(".---");break;
//		case'K': printf("-.-");break;
//		case'L': printf(".-..");break;
//		case'M': printf("--");break;
//		case'N': printf("-.");break;
//		case'O': printf("---");break;
//		case'P': printf(".--.");break;
//		case'Q': printf("--.-");break;
//		case'R': printf(".-.");break;
//		case'S': printf("...");break;
//		case'T': printf("-");break;
//		case'U': printf("..-");break;
//		case'V': printf("...-");break;
//		case'W': printf(".--");break;
//		case'X': printf("-..-");break;
//		case'Y': printf("-.--");break;
//		case'Z': printf("--..");break;
//
//		//handle the lower case;
//
//		case'a': printf(".-");break;
//		case'b': printf("-...");break;
//		case'c': printf("-.-.");break;
//		case'd': printf("-..");break;
//		case'e': printf(".");break;
//		case'f': printf("..-.");break;
//		case'g': printf("--.");break;
//		case'h': printf("....");break;
//		case'i': printf("..");break;
//		case'j': printf(".---");break;
//		case'k': printf("-.-");break;
//		case'l': printf(".-..");break;
//		case'm': printf("--");break;
//		case'n': printf("-.");break;
//		case'o': printf("---");break;
//		case'p': printf(".--.");break;
//		case'q': printf("--.-");break;
//		case'r': printf(".-.");break;
//		case's': printf("...");break;
//		case't': printf("-");break;
//		case'u': printf("..-");break;
//		case'v': printf("...-");break;
//		case'w': printf(".--");break;
//		case'x': printf("-..-");break;
//		case'y': printf("-.--");break;
//		case'z': printf("--..");break;
//
//
//		default : /* Optional */
//			printf("$$$"); break;
//
//		}
//	}
//
//	printf("\nMorse code message has been translated.");
//
//};
//
//
//int MorsecodeToSound(char * morsecodemessage)
//{
//
//	int period=0;
//
//	int l;
//
//	l=strlen(morsecodemessage);
//
//	int i;
//
//	for(i=0;i<l;i++)
//	{
//		switch(morsecodemessage[i])// Infer each character of the English string;
//		{
//		case '.':
//        period+=1;break;
//		case '-':
//		period+=3;break;
//		default : /* Optional */
//        break;
//		}
//	}
//
//	printf("\nMorse code message has been translated.");
//
//	return period;
//}
//
//int main(int argc, char **argv){
//	int fp;
//	unsigned long *PFDR,*PFDDR;
//	unsigned long *PBDR,*PBDDR;  //create pointer to port B/F DR/DDR
//	unsigned long *ptr;  //create pointer to mmap
//	unsigned char state;  //create button's state
//	int button;
//
//	fp = open("/dev/mem",O_RDWR);
//	if(fp == -1){
//		printf("\n error\n");
//		return(-1);  // failed open
//	}
//	ptr = (unsigned long*)mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fp,0x80840000);
//	if(ptr == MAP_FAILED){
//		printf("\n Unable to map memory space \n");
//		return(-2);
//	}  //failed mmap
//	PBDR = ptr+1;
//	PBDDR = ptr+5;
//	PFDR = ptr+12;
//	PFDDR = ptr+13;  // according to their physical address
//	*PBDDR |= 0xE0;
//	*PFDDR |= 0x02;  // give output/input, pbddr 11100000, pfddr 00000010
//
//	// For button = 0; Press button 0 to skip out of the while loop and play the sound.
//	while(state & 0x01){
//		state = *PBDR;
//	}
//
//	int base=1000000;//set the base time period to 1s;
//    int times;//For the period divided by the time;
//
//	char string1[20]={0};
//	printf("please enter the English message:");
//	scanf("%s",string1);
//
//    English_To_Morsecode(string1);//print the Morsecode message after the translation;
//
//    char string2[20]={0};
//    printf("\nplease enter the Morsecode message shows on the screen again:");
//    scanf("%s",string2);
//    printf("Current Morse Code is %s:", string2);
//    times=MorsecodeToSound(string2);
//    printf("\nThe sleep period for the speaker is %d s.",times);
//
//	while(1){
//		printf("\nHere");
//		*PFDR |= 0x02;
//		msync(NULL,4096,MS_SYNC);
//		usleep(times*base);//1s->1000000;
//		*PFDR &= 0xFFFFFFFC;
//		msync(NULL,4096,MS_SYNC);
//		usleep(times*base);//1s->1000000;
//	} //create square wave
//
//	close(fp);
//	return 0;
//}



















 
 
 
 
 

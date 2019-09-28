#include <time.h>
#include <stdlib.h>
#include <stdio.h>

const double THtime=0.01;
const int sizeArr=10;
int index=0;
unsigned char * array;
unsigned char r;

void testElapsedTime(){
	struct timespec start, finish;
	double elapsed;
	for(int i=0;i<sizeArr;i++){
		clock_gettime(CLOCK_MONOTONIC, &start);
		for(;;){
			r = (unsigned char)(rand()%256); 
			clock_gettime(CLOCK_MONOTONIC, &finish);
			elapsed = (finish.tv_sec - start.tv_sec);
			elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
			if(elapsed>THtime){	
				break;
			}
		}
		array[i]=r;
	}
}

int main(){
	array =(unsigned char *)malloc(sizeof(char)*sizeArr);
	srand(time(NULL));   // Initialization, should only be called once.
	testElapsedTime();
	for(int i=0;i<sizeArr;i++){
		char tt=array[i]^0x20;
		printf("%d \n",array[i]);
		printf("%c \n",tt);
	}
	
	free(array);
}


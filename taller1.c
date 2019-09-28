#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

/* threshold time */
const double THtime=0.01;
/* size of the array */
const int sizeArr=13;
unsigned char * array;
unsigned char r;
int index=0;
int flag=0;

/* var to make the block */
pthread_mutex_t mutex1 =PTHREAD_MUTEX_INITIALIZER;

void *numberGen( void *ptr1 );
void *xorMap( void *ptr2 );

int main(){	
	pthread_t thread1, thread2;
	int  iret1, iret2;
	
	/* allocating array */
	array =(unsigned char *)malloc(sizeof(char)*sizeArr);
	srand(time(NULL));
	
	/* init first thread */
	iret1 = pthread_create( &thread1, NULL, numberGen,NULL);
	/* cheking if the first thread is created right */
	if(iret1){
		fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
		exit(EXIT_FAILURE);
	}

	/* init second thread */
	iret2 = pthread_create( &thread2, NULL, xorMap,NULL);
	/* cheking if the second thread is created right */
	if(iret2){
		fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
		exit(EXIT_FAILURE);
	}

	//printf("pthread_create() for thread 1 returns: %d\n",iret1);
	//printf("pthread_create() for thread 2 returns: %d\n",iret2);

	/* Wait till threads are complete before main continues. Unless we  */
	/* wait we run the risk of executing an exit which will terminate   */
	/* the process and all threads before the threads have completed.   */

	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL); 
	
	free(array);
	exit(EXIT_SUCCESS);
}

/**
	method executed by the first thread witch calculate 
	a random number and then saves on the array read by the 
	second thread.
*/
void * numberGen( void *ptr1 ){
	struct timespec start, finish;
	double elapsed;
	for(int i=0;i<sizeArr;i++){
		clock_gettime(CLOCK_MONOTONIC, &start);
		for(;;){
			r = (unsigned char)(rand()%256);      // Returns a pseudo-random integer between 0 and RAND_MAX.
			clock_gettime(CLOCK_MONOTONIC, &finish);

			elapsed = (finish.tv_sec - start.tv_sec);
			elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
			if(elapsed>THtime){	
				break;
			}
		}
		pthread_mutex_lock( &mutex1 );
		int ff=flag;/**/
		pthread_mutex_unlock( &mutex1 );
		if(ff==0){
			pthread_mutex_lock( &mutex1 );
			array[i]=r;/**/
			index=i;/**/
			flag=1;/**/
			pthread_mutex_unlock( &mutex1 );
		}
	}
	pthread_mutex_lock( &mutex1 );
	printf("termino hilo 1 \n");
	pthread_mutex_unlock( &mutex1 );
	return NULL;
}

/**
	method executed by the thread witch calculate 
	the number generate by the first thread, the 
	apply the xor of 0x20 and the print.
*/
void * xorMap( void *ptr2 ){
	for(int i=0;i<sizeArr-1;){
		pthread_mutex_lock( &mutex1 );
		int f=flag;/**/
		pthread_mutex_unlock( &mutex1 );
		if(f==1){
			pthread_mutex_lock( &mutex1 );
			i=index;/**/
			char t1=array[i];/**/
			pthread_mutex_unlock( &mutex1 );
			char tt=t1^0x20;
			printf("%d : %c \n",i,tt);
			pthread_mutex_lock( &mutex1 );
			flag=0;/**/
			pthread_mutex_unlock( &mutex1 );
		}
	}
	pthread_mutex_lock( &mutex1 );
	printf("termino hilo 2 \n");
	pthread_mutex_unlock( &mutex1 );
	return NULL;
}

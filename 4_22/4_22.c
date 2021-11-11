#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

typedef struct statistics
{
	int max, min;
	float avg, std_dev, med;
	int *arr;
}stats;

stats results;
int len;

void *max_num(void *arg)
{
	int *arr = (int *) arg;
	int nmax = arr[0];
	for(int i = 1; i < len; i++)
	{
		if(nmax<arr[i])
			nmax = arr[i];
	}
	results.max = nmax;
	pthread_exit(NULL);
}

void *min_num(void *arg)
{
	int *arr = (int *) arg;
	int nmin = arr[0];
	for(int i = 1; i < len; i++)
	{
		if(nmin>arr[i])
			nmin = arr[i];
	}
	results.min = nmin;
	pthread_exit(NULL);
}

void *arr_avg(void *arg)
{
	int *arr = (int *) arg;
	int sum = 0;
	for(int i = 0; i < len; i++)
	{
		sum += arr[i];
	}
	results.avg = (float) sum / (float) len;
	pthread_exit(NULL);
}

void *standard_dev(void *arg)
{
	int *arr = (int *) arg;
	float temp_sum = 0;
	for(int i = 0; i < len; i++)
	{
		temp_sum += pow(((float)arr[i]-len), (float)2);
	}
	results.std_dev = sqrt(temp_sum / (float) len);
	pthread_exit(NULL);
}

void *arr_med(void *arg)
{
	int *arg_arr = (int *)arg;
	int *arr = (int *)malloc(sizeof(int)*len);
	for(int i = 0; i < len; i++)
		arr[i] = arg_arr[i];
	for(int i = 0; i < len - 1; i++)
	{
		int sw = 0;
		for(int j = 0; j < len - i - 1; j++)
		{
			if(arr[j] > arr[j+1])
			{
				arr[j] = arr[j] ^ arr[j+1];
				arr[j+1] = arr[j] ^ arr[j+1];
				arr[j] = arr[j] ^ arr[j+1];
				sw = 1;
			}
		}
		if(sw == 0)
			break;
	}
	if(len % 2 == 0)
		results.med = (float)(arr[len/2]+arr[len/2-1]) / (float) 2;
	else
		results.med = arr[len/2];

}
int main(void)
{
	printf("Enter number of elements in the array: ");
	scanf("%d", &len);
	int *arr = (int*)malloc(sizeof(int)*len);
	for(int i = 0; i < len; i++)
		scanf("%d", &arr[i]);
	pthread_t tid[4];
	pthread_create(&tid[0], NULL, arr_avg, (void*)arr);
	pthread_create(&tid[1], NULL, max_num, (void*)arr);
	pthread_create(&tid[2], NULL, min_num, (void*)arr);
	pthread_create(&tid[3], NULL, arr_med, (void*)arr);
	pthread_join(tid[0], NULL);
	pthread_create(&tid[0], NULL, standard_dev, (void*)arr);
	for(int i = 0; i < 3; i++)
		pthread_join(tid[i], NULL);
	printf("The average is %f\nThe minimum is %d\nThe maximum is %d\nThe standard deviation is %f\nThe Median is %f\n",results.avg, results.min, results.max, results.std_dev, results.med);
	free(arr);
	return 0;
}

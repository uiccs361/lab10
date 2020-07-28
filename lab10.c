#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#define NW ((2))
#define N ((10000000))

struct pair { 
	volatile int a; 
	volatile int b; 
};

static void * worker(void * ptr)
{
	struct pair * const p = (typeof(p))ptr;
	for (int i = 0; i < N; i++) {
		p->a++; // move values from b to a
		p->b--; // (a+b) should be invariant
	}
	return NULL;
}
static void * observer(void * ptr)
{
	struct pair * const p = (typeof(p))ptr;
	int err = 0;
	for (int i = 0; i < N; i++)
		if ((p->a + p->b) != N*2)
			err++;
	printf("%s #err=%d/%d\n", __func__, err, N);
	return NULL;
}
int main(void)
{
	struct pair p = {.a = 0, .b = N*2};
	printf("First: a=%d, b=%d, a+b=%d\n", p.a, p.b, p.a + p.b);

	pthread_t pts[NW]; // two workers
	for (int i = 0 ; i < NW; i++){
		pthread_create(&pts[i], NULL, worker, &p);
	}
	pthread_t po;
	pthread_create(&po, NULL, observer, &p);
	for (int i = 0 ; i < NW; i++){
		pthread_join(pts[i], NULL);
	}
	pthread_join(po, NULL);
	printf("Last: a=%d, b=%d, a+b=%d\n", p.a, p.b, p.a + p.b);
	return 0;
}

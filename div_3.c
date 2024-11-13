#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#define	LOGIN	"kemuniku"

long long count;
int loop;

#define	LOOP_SEC (2)



static __inline int mul(int x,int y) {
//
// 以下を削除してここに乗算のロジックを入れてください。x=32bit , y=32bit, result=64bit
//	
	register int a = x;
	register int b = y;
	#define mul_inner if(a&1){result += b;}a >>= 1;b <<= 1;
	#define mul_inner2 mul_inner mul_inner mul_inner mul_inner if(a==0){return result;}
	#define mul_inner3 mul_inner2 mul_inner2 mul_inner2 mul_inner2 
	register int result = 0;
	mul_inner3 mul_inner3
	return result;
}

static __inline int div(int x,int y) {
//
// 以下を削除してここに除算のロジックを入れてください。x=32bit , y=32bit, result=32bit
//
	register int ys = y;
    ys <<= (__builtin_clz(y)-1);
    register int now = x;
    register int result = 0;
    register int tmp = 1<<(__builtin_clz(y)-1);
    #define div_inner if(now >= ys){now -= ys;result |= tmp;}tmp >>= 1; ys >>= 1;
    #define div_inner2 div_inner div_inner div_inner div_inner if(tmp==0){return result;}
    #define div_inner3 div_inner2 div_inner2 div_inner2 div_inner2
    div_inner3 div_inner3
    return result;
}

void alarm_mul(int signum){
	printf("MUL: %lld K instructions/sec\n", count/LOOP_SEC/1000);
	loop = 0;
}

void alarm_div(int signum){
	printf("DIV: %lld K instructions/sec\n", count/LOOP_SEC/1000);
	loop = 0;
}

int main() {
	int x,y;


	printf("Start (%s program)\n", LOGIN);


	// Benchmark MUL
	count = 0;
	loop = 1;
	signal(SIGALRM, alarm_mul);
	alarm(LOOP_SEC);
	for (x=1; loop ; ++x) {
		for (y=1; y<0xffff; ++y) {
			if (mul(x,y) != x*y) {
				printf("MUL: Calculation results are incorrect. results=%d, expected=%d\n", mul(x,y), x*y);
				return(-1);
			}
			++count;
		}
	}

	// Benchmark DIV
	count = 0;
	loop = 1;
	signal(SIGALRM, alarm_div);
	alarm(LOOP_SEC);
	for (x=0x0fffffff; loop ; --x) {
		for (y=1; y<0xffff; ++y) {
			if (div(x,y) != x/y) {
				printf("DIV: Calculation results are incorrect. results=%d, expected=%d\n", div(x,y), x/y);
				return(-1);
			}
			++count;
		}
	}

	return 0;

}

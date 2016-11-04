#include <iostream>
#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>
#define HOLE 17	
#define HOLE2 22

void *thread_func(void *arg)
{
	pinMode(18, PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);
	pwmSetClock(375);
	pwmSetRange(1024);
	pwmWrite(18,27 );
	int i = 0, m = 0;
	while (1){
	m = digitalRead(HOLE);
	i = digitalRead(HOLE2);
	//if (m == 0){
  	pwmWrite(18, 73);
	//}
	//if (i == 0){
	//pwmWrite(18,27);
	//}
	}
}

int main()
{
	pthread_t th;
	char buf[1024];
	int stat;
	//GPIOèâä˙âª
	if (wiringPiSetupGpio() == -1) return 1;
	else stat = pthread_create(&th, NULL, thread_func, (void *)(buf));
	//for(int i = 0; i < 10; i++){
		sleep(1);
	//}
	return 0;
}


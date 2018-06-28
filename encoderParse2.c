#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define CHANNEL_A_COL 4
#define CHANNEL_B_COL 5
#define MAX_LINE 100
#define OUTPUT_FORMAT "%lf,%lf,%lf,%d\n"

int main(int argc, char *argv[]){
	if(argc==1)
		return 1;
	FILE *fpi = fopen(argv[1], "r");
	char outName[100];
	if(argc == 2){
		fprintf(stderr,"Only one parameter, putting output in speed.txt");
		strcpy(outName,"speed.txt");
	}
	else
		strcpy(outName, argv[2]);
	FILE *fpo = fopen(outName,"w");

	char line[MAX_LINE];

	double lastChangeTime = 0;
	double lastPWMEdge = 0;
	double currentDutyCycle = 0;
	bool lastA = 0;
	bool lastB = 0;
	bool lastPWM = 0;

	fprintf(fpo,"Time,Velocity,Duty Cycle,Mode\n");
	int i = 0;

	while(fgets(line,MAX_LINE-1,fpi) != NULL){
		i++;
		double time;
		int pwm,selA,selB,A,B;
		sscanf(line, "%lf, %u, %u, %u, %u, %u", &time, &pwm, &selA, &selB, &A, &B);
		if(time != 0){
			if(lastA == 1 && A == 0 && B == 1
				|| lastA == 0 && A == 1 && B == 0
				|| lastB == 0 && B == 1 && A == 1
				|| lastB == 1 && B == 0 && A == 0){
				fprintf(fpo,OUTPUT_FORMAT,time,-1/(1000*(time-lastChangeTime)),currentDutyCycle,(selA<<1)+selB);
				lastChangeTime = time;
			}
			else if(lastA != A || lastB != B){
				fprintf(fpo,OUTPUT_FORMAT,time,1/(1000*(time-lastChangeTime)),currentDutyCycle,(selA<<1)+selB);
				lastChangeTime = time;
			}
		}
		if(lastPWM == 0 && pwm == 1)
			lastPWMEdge = time;
		else if(lastPWM == 1 && pwm == 0)
			currentDutyCycle = (time-lastPWMEdge)*1000;
		lastA = A;
		lastB = B;
		lastPWM = pwm;
	}
	fclose(fpo);
	fclose(fpi);
	fprintf(stderr, "Done parsing %u lines of data",i);
	return 0;

}
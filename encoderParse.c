#import <stdio.h>
#import <stdbool.h>
#import <string.h>

#define CHANNEL_A_COL 4
#define CHANNEL_B_COL 5
#define MAX_LINE 100

int main(int argc, char *argv[]){
	if(argc==1)
		return 1;
	FILE *fpi = fopen(argv[1], "r");
	char outName[100];
	if(argc == 2)
		strcpy(outName,"speed.txt");
	else
		strcpy(outName, argv[2]);
	FILE *fpo = fopen(outName,"w");

	char line[MAX_LINE];

	double lastChangeTime = 0;
	bool lastA = 0;
	bool lastB = 0;

	while(fgets(line,MAX_LINE-1,fpi) != NULL){
		double time;
		int pwm,selA,selB,A,B;
		sscanf(line, "%lf, %u, %u, %u, %u, %u", &time, &pwm, &selA, &selB, &A, &B);
		if(A != lastA || B != lastB){
			fprintf(fpo,"%lf,%lf\n",time,1/(time-lastChangeTime));
			lastChangeTime = time;
		}
		lastA = A;
		lastB = B;
	}

}
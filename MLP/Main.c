#include <stdlib.h>
#include <stdio.h>
#include "PN.h"

char belongsTo(double* y) {
	char max_sel =  0;
	for (int i = 1; i < OutputC; i++)if (y[i] > y[max_sel])max_sel = i;
	return max_sel + 1;
}

static inline void setVal(double* y, int count, int place) {
	y[place] = 1.0;
	int i = 0;
	for (i; i < place; i++)y[i] = 0.0;
	i++;
	for (i; i < count; i++)y[i] = 0.0;
}

void encodedTo(char team,double* y) {
	setVal(y, OutputC, team - 1);
}

int main() {
	FILE* train;
	FILE* test;
	FILE* OK;
	FILE* FALSE[4];
	fopen_s(&train, "trainSet.txt", "r");
	fopen_s(&test, "testSet.txt", "r");
	fopen_s(&OK, "correct.txt", "w");
	fopen_s(FALSE, "False_1.txt", "w");
	fopen_s(FALSE +1, "False_2.txt", "w");
	fopen_s(FALSE + 2, "False_3.txt", "w");
	fopen_s(FALSE + 3, "False_4.txt", "w");
	double(*trainData)[2] = (double (*)[2])malloc(4000 * sizeof(double[2]));
	char* trainDataResults = (char *)malloc(4000);
	for(int i = 0 ; i < 4000 ;i++)fscanf_s(train, "%lf %lf %hhu\n", &(trainData[i][0]), &(trainData[i][1]), &(trainDataResults[i]));
	PN* network = PN_Create();
	PN_Train(network, trainData, trainDataResults, encodedTo, 4000);
	free(trainData);
	free(trainDataResults);
	int found = 0;
	unsigned int predictionErrors[4][4] = { 0 };
	for (int i = 0; i < 4000; i++) {
		char real;
		char predicted;
		double x[2];
		double y[OutputC];
		fscanf_s(test, "%lf %lf %hhu\n", x,x+1,&real);
		PN_Predict(network, x, y);
		predicted = belongsTo(y);
		
		if (real == predicted) {
			fprintf_s(OK, "%lf %lf %hhu\n", x[0],x[1], predicted);
			found += 1;
		}
		else {
			fprintf_s(FALSE[predicted - 1], "%lf %lf %hhu\n",x[0],x[1], real);
			
		}
		predictionErrors[predicted - 1][real - 1] += 1;
	}
	printf("Found %i , %lf%%\n", found, ((double)found / 4000) * 100.0);
	printf("Real | 1 2 3 4\nPredicted\n");
	for (int i = 0; i < 4; i++) printf("%i | %u %u %u %u\n", i + 1, predictionErrors[i][0],
		predictionErrors[i][1], predictionErrors[i][2], predictionErrors[i][3]);
	free(network);
	fclose(test);
	fclose(train);
	fclose(OK);
	fclose(FALSE[0]);
	fclose(FALSE[1]);
	fclose(FALSE[2]);
	fclose(FALSE[3]);
	return;
}
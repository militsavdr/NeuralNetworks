#include <stdio.h>
#include <stdlib.h>

double trainSet[4000][3];
double testSet[4000][3];

void init(double writeout[][3]){
    double condX1A,condX1B , condX2A , condX2B;
    for(int i = 0 ; i < 4000 ; i += 1){
        writeout[i][0] = (double)rand() / (double)RAND_MAX;
        writeout[i][1] = (double)rand() / (double)RAND_MAX;
        writeout[i][0] *= ((double)rand() / (double)RAND_MAX > 0.5) ? 1.0 : -1.0;
        writeout[i][1] *= ((double)rand() / (double)RAND_MAX > 0.5) ? 1.0 : -1.0;
        condX1A = writeout[i][0] - 0.5;
        condX1A *= condX1A;
        condX2A = writeout[i][1] - 0.5;
        condX2A *= condX2A;
        condX1B = writeout[i][0] + 0.5;
        condX1B *= condX1B;
        condX2B = writeout[i][1] + 0.5;
        condX2B *= condX2B;
        if(writeout[i][0] > 0.5 && (condX1A + condX2A < 0.2 || condX1A + condX2B < 0.2))writeout[i][2] = 1.0;
        else if(writeout[i][0] < 0.5 && (condX1A + condX2A < 0.2 || condX1A + condX2B < 0.2))writeout[i][2] = 2.0;
        else if(writeout[i][0] > -0.5 && (condX1B + condX2B < 0.2 || condX1B + condX2A < 0.2))writeout[i][2] = 1.0;
        else if(writeout[i][0] < -0.5 && (condX1B + condX2B < 0.2 || condX1B + condX2A < 0.2)) writeout[i][2] = 2.0;
        else if(writeout[i][0] > 0) writeout[i][2] = 3.0;
        else writeout[i][2] = 4.0;
    }
}

int main(){
    FILE* fp1;
    FILE* fp2;
    fopen_s(&fp1,"trainSet.txt", "w");
    fopen_s(&fp2,"testSet.txt", "w");
    if (!fp1 || !fp2)return 1;
    init(trainSet);
    init(testSet);
    for(int i = 0 ; i < 4000 ; i += 1){
        char tochar = trainSet[i][2];
        fprintf_s(fp1,"%lf %lf %hhu\n",trainSet[i][0],trainSet[i][1],tochar);
        tochar = testSet[i][2];
        fprintf_s(fp2,"%lf %lf %hhu\n",testSet[i][0],testSet[i][1],tochar);
    }
    fclose(fp1);
    fclose(fp2);
    return 0;
}
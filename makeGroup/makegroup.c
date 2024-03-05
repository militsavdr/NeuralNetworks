#include <stdio.h>
#include <stdlib.h>

double pointsData[1200][2];

int makeData(int place , int count , double x[2] , double y[2]){
    count = count + place;
    while(place < count){
        pointsData[place][0] = x[0] + ((double)rand() / (double)RAND_MAX) * (x[1] - x[0]);
        pointsData[place][1] = y[0] + ((double)rand() / (double)RAND_MAX) * (y[1] - y[0]);
        place += 1;
    }
    return count;
}

int main(){
    FILE* fp;
    fopen_s(&fp,"groupData.txt", "w");
    if (!fp)return 1;
    double xlimits[2];
    double ylimits[2];
    int position = 0;
    xlimits[0] = ylimits[0] = 0.8;
    xlimits[1] = ylimits[1] = 1.2;
    position = makeData(position,150,xlimits,ylimits);
    xlimits[0] = ylimits[0] = 0.0;
    xlimits[1] = ylimits[1] = 0.5;
    position = makeData(position,150,xlimits,ylimits);
    xlimits[0] = 1.5;
    xlimits[1] = 2.0;
    ylimits[0] = 0.0;
    ylimits[1] = 0.5;
    position = makeData(position,150,xlimits,ylimits);
    xlimits[0] = 0.0;
    xlimits[1] = 0.5;
    ylimits[0] = 1.5;
    ylimits[1] = 2;
    position = makeData(position,150,xlimits,ylimits);
    xlimits[0] = 1.5;
    xlimits[1] = 2.0;
    ylimits[0] = 1.5;
    ylimits[1] = 2.0;
    position = makeData(position,150,xlimits,ylimits);
    xlimits[0] = 0.0;
    xlimits[1] = 0.4;
    ylimits[0] = 0.8;
    ylimits[1] = 1.2;
    position = makeData(position,75,xlimits,ylimits);
    xlimits[0] = 1.6;
    xlimits[1] = 2.0;
    ylimits[0] = 0.8;
    ylimits[1] = 1.2;
    position = makeData(position,75,xlimits,ylimits);
    xlimits[0] = 0.8;
    xlimits[1] = 1.2;
    ylimits[0] = 0.3;
    ylimits[1] = 0.7;
    position = makeData(position,75,xlimits,ylimits);
    xlimits[0] = 0.8;
    xlimits[1] = 1.2;
    ylimits[0] = 1.3;
    ylimits[1] = 1.7;
    position = makeData(position,75,xlimits,ylimits);
    xlimits[0] = 0.0;
    xlimits[1] = 2.0;
    ylimits[0] = 0.0;
    ylimits[1] = 2.0;
    position = makeData(position,150,xlimits,ylimits);
    for(int i = 0 ; i < 1200;i++){
        fprintf_s(fp,"%lf %lf\n",pointsData[i][0] , pointsData[i][1]);
    }
    fclose(fp);
    return 0;
}
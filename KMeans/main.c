#include <stdio.h>
#include <stdlib.h>
#include "kmeansHeader.h"

#define PROGRAM_RUN 15
#define M 12
#define POINTS_COUNT 1200

static inline short cmpVals(Cluster* cls, Point* point ,unsigned int Count) {
    for (unsigned int i = 0; i < Count; i++) {
        if (point[i].x != cls[i].center.x || point[i].y != cls[i].center.y)return 1;
    }
    return 0;
}

int main(int argc , char ** argv) {

    FILE* fp;
    fopen_s(&fp,"groupData.txt", "r");
    if (!fp) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }
    //unsigned int M = argc > 1 ? atoi(argv[1]) : 3;

    Point * points = (Point *)malloc(POINTS_COUNT * sizeof(Point));
    {
        unsigned int count = 0;
        while (fscanf_s(fp, "%lf %lf\n", &points[count].x, &points[count].y) > 0) count++;
    }
    fclose(fp);
    unsigned int Program_counter = PROGRAM_RUN;
    Cluster* allClusters = (Cluster *)malloc(PROGRAM_RUN * M * sizeof(Cluster));
    double* allErrors = (double*)malloc(PROGRAM_RUN * sizeof(double));
    while (Program_counter--) {
        Point* previous = (Point*)malloc(M * sizeof(Point));
        Cluster* clusters = (Cluster*)malloc(M * sizeof(Cluster) );
        for (unsigned int i = 0; i < M; i++)clusters[i] = initCluster((rand() % 2) + (1.0 / (1 + rand())), (rand() % 2) + (1.0 / (1 + rand())));

        // PerforM the k-Means algorithm
        unsigned iteration = 0;
        do {
            for (unsigned int i = 0; i < M; i++)previous[i] = clusters[i].center;
            assign(points, clusters, M, POINTS_COUNT);
            updateClusters(clusters, M);
            iteration++;
        } while (cmpVals(clusters, previous, M));
        double div = computeDiviation(clusters, M);
        double error = computeError(clusters, M);
        printf("Epoch : %i - Deviasion: %lf - Error: %lf\n",iteration,div, error);
        for (unsigned int i = 0; i < M; i++)allClusters[(Program_counter * M) + i] = clusters[i];
        allErrors[Program_counter] = error;
        free(clusters);
        free(previous);
    }
    unsigned int pos = 0;
    for (int i = 1; i < PROGRAM_RUN; i++) {
        if (allErrors[i] < allErrors[pos])pos = i;
    }
    fopen_s(&fp,"clusters.txt","w");
    for (unsigned int i = 0; i < M; i++)fprintf(fp, "%lf %lf \n", allClusters[(pos * M) + i].center.x, allClusters[(pos * M) + i].center.y);
    free(points);
    free(allClusters);
    free(allErrors);
    fclose(fp);
    return 0;
}
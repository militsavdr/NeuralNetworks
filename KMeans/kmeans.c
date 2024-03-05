#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "kmeansHeader.h"




double euclideanDist(Point b ,Point a){
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

void assign(Point *points , Cluster *clusters , unsigned int ClusterCount , unsigned int PointCount){
    for (unsigned int j = 0; j < ClusterCount; j++) { 
        free(clusters[j].points); 
        clusters[j].points = NULL; 
        clusters[j].count = 0;
    }
    for(unsigned int i=0;i< PointCount;i++){
        double mindist=euclideanDist(clusters[0].center,points[i]);

        unsigned int temp=0;
        for(unsigned int j=1;j< ClusterCount;j++){
            double dist=euclideanDist(clusters[j].center,points[i]);
            if(mindist>dist){
                mindist=dist;
                temp=j;
            }
        }
        if (clusters[temp].points == NULL) {
            clusters[temp].points = (Point*)malloc(sizeof(Point));
            clusters[temp].points[0] = points[i];
            clusters[temp].count = 1;
        }
        else {
            clusters[temp].points = 
                (Point*)realloc( clusters[temp].points, (++clusters[temp].count) * sizeof(Point));
            clusters[temp].points[clusters[temp].count - 1]=points[i];
        }
        
    }

}

Cluster initCluster(double x, double y) {
    Cluster result;
    result.center.x = x;
    result.center.y = y;
    result.points = NULL;
    result.count = 0;
    return result;
}

void updateClusters(Cluster *clusters , int MA){

    for(int i=0;i<MA;i++){
        double sumX =0.0 , sumY =0.0;

        for(int j=0; j<clusters[i].count;j++){
            sumX+=clusters[i].points[j].x;
            sumY+=clusters[i].points[j].y;

        }
        if(clusters[i].count>0){
            clusters[i].center.x=sumX/clusters[i].count;
            clusters[i].center.y=sumY/clusters[i].count;
        }
        
    }
}

double computeDiviation( Cluster *clusters , int MA){
    double div=0;
    for(int i=0; i< MA;i++){
        double error = 0;
        for (int j = 0; j < clusters[i].count; j++) {
            error += pow(euclideanDist(clusters[i].center, clusters[i].points[j]), 2);
        }
        error /= clusters[i].count;
        for (int j = 0; j < clusters[i].count; j++) {
            double dist = pow(euclideanDist(clusters[i].center, clusters[i].points[j]), 2);
            div += pow(error - dist,2);
        }
    }
    return div;
}

double computeError(Cluster* clusters, int MA) {
    double error = 0;
    for (int i = 0; i < MA; i++) {

        for (int j = 0; j < clusters[i].count; j++) {
            error += pow(euclideanDist(clusters[i].center, clusters[i].points[j]), 2);
        }
    }
    return error;
}

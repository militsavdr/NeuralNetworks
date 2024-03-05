#define ITERATIONS 100

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point center;
    int count;
    Point *points;
} Cluster;

double euclideanDist(Point b, Point a);
void assign(Point* points, Cluster* clusters, unsigned int ClusterCount, unsigned int PointCount);
void updateClusters(Cluster *clusters, int MA);
double computeDiviation(Cluster *clusters, int MA);
double computeError(Cluster* clusters, int MA);
Cluster initCluster(double x,double y);

#define NH1 40
#define NH2 30
#define NH3 50
#define InputC 2
#define OutputC 4
#define B 10
#define N 100

typedef struct PerceptronNetwork PN;

void PN_Predict(PN* this, double* x, double * y);
void PN_Train(PN* this, double (* X)[2], char* data, void (*toArray)(char, double*), int Total);

PN* PN_Create();

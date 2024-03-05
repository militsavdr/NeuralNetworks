#include <math.h>
#include <stdlib.h>
#include "PN.h"
#include <stdio.h>


typedef struct PerceptronNetwork {
	double NH1Layer[NH1][InputC + 1];
	double NH2Layer[NH2][NH1 + 1];
	double NH3Layer[NH3][NH2 + 1];
	double Output[OutputC][NH3 + 1];
}PN;

static double relu(double input) {
	return input > 0 ? input : 0;
}

static inline double relu_deriv(double input) {
	return input > 0 ? 1 : 0;
}

static double logistic(double input) {
	return 1 / (1 + exp(-input));
}

static inline double logistics_deriv(double input) {
	double s = logistic(input);
	return s * (1 - s);
}

static double tahn(double input) {
	return (exp(input) - exp(-input)) / (exp(input) + exp(-input));
}

static inline double tahn_deriv(double input) {
	double h = tahn(input);
	return 1 - (h * h);
}


static inline double calcError(double* ideal, double* real, double* errorTot) {
	double result = 0.0;
	for (int i = 0; i < OutputC; i++) {
		double firstFactor = (real[i] - ideal[i]);
		errorTot[i] = firstFactor;
		result += (firstFactor * firstFactor);
	}
	return (result) / 2;
}

static void init(double *array,int count,int size) {
	int total = count * size;
	for (int i = 0; i < total; i++)array[i] = (rand() % 2 == 1 ? -1 : 1) * (1.0 / (rand() + 1));
	return;
}

static double neuron_trigger(double * Input , double * weights , unsigned int count , double bias , double (*actfunc)(double)) {
	double total = 0;
	for (unsigned int i = 0; i < count; i++)total += Input[i] * weights[i];
	return actfunc(total + bias);
}


static void forwardPropagation(PN* this, double* Input, double * Output,double * NH1_Output, double* NH2_Output, double* NH3_Output) {
	//Passing information through the network , 
	// the output of each layer are stored internaly , 
	// except the final output
	for (int i = 0; i < NH1; i++)NH1_Output[i] = neuron_trigger(Input, this->NH1Layer[i], InputC, this->NH1Layer[i][InputC],tahn);
	for (int i = 0; i < NH2; i++)NH2_Output[i] = neuron_trigger(NH1_Output, this->NH2Layer[i], NH1, this->NH2Layer[i][NH1], tahn);
	for (int i = 0; i < NH3; i++)NH3_Output[i] = neuron_trigger(NH2_Output, this->NH3Layer[i], NH2, this->NH3Layer[i][NH2], logistic);
	for (int i = 0; i < OutputC; i++)Output[i] = neuron_trigger(NH3_Output, this->Output[i], NH3, this->Output[i][NH3], logistic);
}
static double backprop(PN* this, double (*X)[2], double(* Ideal)[OutputC], int countOf) {
	//Malloc is used to prevent stack overflow , These arrays store the Network output and its error per neuron
	double * Real = (double*)malloc(OutputC * sizeof(double));
	double * errorParts = (double*)malloc(OutputC * sizeof(double));
	//The output of each layer , useful for forward propagation and for this function
	double * NH1_Output = (double*)malloc(NH1 * sizeof(double));
	double * NH2_Output = (double*)malloc(NH2 * sizeof(double));
	double * NH3_Output = (double*)malloc(NH3 * sizeof(double));
	//The delta values for each layer , used to calaculate error's derivative
	double * delta_Real = (double*)malloc(OutputC * sizeof(double));
	double * delta_NH3 = (double*)malloc(NH3 * sizeof(double));
	double * delta_NH2 = (double*)malloc(NH2 * sizeof(double));
	double * delta_NH1 = (double*)malloc(NH1 * sizeof(double));
	//The error derivative matrix , will be used at the end for their corresponded layer
	double (* derriv_Real)[NH3 + 1] = (double(*)[NH3 + 1])calloc(OutputC, sizeof(double[NH3 + 1]));
	double(*derriv_NH3)[NH2 + 1] = (double(*)[NH2 + 1])calloc(NH3, sizeof(double[NH2 + 1]));
	double(*derriv_NH2)[NH1 + 1] = (double(*)[NH1 + 1])calloc(NH2, sizeof(double[NH1 + 1]));
	double(*derriv_NH1)[InputC + 1] = (double(*)[InputC + 1])calloc(NH1, sizeof(double[InputC + 1]));
	//To allow serial or group learing
	double error = 0;
	for (int counter = 0; counter < countOf; counter++) {
		
		forwardPropagation(this, X[counter], Real, NH1_Output, NH2_Output, NH3_Output);
		double currentError = calcError(Ideal[counter], Real, errorParts);
		error += currentError;
		// For every layer , the delta is calculated from the ouput they generated and the input they received
		//Output layer's delta uses each neurons error as the output the layer generated
		for (int i = 0; i < OutputC; i++) {
			double U = 0;
			for (int k = 0; k < NH3; k++)U += NH3_Output[k] * (this->Output)[i][k];
			U += this->Output[i][NH3];
			delta_Real[i] = logistics_deriv(U) * errorParts[i];
		}

		for (int i = 0; i < NH3; i++) {
			double U = 0;
			for (int k = 0; k < NH2; k++)U += NH2_Output[k] * (this->NH3Layer)[i][k];
			U += this->NH3Layer[i][NH2];

			double nextLayer = 0;
			for (int k = 0; k < OutputC; k++)nextLayer += this->Output[k][i] * delta_Real[k];
			delta_NH3[i] = logistics_deriv(U) * nextLayer;
		}
		

		
		for (int i = 0; i < NH2; i++) {
			double U = 0;
			for (int k = 0; k < NH1; k++)U += NH1_Output[k] * (this->NH2Layer)[i][k];
			U += this->NH2Layer[i][NH1];

			double nextLayer = 0;
			for (int k = 0; k < NH3; k++)nextLayer += this->NH3Layer[k][i] * delta_NH3[k];
			delta_NH2[i] = tahn_deriv(U) * nextLayer;
		}
		

		
		for (int i = 0; i < NH1; i++) {
			double U = 0;
			for (int k = 0; k < InputC; k++)U += X[counter][k] * (this->NH1Layer)[i][k];
			U += this->NH1Layer[i][InputC];

			double nextLayer = 0;
			for (int k = 0; k < NH2; k++)nextLayer += this->NH2Layer[k][i] * delta_NH2[k];
			delta_NH1[i] = tahn_deriv(U) * nextLayer;
		}
		
		//Generating the error derivative matrix for each layer using the layer's Input
		for (int i = 0; i < OutputC; i++) {
			for (int k = 0; k < NH3; k++) derriv_Real[i][k] += delta_Real[i] * NH3_Output[k];
			derriv_Real[i][NH3] += delta_Real[i];
		}
		for (int i = 0; i < NH3; i++) {
			for (int k = 0; k < NH2; k++) derriv_NH3[i][k] += delta_NH3[i] * NH2_Output[k];
			derriv_NH3[i][NH2] += delta_NH3[i];
		}
		for (int i = 0; i < NH2; i++) {
			for (int k = 0; k < NH1; k++) derriv_NH2[i][k] += delta_NH2[i] * NH1_Output[k];
			derriv_NH2[i][NH1] += delta_NH2[i];
		}
		for (int i = 0; i < NH1; i++) {
			for (int k = 0; k < InputC; k++) derriv_NH1[i][k] += delta_NH1[i] * X[counter][k];
			derriv_NH1[i][InputC] += delta_NH1[i];
		}
	}
	// Updating the weights and bias of every neuron , learning rate despends on the average error of the set
	double n = error / (countOf);
	for (int i = 0; i < OutputC; i++) {
		for (int k = 0; k <= NH3; k++)this->Output[i][k] -= n * derriv_Real[i][k];
	}

	for (int i = 0; i < NH3; i++) {
		for (int k = 0; k <= NH2; k++)this->NH3Layer[i][k] -= n * derriv_NH3[i][k];
	}

	for (int i = 0; i < NH2; i++) {
		for (int k = 0; k <= NH1; k++)this->NH2Layer[i][k] -= n * derriv_NH2[i][k];
	}

	for (int i = 0; i < NH1; i++) {
		for (int k = 0; k <= InputC; k++)this->NH1Layer[i][k] -= n * derriv_NH1[i][k];
	}
	free(delta_NH1); free(delta_NH2); free(delta_NH3); free(NH1_Output); free(NH2_Output);
	free(NH3_Output); free(Real); free(errorParts); free(derriv_Real); free(derriv_NH1);
	free(derriv_NH2); free(derriv_NH3); free(delta_Real);

	return error;
}


void PN_Train(PN* this, double (*X)[2], char* data, void (*toArray)(char, double*), int Total) {
	double (*Y)[OutputC] = (double (*)[OutputC])malloc(B * sizeof(double [OutputC]));

	const unsigned int samples = Total / N;
	const unsigned int times = N / B;

	unsigned int epoch = 0;
	double errorPrev = INFINITY;
	double diff;
	double errorGlobal;
	do {
		errorGlobal = 0;
		for (unsigned int count = 0; count < samples; count += 1) {
			for (unsigned int k = 0; k < times; k++) {
				for (int i = 0; i < B; i++) {
					//Converts the team number to an array of doubles to train the network
					toArray(data[(count * N) + (k * B) + i], Y[i]);
				}
				errorGlobal += backprop(this, X +(count * N) + (k * B), Y, B);
			}
		}
		errorGlobal /= Total;
		epoch += 1;
		diff = errorPrev - errorGlobal;
		errorPrev = errorGlobal;
		printf("%i , %lf\n",epoch , errorPrev);
	} while (epoch < 700 || (diff > 0.000001 || diff < -0.000001));
	free(Y);
	return;
}
PN * PN_Create() {
	PN* object = (PN*)malloc(sizeof(PN));
	init(object->NH1Layer, NH1,InputC);
	init(object->NH2Layer, NH2,NH1);
	init(object->NH3Layer, NH3,NH2);
	init(object->Output, OutputC,NH3);
	return object;
}

void PN_Predict(PN* this, double* x , double * y) {
	double * NH1_OUT = (double*)malloc(NH1 * sizeof(double));
	double * NH2_OUT = (double*)malloc(NH2 * sizeof(double));
	double * NH3_OUT = (double*)malloc(NH3 * sizeof(double));
	forwardPropagation(this, x, y, NH1_OUT, NH2_OUT, NH3_OUT);
	free(NH1_OUT); free(NH2_OUT); free(NH3_OUT);
	return;
}


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <math.h>

using namespace std;

void system1Simulation(int A, int B, int k){
	int At = A;
	int Bt = B;
	double time = 0;
	
	vector<vector<double> > funcValues(A);
	for ( int i = 0 ; i < 3 ; i++ ){
		funcValues[i].resize(A);
	}
	funcValues[0].at(0) = At;
	funcValues[1].at(0) = Bt;
	funcValues[2].at(0) = 0;
	for(int i = 1; i<A; i++){
		double p = At*Bt*k;
		At--;
		Bt++;
		time += -(1/p)*(rand()/(double)RAND_MAX);
		funcValues[0].at(i) = At;
		funcValues[1].at(i) = Bt;
		funcValues[2].at(i) = time;
	}
	printf("A                      B                       time");
	printf("\n");
	for(int i = 0; i<A; i++){
		printf("%6.10f                %6.10f                  %6.10f",(double)funcValues[0].at(i), (double)funcValues[1].at(i), (double)funcValues[2].at(i) );
		printf("\n");
	}
};


int main(){
	//system1Simulation(199, 1, 1);
	//system1Simulation(1999, 1, 1);
	//system1Simulation(19999, 1, 1);
	system1Simulation(199999, 1, 1);
	while(true);
	return 0;
}
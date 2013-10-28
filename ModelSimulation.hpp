#ifndef ModelSimulation_hpp_
#define ModelSimulation_hpp_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>

template <class T>
class System1 {

public:
	static const long DefaultA = 199;
	static const long DefaultB = 1;
	static const T DefaultRate = 1;

	struct Data {

		long A;
		long B;
		T t;

		Data() : A(0), B(0), t(0) {}
		Data(long nA, long nB, T nt) : A(nA), B(nB), t(nt) {}
	};

	typedef typename std::vector<System1<T>::Data> DataVector;

private:
	long initialA;
	long initialB;
	T rate;
	DataVector data;

public:
	System1() : initialA(1), initialB(1), rate(1) {}
	System1(long A, long B, T k) : initialA(A), initialB(B), rate(k) {}

	void run() {
		long At = initialA;
		long Bt = initialB;
		T t = 0;

		// Prepare the vector for receiving the values
		data.clear();
		data.resize(initialA);

		// Defining the initial values
		data[0].A = initialA;
		data[0].B = initialB;
		data[0].t = 0;

		// Loop for calculating the further iterations 
		for(int i = 1; i < initialA; i++){
			T P = At * Bt * rate;
			T p = -(1/P);
			At--;
			Bt++;
			t += p * log(rand()/(T)RAND_MAX);
			data.at(i).A = At;
			data.at(i).B = Bt;
			data.at(i).t = t; 
		}
	}

	bool print() {
		printTabulated(std::cout, " ");
	}

	void printTabulated(std::ostream& out, const char* colsep) {

		int w = 10, p = 10;
		out << std::setw(w) << std::setprecision(p) << "A";
		out << std::setw(w) << std::setprecision(p) << colsep;
		out << std::setw(w) << std::setprecision(p) << "B";
		out << std::setw(w) << std::setprecision(p) << colsep;
		out << std::setw(w) << std::setprecision(p) << "time" << std::endl;
		for (typename DataVector::iterator it = data.begin(); it != data.end(); ++it) {
			out << std::setw(w) << std::setprecision(p) << (*it).A << colsep;
			out << std::setw(w) << std::setprecision(p) << (*it).B << colsep;
			out << std::setw(w) << std::setprecision(p) << (*it).t << std::endl;
		}
	}

	bool exportCsv(const char* basename) {

		bool ret = false;
		std::string filename(basename);
		filename += ".csv";
		std::ofstream file;
		file.open(filename.data());
		if (file.is_open()) {
			exportCsv(file);
			file.close();
			ret = true;
		}
		return ret;
	}

	void exportCsv(std::ofstream& file) {
		printTabulated(file, ";");
	}
};

#endif

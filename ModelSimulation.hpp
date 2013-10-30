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
	struct Data {

		long A;
		long B;
		T t;

		Data() : A(0), B(0), t(0) {}
		Data(long nA, long nB, T nt) : A(nA), B(nB), t(nt) {}
	};

	typedef typename std::vector<System1<T>::Data> DataVector;

	static const long DefaultA = 199;
	static const long DefaultB = 1;
	static const T DefaultRate = 1;

private:
	long initialA;
	long initialB;
	T rate;
	DataVector data;

public:
	System1() : initialA(1), initialB(1), rate(1) {}
	System1(long A, long B, T k) : initialA(A), initialB(B), rate(k) {}

	long getInitialA() { return initialA; }
	long getInitialB() { return initialB; }
	T getRate() { return rate; }
	size_t getTotalIterations() { return data.size(); }
	Data& getIteration(size_t iter) { return data.at(iter); }

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
		printTabulated(out, colsep, 10, 10);
	}

	void printTabulated(std::ostream& out, const char* colsep, int w, int p) {

		out << std::setw(w) << std::setprecision(p) << "Total A";
		out << std::setw(w) << std::setprecision(p) << colsep;
		out << std::setw(w) << std::setprecision(p) << "Total B";
		out << std::setw(w) << std::setprecision(p) << colsep;
		out << std::setw(w) << std::setprecision(p) << "Normalized A";
		out << std::setw(w) << std::setprecision(p) << colsep;
		out << std::setw(w) << std::setprecision(p) << "Normalized B";
		out << std::setw(w) << std::setprecision(p) << colsep;
		out << std::setw(w) << std::setprecision(p) << "Time" << std::endl;
		for (typename DataVector::iterator it = data.begin(); it != data.end(); ++it) {
			T normA = (*it).A/(T)((*it).A + (*it).B);
			T normB = (*it).A/(T)((*it).A + (*it).B);
			out << std::setw(w) << std::setprecision(p) << (*it).A << colsep;
			out << std::setw(w) << std::setprecision(p) << (*it).B << colsep;
			out << std::setw(w) << std::setprecision(p) << normA << colsep;
			out << std::setw(w) << std::setprecision(p) << normB << colsep;
			out << std::setw(w) << std::setprecision(p) << (*it).t << std::endl;
		}
	}

	bool exportCsv(const char* basename) {

		bool ret = false;
    	std::stringstream filename;
		filename << basename << "_" << initialA << "_" << initialB << "_" << rate << ".csv";
		std::ofstream file;
		file.open(filename.str().data());
		if (file.is_open()) {
			exportCsv(file);
			file.close();
			ret = true;
		} else {
			std::cout << "ERROR: Couldn't open " << filename << std::endl;
		}
		return ret;
	}

	void exportCsv(std::ofstream& file) {
		printTabulated(file, ";", 0, 10);
	}

};


template <class T>
class System2 {

public:
	struct Initials {

		long A, B;
		T da, db;
		T ka, kb;
		T Ka, Kb;
		T h, l;

		Initials() : A(0), B(0), da(0), db(0), ka(0), kb(0), Ka(0), Kb(0), h(0), l(0) {}
		Initials(long vA, long vB, T vda, T vdb, T vka, T vkb, T vKa, T vKb, T vh, T vl) :
			A(vA), B(vB), da(vda), db(vdb), ka(vka), kb(vkb), Ka(vKa), Kb(vKb), h(vh), l(vl) {}
	};

	struct Data {

		long A, B;
		T nA, nB;
		T t;

		Data() : A(0), B(0), nA(0), nB(0) {};
		Data(long vA, long vB) {
			set(vA, vB);
		}

		void set(long vA, long vB, T vt) {
			T sum = vA + vB;
			A = vA; B = vB; t = vt;
			nA = 0; nB = 0;
			if (sum != 0) {
				nA = A / sum;
				nB = B / sum;
			}
		}
	};

	typedef std::vector<Data> DataVector;

	static const size_t DefaultMaxIterations = 1000;
	static const long Default_A = 100;
	static const long Default_B = 10;
	static const T Default_da = 1;
	static const T Default_db = 1;
	static const T Default_ka = 1;
	static const T Default_kb = 1;
	static const T Default_Ka = 1;
	static const T Default_Kb = 1;
	static const T Default_h = 1;
	static const T Default_l = 1;

private:
	Initials initials;
	DataVector data;
	size_t maxIterations;

public:
	System2() : maxIterations(DefaultMaxIterations) {}
	System2(const Initials& vInitials) : initials(vInitials), maxIterations(DefaultMaxIterations) {}
	System2(long vA, long vB, T vda, T vdb, T vka, T vkb, T vKa, T vKb, T vh, T vl) : maxIterations(DefaultMaxIterations) {
		initials.A = vA;
		initials.B = vB;
		initials.da = vda;
		initials.db = vdb;
		initials.ka = vka;
		initials.kb = vkb;
		initials.Ka = vKa;
		initials.Kb = vKb;
		initials.h = vh;
		initials.l = vl;
	}

	Initials& getInitials() { return initials; }
	size_t getTotalIterations() { return data.size(); }
	size_t getMaxIterations() { return maxIterations; }
	Data& getIteration(size_t iter) { return data.at(iter); }


	void run() {
		long At = initials.A;
		long Bt = initials.B;
		Data current;
		T t = 0;
		size_t counterMax = maxIterations;
		size_t counter = 0;

		T da = initials.da;
		T db = initials.db;
		T ka = initials.ka;
		T kb = initials.kb;
		T Ka = initials.Ka;
		T Kb = initials.Kb;
		T h = initials.h;
		T l = initials.l;

		// create the matrix that will hold all the graph values
		data.clear();

		// Push initial values
		current.set(At, Bt, t);
		data.push_back(current);

		// calculate all rates and their normalization
		while (counter != counterMax && At != 0 && Bt != 0) {
			T rate1 = (T)ka * (pow(At,h)*Kb) / ((Ka+pow(At, h)) * (Kb+pow(Bt,l)));
			T rate2 = Bt * db;
			T rate3 = (T)kb * (pow(Bt,l)*Ka) / ((Ka+pow(At, h)) * (Kb+pow(Bt,l)));
			T rate4 = At * da;
			T rateSum = rate1 + rate2 + rate3 + rate4;
			T rate1Norm = rate1/rateSum;
			T rate2Norm = rate2/rateSum;
			T rate3Norm = rate3/rateSum;
			T rate4Norm = rate4/rateSum;

			// Monte Carlo algorithm to choose what will be the next reaction
			int reaction = 0;

			T randonN = rand()/(T)RAND_MAX;
			if(0 <= randonN && randonN < rate1Norm) {
				reaction = 1;
			}
			if(rate1Norm <= randonN && randonN < (rate1Norm + rate2Norm)) {
				reaction = 2;
			}
			if((rate1Norm + rate2Norm) <= randonN && randonN < (rate1Norm + rate2Norm + rate3Norm)) {
				reaction = 3;
			}
			if((rate1Norm + rate2Norm + rate3Norm) <= randonN && randonN < (rate1Norm + rate2Norm + rate3Norm + rate4Norm)) {
				reaction = 4;
			}

			// Process the reaction

			if(reaction == 1){
				Bt++;

			}else if(reaction == 2){
				if(Bt > 0){
					Bt--;
				}

			}else if(reaction == 3){
				At++;

			}else if(reaction == 4){
				if(At > 0){
					At--;
				}
			}

			// Update the matrix with values normalized
			t += -(1 / rateSum) * log(rand() / (T) RAND_MAX);
			current.set(At, Bt, t);
			data.push_back(current);
			counter++;
		}
	}

	bool print() {
		printTabulated(std::cout, " ");
	}

	void printTabulated(std::ostream& out, const char* colsep) {
		printTabulated(out, colsep, 10, 10);
	}

	void printTabulated(std::ostream& out, const char* colsep, int w, int p) {

		out << std::setw(w) << std::setprecision(p) << "Total A";
		out << std::setw(w) << std::setprecision(p) << colsep;
		out << std::setw(w) << std::setprecision(p) << "Total B";
		out << std::setw(w) << std::setprecision(p) << colsep;
		out << std::setw(w) << std::setprecision(p) << "Normalized A";
		out << std::setw(w) << std::setprecision(p) << colsep;
		out << std::setw(w) << std::setprecision(p) << "Normalized B";
		out << std::setw(w) << std::setprecision(p) << colsep;
		out << std::setw(w) << std::setprecision(p) << "Time" << std::endl;
		for (typename DataVector::iterator it = data.begin(); it != data.end(); ++it) {
			out << std::setw(w) << std::setprecision(p) << (*it).A << colsep;
			out << std::setw(w) << std::setprecision(p) << (*it).B << colsep;
			out << std::setw(w) << std::setprecision(p) << (*it).nA << colsep;
			out << std::setw(w) << std::setprecision(p) << (*it).nB << colsep;
			out << std::setw(w) << std::setprecision(p) << (*it).t << std::endl;
		}
	}

	bool exportCsv(const char* basename) {

		bool ret = false;
    	std::stringstream filename;
		filename << basename << "_" << initials.A << "_" << initials.B << ".csv";
		std::ofstream file;
		file.open(filename.str().data());
		if (file.is_open()) {
			exportCsv(file);
			file.close();
			ret = true;
		} else {
			std::cout << "ERROR: Couldn't open " << filename << std::endl;
		}
		return ret;
	}

	void exportCsv(std::ofstream& file) {
		printTabulated(file, ";", 0, 10);
	}

};

template <class T>
class System3 {

public:
	struct Initials {

		long A, B, C;
		T kt, ka, kb;
		T dc;

		Initials() : A(0), B(0), C(0), kt(0), ka(0), kb(0), dc(0) {}
		Initials(long vA, long vB, long vC, T vkt, T vka, T vkb, T vdc) :
			A(vA), B(vB), C(vC), kt(vkt), ka(vka), kb(vkb), dc(vdc) {}
	};

	struct Data {

		long A, B, C;
		T nA, nB, nC;
		T t;

		Data() : A(0), B(0), C(0), nA(0), nB(0), nC(0) {};
		Data(long vA, long vB, long vC, T vt) {
			set(vA, vB, vC, vt);
		}

		void set(long vA, long vB, long vC, T vt) {
			T sum = vA + vB + vC;
			A = vA; B = vB; C = vC; t = vt;
			nA = 0; nB = 0; nC = 0;
			if (sum != 0) {
				nA = A / sum;
				nB = B / sum;
				nC = C / sum;
			}
		}
	};

	typedef std::vector<Data> DataVector;

	static const size_t DefaultMaxIterations = 1000;
	static const long Default_A = 10;
	static const long Default_B = 10;
	static const long Default_C = 0;
	static const T Default_kt = 10;
	static const T Default_ka = 10;
	static const T Default_kb = 10;
	static const T Default_dc = 1;

private:
	Initials initials;
	DataVector data;
	size_t maxIterations;

public:
	System3() : maxIterations(DefaultMaxIterations) {}
	System3(const Initials& vInitials) : initials(vInitials), maxIterations(DefaultMaxIterations) {}
	System3(long vA, long vB, long vC, T vkt, T vka, T vkb, T vdc) : maxIterations(DefaultMaxIterations) {
		initials.A = vA;
		initials.B = vB;
		initials.C = vC;
		initials.kt = vkt;
		initials.ka = vka;
		initials.kb = vkb;
		initials.dc = vdc;
	}

	Initials& getInitials() { return initials; }
	size_t getTotalIterations() { return data.size(); }
	size_t getMaxIterations() { return maxIterations; }
	Data& getIteration(size_t iter) { return data.at(iter); }


	void run() {
		long At = initials.A;
		long Bt = initials.B;
		long Ct = initials.C;
		Data current;
		T t = 0;
		size_t counterMax = maxIterations;
		size_t counter = 0;

		T kt = initials.kt;
		T ka = initials.ka;
		T kb = initials.kb;
		T dc = initials.dc;

		// create the matrix that will hold all the graph values
		data.clear();

		// Push initial values
		current.set(At, Bt, Ct, t);
		data.push_back(current);

		// Calculate all rates and their normalization
		while(counter != counterMax) {
			T rate1 = At * Bt * kt;
			T rate2 = At * Ct * ka;
			T rate3 = Bt * Ct * kb;
			T rate4 = Ct * dc;
			T rate5 = At * dc;
			T rate6 = Bt * dc;
			T rateSum = rate1 + rate2 + rate3 + rate4 + rate5 + rate6;
			T rate1Norm = rate1 / rateSum;
			T rate2Norm = rate2 / rateSum;
			T rate3Norm = rate3 / rateSum;
			T rate4Norm = rate4 / rateSum;
			T rate5Norm = rate5 / rateSum;
			T rate6Norm = rate6 / rateSum;

			// Monte Carlo algorithm to choose what will be the next reaction
			int reaction = 0;
			T randonN = rand()/(T)RAND_MAX;

			if(0 <= randonN && randonN < rate1Norm){
				reaction = 1;
			}
			if(rate1Norm <= randonN && randonN < (rate1Norm + rate2Norm)) {
				reaction = 2;
			}
			if((rate1Norm + rate2Norm) <= randonN && randonN < (rate1Norm + rate2Norm + rate3Norm)) {
				reaction = 3;
			}
			if((rate1Norm + rate2Norm + rate3Norm) <= randonN && randonN < (rate1Norm + rate2Norm + rate3Norm + rate4Norm)) {
				reaction = 4;
			}
			if((rate1Norm + rate2Norm + rate3Norm + rate4Norm) <= randonN && randonN < (rate1Norm + rate2Norm + rate3Norm + rate4Norm + rate5Norm)) {
				reaction = 5;
			}
			if((rate1Norm + rate2Norm + rate3Norm + rate4Norm + rate5Norm) <= randonN && randonN < (rate1Norm + rate2Norm + rate3Norm + rate4Norm + rate5Norm + rate6Norm)) {
				reaction = 6;
			}

			// Process the reaction
			if(reaction == 1){
				At--;
				Bt--;
				Ct++;

			}else if(reaction == 2){
				At--;
				Ct--;

			}else if(reaction == 3){
				Bt--;
				Ct--;

			}else if(reaction == 4){
				Ct--;

			}else if(reaction == 5){
				At--;

			}else if(reaction == 6){
				Bt--;
			}

			// Update the matrix with values normalized
			t += -(1 / rateSum) * log(rand() / (T)RAND_MAX);
			current.set(At, Bt, Ct, t);
			data.push_back(current);
			counter++;
			if (At == 0 && Bt == 0 && Ct == 0) break;
		}	
	}

	bool print() {
		printTabulated(std::cout, " ");
	}

	void printTabulated(std::ostream& out, const char* colsep) {
		printTabulated(out, colsep, 10, 10);
	}

	void printTabulated(std::ostream& out, const char* colsep, int w, int p) {

		out << std::setw(w) << std::setprecision(p) << "Total A";
		out << std::setw(w) << std::setprecision(p) << colsep;
		out << std::setw(w) << std::setprecision(p) << "Total B";
		out << std::setw(w) << std::setprecision(p) << colsep;
		out << std::setw(w) << std::setprecision(p) << "Total C";
		out << std::setw(w) << std::setprecision(p) << colsep;
		out << std::setw(w) << std::setprecision(p) << "Normalized A";
		out << std::setw(w) << std::setprecision(p) << colsep;
		out << std::setw(w) << std::setprecision(p) << "Normalized B";
		out << std::setw(w) << std::setprecision(p) << colsep;
		out << std::setw(w) << std::setprecision(p) << "Normalized C";
		out << std::setw(w) << std::setprecision(p) << colsep;
		out << std::setw(w) << std::setprecision(p) << "Time" << std::endl;
		for (typename DataVector::iterator it = data.begin(); it != data.end(); ++it) {
			out << std::setw(w) << std::setprecision(p) << (*it).A << colsep;
			out << std::setw(w) << std::setprecision(p) << (*it).B << colsep;
			out << std::setw(w) << std::setprecision(p) << (*it).C << colsep;
			out << std::setw(w) << std::setprecision(p) << (*it).nA << colsep;
			out << std::setw(w) << std::setprecision(p) << (*it).nB << colsep;
			out << std::setw(w) << std::setprecision(p) << (*it).nC << colsep;
			out << std::setw(w) << std::setprecision(p) << (*it).t << std::endl;
		}
	}

	bool exportCsv(const char* basename) {

		bool ret = false;
    	std::stringstream filename;
		filename << basename << "_" << initials.A << "_" << initials.B << "_" << initials.C << ".csv";
		std::ofstream file;
		file.open(filename.str().data());
		if (file.is_open()) {
			exportCsv(file);
			file.close();
			ret = true;
		} else {
			std::cout << "ERROR: Couldn't open " << filename << std::endl;
		}
		return ret;
	}

	void exportCsv(std::ofstream& file) {
		printTabulated(file, ";", 0, 10);
	}

};


#endif

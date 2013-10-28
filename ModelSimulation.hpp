#ifndef ModelSimulation_hpp_
#define ModelSimulation_hpp_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <mgl2/mgl.h>

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
		printTabulated(file, ";");
	}

/*
	bool exportGraphs(const char* basename) {
		mglData y;
		mgls_prepare1d(&y);
		gr->SetOrigin(0,0,0);

		gr->SubPlot(1,2,0,"");
		gr->Title("Normalized A/B Qty.");
		gr->Box();
		gr->Plot(y);

		gr->SubPlot(1,2,0,"");
		gr->Title("Normalized A/B Qty.");
		gr->Box();
		gr->Plot(y);

		mglData yc(30), xc(30), z(30);
		z.Modify("2*x-1");
		yc.Modify("sin(pi*(2*x-1))"); xc.Modify("cos(pi*2*x-pi)");
		gr->Plot(xc,yc,z,"rs");
	}
*/

	class Group {

	private:
		std::vector<System1<T> > systems;

	public:
		Group() {};

		void add(System1<T> sys) {
			systems.push_back(sys);
		}

		void clear() {
			systems.clear();
		}

		void exportGraphs(const char* basename) {

  			mglGraph gr(0, 2000, 800);
			size_t maxIterations = 0;
			size_t maxSystems = systems.size();
			mglData abTotals;
			std::string filename(basename);
			filename += ".png";

			for (typename std::vector<System1<T> >::iterator it = systems.begin(); it != systems.end(); ++it) {
				size_t iterations = (*it).getTotalIterations();
				if (iterations > maxIterations)
					maxIterations = iterations;
			}

			std::cout << "** Max Iterations: " << maxIterations << std::endl;
			std::cout << "** Max Systems: " << maxSystems << std::endl;
			abTotals.Create(maxIterations, maxSystems);
			std::cout << "** Initializing data..." << std::endl;
			for (size_t iter = 0; iter < maxIterations; iter++) {
				for (size_t sysid = 0; sysid < maxSystems; sysid++) {
					System1<T> sys = systems.at(sysid);

					if (iter < sys.getTotalIterations()) {
						System1<T>::Data data = sys.getIteration(iter);
						T val = (data.B) / (T)(data.A + data.B);
						//std::cout << "*** [" << iter << ", " << sysid << "] = " << val << std::endl;
						abTotals.Put(val, iter, sysid);

					} else {
						//std::cout << "*** [" << iter << ", " << sysid << "] (x) = " << 0 << std::endl;
						abTotals.Put(0, iter, sysid);
					}
				}
			}
			
			std::cout << "** Starting the drawing process..." << std::endl;
  			gr.Alpha(true);
  			gr.Light(true);
  			//gr.SetCoor(12); // Log-X
  			gr.SetTickTempl('x', "%.5g");
  			gr.SetTickTempl('y', "%.5g");
			gr.SetOrigin(0,0,0);
			gr.SetRanges(0, maxIterations, 0, 1);
			gr.Adjust("xy");
			gr.Title("Normalized A/B Totals");
			gr.Box();
			gr.Plot(abTotals);
			gr.WriteFrame(filename.data());

			/*
			gr->SubPlot(1,2,0,"");
			gr->Title("Normalized B per Initial Cond.");
			gr->Box();
			gr->Plot(abTotals);
			*/

		}
	};
};

#endif

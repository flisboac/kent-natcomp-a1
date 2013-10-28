#include <iostream>
#include <sstream>
#include <string>
#include "ModelSimulation.hpp"

#define PREC double
#define ATOX std::atof

static void print_help(const char* progname);

int main(int argc, char** argv){
	int systemId, ret = 0;

	if (argc == 1) {
		print_help(argv[1]);
		return 1;
	}

	systemId = atoi(argv[1]);

	switch(systemId) {
	case 1: {
		long A = System1<PREC>::DefaultA;
		long B = System1<PREC>::DefaultB;
		PREC k = System1<PREC>::DefaultRate;

    	std::stringstream basename;

		if (argc > 2) A = atol(argv[2]);
		if (argc > 3) B = atol(argv[3]);
		if (argc > 4) k = ATOX(argv[4]);

		if (argc > 6)
			basename << argv[5];
		else
			basename << "System1_" << A << "_" << B << "_" << k;

		std::cout << "System: 1" << std::endl;
		std::cout << "Initial Population of A: " << A << std::endl;
		std::cout << "Initial Population of B: " << B << std::endl;
		std::cout << "Rate: " << k << std::endl;
		std::cout << "Output file basename: " << basename.str().data() << std::endl;

		System1<PREC> sys(A, B, k);

		std::cout << "* Starting simulation..." << std::endl;
		sys.run();

		std::cout << "* Simulation finished. Exporting data..." << std::endl;
		sys.exportCsv(basename.str().data());

		std::cout << "* Finished." << std::endl;
		//sys.print();
		break;
	}
	case 2: {
		std::cout << "(Not yet implemented)" << std::endl;
		break;
	}
	default:
		print_help(argv[1]);
		break;
	}

	//system1Simulation(199, 1, 1);
	//system1Simulation(1999, 1, 1);
	//system1Simulation(19999, 1, 1);
	//system1Simulation(199999, 1, 10);
	return ret;
}

void print_help(const char* progname) {
	if (!progname) progname = "./ModelSimulation";
	std::cout << "ModelSimulation 0.1" << std::endl;
	std::cout << "Assignment 1 for COXXX Natural Computation class" << std::endl;
	std::cout << "University of Kent at Canterbury" << std::endl;
	std::cout << "Authors: Flavio Lisboa, Tomas Almeida" << std::endl;
	std::cout << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << "$ " << progname << " <system> [options]" << std::endl;
	std::cout << std::endl;
	std::cout << "Please select a system to simulate:" << std::endl;
	std::cout << "$ " << progname << " 1 [A] [B] [k] [basename]" << std::endl;
	std::cout << "\tSimulates the System 1, with initial population of 'A' and 'B'," << std::endl;
	std::cout << "\tin cells, and rate 'k'. Writes CSV data to '<basename>.csv' and" << std::endl;
	std::cout << "\tgraphics to '<basename>.png'." << std::endl;
	std::cout << "$ " << progname << " 2 [A] [B] [k] [basename]" << std::endl;
	std::cout << "\t(TBD)" << std::endl;
}
#include <iostream>
#include <sstream>
#include <string>
#include "ModelSimulation.hpp"

#define PREC double
#define ATOX std::atof

struct System1Run {

	long A, B;
	PREC k;
};


static const System1Run System1_Runs[] = {
	{ 199, 1, 1 },
	{ 1999, 1, 1 },
	{ 19999, 1, 1 },
	{ 199999, 1, 1 }
};

typedef System2<PREC>::Initials System2Run;
static const System2Run System2_Runs[] = {
	{ 2000, 200, /* d */ 1, 1, /* k */ 1, 1, /* K */ 1, 1, /* h/l */ 1, 1 },
	{ 2000, 2000, /* d */ 0.1, 0.1, /* k */ 100, 100, /* K */ 10, 10, /* h/l */ 0.5, -0.5 },
	{ 5000, 5000, /* d */ 10, 0.1, /* k */ 0.1, 100, /* K */ 10, 10, /* h/l */ 0.5, -0.5 }
};

static void print_help(const char* progname);
static void run_system1(System1<PREC>& sys, const char* basename, bool exportData);
static void run_system2(System2<PREC>& sys, const char* basename, bool exportData);
static void run_system3(System3<PREC>& sys, const char* basename, bool exportData);

int main(int argc, char** argv){
	int systemId, ret = 0;

	if (argc == 1) {
		print_help(argv[1]);
		return 1;
	}

	systemId = atoi(argv[1]);

	switch(systemId) {
	case 1: {
		bool specific = false;
		long A = System1<PREC>::DefaultA;
		long B = System1<PREC>::DefaultB;
		PREC k = System1<PREC>::DefaultRate;

    	const char* basename;

		if (argc > 2) { specific = true; A = atol(argv[2]); };
		if (argc > 3) { specific = true; B = atol(argv[3]); };
		if (argc > 4) { specific = true; k = ATOX(argv[4]); };

		if (argc > 5) {
			specific = true;
			basename = argv[5];

		} else {
			basename = "System1";
		}

		if (specific) {
			System1<PREC> sys(A, B, k);
			run_system1(sys, basename, true);

		} else {
			std::vector<System1<PREC> > systems;

			for (size_t i = 0; i < sizeof(System1_Runs) / sizeof(System1Run); i++) {
				System1<PREC> sys(System1_Runs[i].A, System1_Runs[i].B, System1_Runs[i].k);
				run_system1(sys, basename, true);
			}

			std::cout << "* Finished batch of simulations." << std::endl;
		}
		break;
	}
	case 2: {
		bool specific = false;
		long A  = System2<PREC>::Default_A;  // Initial A
		long B  = System2<PREC>::Default_B;  // Initial B
		PREC da = System2<PREC>::Default_da; // Decay Reaction Constant of A (da)
		PREC db = System2<PREC>::Default_db; // Decay Reaction Constant of B (db)
		PREC ka = System2<PREC>::Default_ka; // Level Constant of A (ka)
		PREC kb = System2<PREC>::Default_kb; // Level Constant of B (kb)
		PREC Ka = System2<PREC>::Default_Ka; // Value Ka
		PREC Kb = System2<PREC>::Default_Kb; // Value Kb
		PREC h  = System2<PREC>::Default_h;  // Value h
		PREC l  = System2<PREC>::Default_l;  // Value l

    	const char* basename;

		if (argc >  2) { specific = true; A  = atol(argv[ 2]); };
		if (argc >  3) { specific = true; B  = atol(argv[ 3]); };
		if (argc >  4) { specific = true; da = ATOX(argv[ 4]); };
		if (argc >  5) { specific = true; db = ATOX(argv[ 5]); };
		if (argc >  6) { specific = true; ka = ATOX(argv[ 6]); };
		if (argc >  7) { specific = true; kb = ATOX(argv[ 7]); };
		if (argc >  8) { specific = true; Ka = ATOX(argv[ 8]); };
		if (argc >  9) { specific = true; Kb = ATOX(argv[ 9]); };
		if (argc > 10) { specific = true; h  = ATOX(argv[10]); };
		if (argc > 11) { specific = true; l  = ATOX(argv[11]); };

		if (argc > 12) {
			specific = true;
			basename = argv[12];

		} else {
			basename = "System2";
		}

		if (specific) {
			System2<PREC> sys(A, B, da, db, ka, kb, Ka, Kb, h, l);
			run_system2(sys, basename, true);

		} else {
			std::vector<System2<PREC> > systems;

			for (size_t i = 0; i < sizeof(System2_Runs) / sizeof(System2<PREC>::Initials); i++) {
				System2<PREC> sys(System2_Runs[i]);
				run_system2(sys, basename, true);
			}

			std::cout << "* Finished batch of simulations." << std::endl;
		}
		break;
	}
	case 3: {
		bool specific = false;
		long A  = System3<PREC>::Default_A;  // Initial A
		long B  = System3<PREC>::Default_B;  // Initial B
		long C  = System3<PREC>::Default_C; 
		PREC kt = System3<PREC>::Default_kt; 
		PREC ka = System3<PREC>::Default_ka; 
		PREC kb = System3<PREC>::Default_kb; 
		PREC dc = System3<PREC>::Default_dc; 

    	const char* basename;

		if (argc >  2) { specific = true; A  = atol(argv[ 2]); };
		if (argc >  3) { specific = true; B  = atol(argv[ 3]); };
		if (argc >  4) { specific = true; C  = atol(argv[ 4]); };
		if (argc >  5) { specific = true; kt = ATOX(argv[ 5]); };
		if (argc >  6) { specific = true; ka = ATOX(argv[ 6]); };
		if (argc >  7) { specific = true; kb = ATOX(argv[ 7]); };
		if (argc >  8) { specific = true; dc = ATOX(argv[ 8]); };

		if (argc > 9) {
			specific = true;
			basename = argv[9];

		} else {
			basename = "System3";
		}

		System3<PREC> sys(A, B, C, kt, ka, kb, dc);
		run_system3(sys, basename, true);
		break;
	}
	default:
		print_help(argv[1]);
		break;
	}

	return ret;
}

void run_system1(System1<PREC>& sys, const char* basename, bool exportData) {
	std::cout << "System: 1" << std::endl;
	std::cout << "Initial Population of A: " << sys.getInitialA() << std::endl;
	std::cout << "Initial Population of B: " << sys.getInitialB() << std::endl;
	std::cout << "Rate: " << sys.getRate() << std::endl;
	std::cout << "Output file basename: " << basename << std::endl;

	std::cout << "* Starting simulation..." << std::endl;
	sys.run();

	if (exportData) {
		std::cout << "* Finished. Exporting data..." << std::endl;
		sys.exportCsv(basename);
	}

	std::cout << "* Finished." << std::endl;
}

void run_system2(System2<PREC>& sys, const char* basename, bool exportData) {
	std::cout << "System: 2" << std::endl;
	std::cout << "Initial Population of A: "           << sys.getInitials().A  << std::endl;
	std::cout << "Initial Population of B: "           << sys.getInitials().B  << std::endl;
	std::cout << "Decay Reaction Constant of A (da): " << sys.getInitials().da << std::endl;
	std::cout << "Decay Reaction Constant of B (db): " << sys.getInitials().db << std::endl;
	std::cout << "Level Constant of A (ka): "          << sys.getInitials().ka << std::endl;
	std::cout << "Level Constant of B (kb): "          << sys.getInitials().kb << std::endl;
	std::cout << "Value Ka: "                          << sys.getInitials().Ka << std::endl;
	std::cout << "Value Kb: "                          << sys.getInitials().Kb << std::endl;
	std::cout << "Value h: "                           << sys.getInitials().h  << std::endl;
	std::cout << "Value l: "                           << sys.getInitials().l  << std::endl;
	std::cout << "Output file basename: " << basename << std::endl;

	std::cout << "* Starting simulation..." << std::endl;
	sys.run();

	if (exportData) {
		std::cout << "* Finished. Exporting data..." << std::endl;
		sys.exportCsv(basename);
	}

	std::cout << "* Finished." << std::endl;
}

void run_system3(System3<PREC>& sys, const char* basename, bool exportData) {
	std::cout << "System: 2" << std::endl;
	std::cout << "Initial Population of A                : " << sys.getInitials().A  << std::endl;
	std::cout << "Initial Population of B                : " << sys.getInitials().B  << std::endl;
	std::cout << "Initial Population of C                : " << sys.getInitials().C  << std::endl;
	std::cout << "Rate of Positive Reaction (kt, A & B)  : " << sys.getInitials().kt << std::endl;
	std::cout << "Rate of Negative Reaction (ka, A & !B) : " << sys.getInitials().ka << std::endl;
	std::cout << "Rate of Negative Reaction (kb, !A & B) : " << sys.getInitials().kb << std::endl;
	std::cout << "Decay Rate of C (dc, !A & !B)          : " << sys.getInitials().dc << std::endl;
	std::cout << "Output file basename: " << basename << std::endl;

	std::cout << "* Starting simulation..." << std::endl;
	sys.run();

	if (exportData) {
		std::cout << "* Finished. Exporting data..." << std::endl;
		sys.exportCsv(basename);
	}

	std::cout << "* Finished." << std::endl;
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

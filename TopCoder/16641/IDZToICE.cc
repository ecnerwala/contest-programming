#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class IDZToICE {

public:

	vector <string> transform(vector <string> program) {
		assert('J' - 'A' == 9);
		int N = int(program.size());
		// we will just double the instruction count unconditionally
		vector<string> next_program; next_program.reserve(2*N);
		for (int i = 0; i < N; i++) {
			istringstream strm(program[i]);
			string op; strm >> op;
			if (op == "INC") {
				char reg; strm >> reg;
				next_program.push_back(string("INC") + " " + reg);
				next_program.push_back("CLEAR Z");
			} else if (op == "DEC") {
				char reg; strm >> reg;
				next_program.push_back(string("EQUAL") + " " + reg + " " + char(reg + 10) + " " + to_string(2 * i + 2) + " " + to_string(2 * i + 1));
				next_program.push_back(string("INC") + " " + char(reg + 10));
			} else if (op == "ZERO") {
				char reg; strm >> reg;
				int y, z; strm >> y >> z;
				next_program.push_back(string("EQUAL") + " " + reg + " " + char(reg + 10) + " " + to_string(2 * y) + " " + to_string(2 * z));
				next_program.push_back("CLEAR Z");
			} else assert(false);
		}

		assert(int(next_program.size()) == 2 * N);

		next_program.push_back(string("EQUAL") + " " + 'C' + " " + char('C' + 10) + " " + to_string(2 * N + 4) + " " + to_string(2 * N + 1));
		next_program.push_back(string("INC") + " " + 'Z');
		next_program.push_back(string("INC") + " " + char('C' + 10));
		next_program.push_back(string("EQUAL") + " " + 'C' + " " + char('C' + 10) + " " + to_string(2 * N + 4) + " " + to_string(2 * N + 1));
		assert(int(next_program.size()) == 2 * N + 4);
		return next_program;
	}
};

void my_tests() {
	// FILL IN TESTS
	cerr << 'J' - 'A' << '\n';
}

}  // namespace ecnerwala

using ecnerwala::IDZToICE;

// vim:ft=cpp

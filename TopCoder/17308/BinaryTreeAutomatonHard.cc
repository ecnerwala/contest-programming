#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

const string BITS = "abcdefghijkmnoqs";
char n(char a) {
	return a ^ ('A' ^ 'a');
}
// tuvwxyz
// y is vis left
// z is vis right

class BinaryTreeAutomatonHard {

public:

	vector <string> construct(int N) {
		vector<string> res;
		res.push_back("makedepth");

		auto add_op = [&](string state, string conds, string nstate, string toggle, string move) {
			res.push_back(state+":"+conds+":"+nstate+":"+toggle+":"+move);
		};

		add_op("makedepth", "LRP", "EXIT", "x", "p");
		add_op("makedepth", "LR", "popdepth", "", "p");

		// increment the current guy
		assert(int(BITS.size()) == 16);
		for (int i = 0; i < 16; i++) {
			add_op("makedepth", BITS.substr(0, i) + n(BITS[i]), "pushdepth", BITS.substr(0, i+1), "");
		}

		add_op("pushdepth", "", "pushdepth"+to_string(0), "", "");
		for (int i = 0; i < 16; i++) {
			add_op("pushdepth"+to_string(i), string() + BITS[i], "pushdepthl"+to_string(i), string() + BITS[i], "");
			add_op("pushdepth"+to_string(i), string() + n(BITS[i]), "pushdepth"+to_string(i+1), "", "");
			add_op("pushdepthl"+to_string(i), "l", "setdepthl"+to_string(i), "", "l");
			add_op("setdepthl"+to_string(i), "", "pushdepthr"+to_string(i), string() + BITS[i], "p");
			add_op("pushdepthl"+to_string(i), "L", "pushdepthr"+to_string(i), "", "");

			add_op("pushdepthr"+to_string(i), "r", "setdepthr"+to_string(i), "", "r");
			add_op("setdepthr"+to_string(i), "", "pushdepth"+to_string(i+1), string() + BITS[i], "p");
			add_op("pushdepthr"+to_string(i), "R", "pushdepth"+to_string(i+1), "", "");
		}

		add_op("pushdepthr"+to_string(16), "", "try_push_l", "y", "l");

		add_op("try_push_l", "l", "makedepth", "y", "l");
		add_op("popdepth", "y", "try_push_r", "y", "");
		add_op("try_push_l", "L", "try_push_r", "", "");
		add_op("try_push_r", "r", "makedepth", "z", "r");
		add_op("popdepth", "z", "check_depths", "z", "");
		add_op("try_push_r", "R", "check_depths", "", "");

		add_op("checkdepthl", "", "checkdepthl"+to_string(16), "", "");
		for (int i = 0; i < 16; i++) {
			add_op("checkdepthl"+to_string(i+1), string() + BITS[i], "checkdepthltrue"+to_string(i+1), "", "l");
			add_op("checkdepthl"+to_string(i+1), string() + n(BITS[i]), "checkdepthlfalse"+to_string(i+1), "", "l");
			add_op("checkdepthltrue"+to_string(i+1), string() + BITS[i], "checkdepthl"+to_string(i), "", "p");
			add_op("checkdepthlfalse"+to_string(i+1), string() + n(BITS[i]), "checkdepthl"+to_string(i), "", "p");

			add_op("checkdepthltrue"+to_string(i+1), string() + n(BITS[i]), "pushdepthr", "", "p");
			add_op("checkdepthlfalse"+to_string(i+1), string() + BITS[i], "pulldepthl", "", "p");
		}
		add_op("checkdepthr", "", "checkdepthr"+to_string(16), "", "");
		for (int i = 0; i < 16; i++) {
			add_op("checkdepthr"+to_string(i+1), string() + BITS[i], "checkdepthrtrue"+to_string(i+1), "", "r");
			add_op("checkdepthr"+to_string(i+1), string() + n(BITS[i]), "checkdepthrfalse"+to_string(i+1), "", "r");
			add_op("checkdepthrtrue"+to_string(i+1), string() + BITS[i], "checkdepthr"+to_string(i), "", "p");
			add_op("checkdepthrfalse"+to_string(i+1), string() + n(BITS[i]), "checkdepthr"+to_string(i), "", "p");

			add_op("checkdepthrtrue"+to_string(i+1), string() + n(BITS[i]), "donecheckr", "", "p");
			add_op("checkdepthrfalse"+to_string(i+1), string() + BITS[i], "pulldepthr", "", "p");
		}
		add_op("donecheckr", "", "popdepth", "", "p");

		add_op("pulldepthl", "", "pulldepthl"+to_string(0), "", "l");
		for (int i = 0; i < 16; i++) {
			add_op("pulldepthl"+to_string(i), string() + BITS[i], "getdepthltrue"+to_string(i), "", "p");
			add_op("pulldepthl"+to_string(i), string() + n(BITS[i]), "getdepthlfalse"+to_string(i), "", "p");
			add_op("getdepthl"+to_string(i), string() + BITS[i], "getdepthltrue"+to_string(i), "", "p");
		}
		add_op("pushdepthl"+to_string(16), "", "makedepth", "y", "l");

		return res;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::BinaryTreeAutomatonHard;

// vim:ft=cpp

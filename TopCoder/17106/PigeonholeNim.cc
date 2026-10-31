#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class PigeonholeNim {

public:

	int countWinningMoves(int R, int C, vector <int> piles, int emptyWins) {
		const int V = 220;
		vector<char> isValid(V+1);
		for (int v = 0; v <= V; v++) {
			int a = v % 10;
			int b = v / 10 % 10;
			int c = v / 10 / 10 % 10;
			isValid[v] = !(a == 4 || b == 4 || c == 4 || a == 7 || b == 7 || c == 7);
		}

		const int AUTOLOSE = V+1;

		for (int c = 0; c < C; c++) {
			array<int, 2> top_nimbers{-1, -1};
			auto run_transition = [&](int P) -> void {
				assert(P > 1);
				vector<int> nimber(P+1);
				nimber[0] = top_nimbers[0];
				for (int i = 1; i <= P; i++) {
					map<int, int> ex;
					for (int v = 1; v <= i; v++) {
						if (!isValid[v]) continue;
						ex[nimber[i-v]]++;
						if (v == i) ex[top_nimbers[1]]++;
					}
					nimber[i] = 0;
					auto it = ex.begin();
					while (it != ex.end() && *it->first == nimber[i]) {
						++it;
						nimber[i]++;
					}
				}
				top_nimbers = {nimber[P], nimber[P-1]};
			};
			vector<int> cur; cur.reserve(R);
			for (int r = R-1; r >= 0; r--) {
				cur.push_back(r);
			}
			if (cur[0] == 1) {
			}
		}
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::PigeonholeNim;

// vim:ft=cpp

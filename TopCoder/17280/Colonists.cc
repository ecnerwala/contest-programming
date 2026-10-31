#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

bool is_prime(int a) {
	if (a <= 1) return false;
	for (int i = 2; i * i <= a; i ++) {
		if (a % i == 0) return false;
	}
	return true;
}

class Colonists {

public:

	// win if xor of odd colonies is 0
	// normal moves, slow moves always touch exactly 1
	// fast moves always changes xor
	// slow move is always possible to make 0
	long long countMoves(vector <int> planet, vector <int> colonySize) {
		int N = int(planet.size());
		int cur_val = 0;
		for (int i = 0; i < N; i++) {
			if (planet[i] & 1) {
				cur_val ^= colonySize[i];
			}
		}

		std::map<int, std::vector<int>> colonies;
		for (int i = 0; i < N; i++) {
			colonies[planet[i]].push_back(colonySize[i]);
		}
		for (auto& it : colonies) {
			std::sort(it.second.begin(), it.second.end());
		}

		int64_t ans = 0;

		// normal moves always are from odd or to odd
		// casework on the odd guy
		for (int i = 0; i < N; i++) {
			if (planet[i] & 1) {
				int tgt = colonySize[i] ^ cur_val;
				if (tgt < colonySize[i]) {
					// normal move
					ans += int(colonies[planet[i]-1].size());

					// slow move
					ans ++;
				}
				if (tgt > colonySize[i]) {
					// normal move to
					int v = tgt - colonySize[i];
					const auto& vec = colonies[planet[i]+1];
					// number of things >= v
					ans += int(vec.end() - std::lower_bound(vec.begin(), vec.end(), v));
				}

				if (planet[i] >= 3 && tgt == colonySize[i] - planet[i] && is_prime(planet[i])) {
					// fast move
					ans++;
				}
			} else {
				if (cur_val > 0 && cur_val % 2 == 0 && colonySize[i] >= cur_val / 2) {
					// slow move
					ans++;
				}
			}
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::Colonists;

// vim:ft=cpp

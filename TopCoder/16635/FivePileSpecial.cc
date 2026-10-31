#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class FivePileSpecial {

public:

	vector<long long> findWinningMoves(vector<long long> piles_) {
		using ll = long long;
		assert(int(piles_.size()) == 5);
		const array<ll, 5> orig_piles{piles_[0], piles_[1], piles_[2], piles_[3], piles_[4]};

		vector<ll> res;
		auto solve = [&](array<bool, 5> take) {
			array<ll, 5> cur_piles = orig_piles;
			array<ll, 5> cur_take{0,0,0,0,0};
			int64_t x = 0;
			for (int b = 0; b < 62; b++) {
				for (int i = 0; i < 5; i++) {
					if ((cur_piles[i] >> b) & 1) {
						x ^= int64_t(1) << b;
					}
				}
				if (x & (int64_t(1) << b)) {
					for (int i = 0; i < 5; i++) {
						if (take[i]) {
							cur_piles[i] -= int64_t(1) << b;
							if (cur_piles[i] < 0) return;
							cur_take[i] += int64_t(1) << b;
						}
					}
				}
			}

			if (!x) return;

			res.insert(res.end(), cur_take.begin(), cur_take.end());
		};

		solve({1,0,0,0,0});
		solve({0,1,0,0,0});
		solve({0,0,1,0,0});
		solve({0,0,0,1,0});
		solve({0,0,0,0,1});
		
		solve({1,1,1,0,0});
		solve({0,1,1,1,0});
		solve({0,0,1,1,1});

		solve({1,1,1,1,1});

		return res;
	}

	// center empty => normal nim
	// all of these alter the xor, by considering lowest bit of x
	// so we just need to count number of such moves for which xor => 0
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::FivePileSpecial;

// vim:ft=cpp

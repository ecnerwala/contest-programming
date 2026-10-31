#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class FollowingNim {

public:

	int countWinningMoves(vector <int> piles, vector <int> special) {
		const int V = *max_element(piles.begin(), piles.end());

		// Number of winning moves
		vector<int> special_winning(V+1);
		for (int i = 0; i <= V; i++) {
			special_winning[i] = 0;
			for (int j : special) {
				if (i >= j && !special_winning[i-j]) special_winning[i]++;
			}
		}

		vector<int> nimber(V+1);
		vector<int> non_specials; non_specials.reserve(V+1);
		for (int i = 0; i <= V; i++) {
			nimber[i] = int(non_specials.size());
			if (!special_winning[i]) {
				non_specials.push_back(i);
				continue;
			}
		}

		int tot_nimber = 0;
		int cnt_special_winning = 0;
		int last_special_winning = -1;
		for (int i = 0; i < int(piles.size()); i++) {
			if (special_winning[piles[i]]) {
				cnt_special_winning++;
				last_special_winning = piles[i];
			} else {
				tot_nimber ^= nimber[piles[i]];
			}
		}

		// For any non-special-winning pile, any special move results in a loss
		// via special-winning, so you can only make non-special moves.
		// If there's another special-winning pile, then any (non-special) move
		// in a non-special-winning pile lets P2 win in that one.
		// Thus, if there's at least one special-winning pile, all winning
		// moves are in a special-winning pile.

		if (cnt_special_winning) {
			if (cnt_special_winning >= 2) {
				int ans = 0;
				for (int i = 0; i < int(piles.size()); i++) {
					// Number of special winning moves
					ans += special_winning[piles[i]];
				}
				return ans;
			} else {
				int P = last_special_winning;
				assert(P != -1);

				if (tot_nimber < nimber[P]) {
					int nimber_loss = non_specials[tot_nimber];
					assert(nimber_loss < P);
					int ans = 0;
					for (int j : special) {
						ans += 0 <= P-j && P-j < nimber_loss && !special_winning[P-j];
					}
					ans++; // exactly nimber_loss
					return ans;
				} else {
					return special_winning[P];
				}
			}
		} else {
			if (tot_nimber) {
				int target_nimber = 1 << (31 - __builtin_clz(tot_nimber));
				int ans = 0;
				for (int i = 0; i < int(piles.size()); i++) {
					int P = piles[i];
					if (nimber[P] & target_nimber) {
						ans++;
					}
				}
				return ans;
			} else {
				return 0;
			}
		}
	}
};

void my_tests() {
	// FILL IN TESTS
	cerr << FollowingNim().countWinningMoves({1, 2}, {1}) << '\n';
}

}  // namespace ecnerwala

using ecnerwala::FollowingNim;

// vim:ft=cpp

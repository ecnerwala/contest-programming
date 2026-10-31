#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int A, B, C, D, T; cin >> A >> B >> C >> D >> T;

	// if C == 1, the game ends when a plate has no moves, so make the game tree 1 shallower and flip D
	// We'll let (A, C) really describe the action space
	D ^= C;

	// B decides how we combine the A
	// B = 0 is normal sum games
	// B = 1 is "first player must win all of the individual subgames"
	// B = 2 is

	auto get_nimber = [&](int x) -> int {
		if (A == 0) {
			return x - C;
		} else if (A == 1) {
			return (x - C) & 3;
		} else if (A == 2) {
			assert(x >= 1);
			while (x > 3 && (x & 1)) x /= 2;
			if (x == 1) return C ? -1 : 0;
			else if (x == 2) return C ? 0 : 1;
			else if (x == 3) return C ? 1 : 0;
			else return x / 2;
		} else if (A == 3) {
			assert(x >= 1);
			if (C == 1 && x < 34) {
				return (x+2)%3;
			} else {
				x %= 34;
				int side = x / 17;
				x %= 17;
				if (!side) {
					return x % 3;
				} else {
					int v = x % 3;
					if (v) return 3-v;
					else return v;
				}
			}
		} else if (A == 4) {
			if (C == 0) {
				return (x - 1) & 1;
			} else if (C == 1) {
				if (x == 1) return -1;
				// TODO
				assert(false);
			} else assert(false);
		} else assert(false);
	};

	auto solve = [&](std::vector<int> X) -> bool {
		if (C == 1) {
			for (auto x : X) {
				if (A != 4 && get_nimber(x) == -1) return !D;
				if (A == 4 && x == 1) return !D;
			}
		}

		if (B == 2) {
			int v = *max_element(X.begin(), X.end());
			if(D == 0) {
				if(A != 4) return get_nimber(v) != 0;
			}
			if(D == 1){
				if (C == 1 && A == 0) return v != 0;
				if (C == 0 && A == 1) return v % 4 != 1;
			}
		}
		if (D == 0) {
			// B decides
			if (B == 0) {
				int tot = 0;
				for (auto x : X) {
					int cur_nimber = get_nimber(x);
					if (cur_nimber == -1) {
						// the plate starts with no possible operations, so we instantly win
						assert(C == 1);
						return true;
					}
					tot ^= cur_nimber;
				}
				return tot != 0;
			} else if (B == 1) {
				for (auto x : X) {
					if (A == 0 || A == 1 || A == 2 || A == 3) {
						int cur_nimber = get_nimber(x);
						if (cur_nimber == -1) {
							// the plate starts with no possible operations, so we instantly win
							assert(C == 1);
							return true;
						}
						if (cur_nimber != 0) return true;
					} else if (A == 4) {
						if (C == 0) {
							if ((x-1) & 1) return true;
						} else if (C == 1) {
							if (x % 5 == 4 || x % 5 == 0 || x % 5 == 1) return true;
						} else assert(false);
					} else assert(false);
				}
				return false;
			} else if (B == 2) {
				int v = *max_element(X.begin(), X.end());
				if(C == 0 && A != 4) return get_nimber(v) != 0;
				// TODO
				return false;
			} else assert(false);
		} else if (D == 1) {
			// TODO
			return false;
		} else assert(false);
	};
	for (int t = 0; t < T; t++) {
		int N; cin >> N;
		std::vector<int> X(N);
		for (auto& x : X) cin >> x;

		if (solve(X)) {
			cout << "Black" << '\n';
		} else {
			cout << "White" << '\n';
		}
	}

	return 0;
}

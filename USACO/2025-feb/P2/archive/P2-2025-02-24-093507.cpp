#include <bits/stdc++.h>

// [w + ib, x - ia] * [a, b] = c
// [y + jb, z - ja] * [a, b] = d
//
// 1 = (w+ib)(z-ja) - (x-ia)(y+jb)
// 1 = wz - xy + i*d - j*c
//
// [w + kcb, x - kca] * [a, b] = c
// [y + kdb, z - kda] * [a, b] = d

int signum(int64_t a) { return (a > 0) - (a < 0); }

std::pair<std::vector<std::pair<int64_t, bool>>, int64_t> cfrac(int64_t A, int64_t B) {
	assert(A > 0 && B > 0);
	std::vector<std::pair<int64_t, bool>> res;
	while (A != B) {
		if (A > B) {
			res.push_back({(A-1) / B, true});
			A = ((A-1) % B) + 1;
		} else {
			res.push_back({(B-1) / A, false});
			B = ((B-1) % A) + 1;
		}
	}
	return {std::move(res), A};
}

int64_t solve(int64_t A, int64_t B, int64_t C, int64_t D) {
	int64_t num_moves = 0;

	// take care of random cases, including gcd = 0
	if (A == C && B == D) return 0;

	if ((A == 0 && B == 0) || (C == 0 && D == 0)) return -1;

	// if we are making at least one move, move away from the 0 space
	if (A == 0) { num_moves++; A = B; }
	if (B == 0) { num_moves++; B = A; }
	if (C == 0) { num_moves++; C = -D; }
	if (D == 0) { num_moves++; D = -C; }

	int sAB = signum(A) * signum(B);
	int sCD = signum(C) * signum(D);

	assert(sAB != 0 && sCD != 0);

	// the signum product only increases
	if (sAB > sCD) {
		return -1;
	}

	assert(sAB <= sCD);
	if (sCD < 0) {
		std::swap(A, C);
		std::swap(B, D);
		std::swap(sAB, sCD);
		B = -B;
		D = -D;
		sAB = -sAB;
		sCD = -sCD;
		assert(sAB > 0);
	}


	if (sAB > 0) {
		assert(sCD >= sAB);
		assert(sCD > 0);

		// good iff CD passes AB on the way down
		if (A < 0) {
			A = -A, B = -B, C = -C, D = -D;
		}
		assert(A > 0 && B > 0);
		if (C < 0) return -1;
		assert(C > 0 && D > 0);

		auto [cfCD, gCD] = cfrac(C, D);
		auto [cfAB, gAB] = cfrac(A, B);
		if (gAB != gCD) return -1;
		// match up the suffix
		while (!cfAB.empty()) {
			if (cfCD.empty() || cfCD.back().second != cfAB.back().second || cfCD.back().first < cfAB.back().first) {
				return -1;
			}
			cfCD.back().first -= cfAB.back().first;
			if (cfCD.back().first == 0) cfCD.pop_back();
			cfAB.pop_back();
		}
		for (auto op : cfCD) {
			num_moves += op.first;
		}
		return num_moves;
	} else {
		assert(sAB < 0);
		assert(sCD > 0);
		if ((A > 0) != (C > 0)) {
			std::swap(A, B);
			std::swap(C, D);
		}
		assert((A > 0) == (C > 0));
		assert((B > 0) != (D > 0));
		if (A < 0) {
			A = -A, B = -B, C = -C, D = -D;
		}
		assert(A > 0 && B < 0 && C > 0 && D > 0);
		auto [cfCD, gCD] = cfrac(C, D);
		auto [cfAB, gAB] = cfrac(-B, A);
		if (gAB != gCD) return -1;
		for (auto op : cfCD) {
			num_moves += op.first;
		}
		for (auto op : cfAB) {
			num_moves += op.first;
		}
		num_moves += 2;

		int64_t num_savings = 3;
		while (!cfAB.empty() && !cfCD.empty() && cfAB.back().second == cfCD.back().second) {
			int64_t l = std::min(cfAB.back().first, cfCD.back().first);
			num_savings += 2 * l;
			cfAB.back().first -= l;
			if (cfAB.back().first == 0) cfAB.pop_back();
			cfCD.back().first -= l;
			if (cfCD.back().first == 0) cfCD.pop_back();
		}
		if (!cfAB.empty() && !cfCD.empty() && !cfAB.back().second && cfCD.back().second) {
			num_moves -= num_savings;
		}
		return num_moves;
	}

	assert(false);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t Q; cin >> Q;
	while (Q--) {
		int64_t A, B, C, D; cin >> A >> B >> C >> D;
		cout << solve(A, B, C, D) << '\n';
	}

	return 0;
}

// [10, -3] -> [10, 7]
// v
// [7, -3] and [3, 7]
// [1, -3] and [3, 1]
// [1, -2] and [2, 1]
// [1, -1] and [1, 1]
//
// [2, -1] and [2, 1]
// [1, -1] and [1, 1]

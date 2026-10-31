#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

std::set<int> seen;
std::vector<int> vals;
std::vector<int> best;
void dfs(int B, int P) {
	//cerr << "trying"; for (int v : vals) {cerr << ' ' << v;} cerr << '\n';
	if (int(vals.size()) == B) {
		if (vals.back() < best.back()) {
			best = vals;
			cerr << "found"; for (int v : best) {cerr << ' ' << v;} cerr << '\n';
		}
		return;
	}
	int L = int(vals.size());
	for (int v = (vals.empty() ? 0 : vals.back()); v < best.back(); v++) {
		bool good = true;
		std::set<int> prv_seen = seen;
		for (int msk = (1 << L); msk < (2 << L); msk++) {
			if (__builtin_popcount(msk) <= P) {
				int sum = v;
				for (int i = 0; i < L; i++) {
					if (msk & (1 << i)) sum += vals[i];
				}
				if (!seen.insert(sum).second) {
					good = false;
					break;
				}
			}
		}

		if (good) {
			vals.push_back(v);
			dfs(B, P);
			vals.pop_back();
		}
		seen = prv_seen;
	}
}

void dfs2(int B, int P) {
	//cerr << "trying"; for (int v : vals) {cerr << ' ' << v;} cerr << '\n';
	if (int(vals.size()) == B) {
		if (vals.front() < best.front()) {
			best = vals;
			cerr << "found"; for (int v : best) {cerr << ' ' << v;} cerr << '\n';
		}
		return;
	}
	int L = int(vals.size());
	for (int v = (L == 0 ? 0 : vals.back()); L == 0 ? v < best.front() : v >= 0; L == 0 ? v++ : v--) {
		if (L == 0) cerr << "checking first " << v << '\n';
		bool good = true;
		std::set<int> prv_seen = seen;
		for (int msk = (1 << L); msk < (2 << L); msk++) {
			if (__builtin_popcount(msk) <= P) {
				int sum = v;
				for (int i = 0; i < L; i++) {
					if (msk & (1 << i)) sum += vals[i];
				}
				if (!seen.insert(sum).second) {
					good = false;
					break;
				}
			}
		}

		if (good) {
			vals.push_back(v);
			dfs2(B, P);
			vals.pop_back();
		}
		seen = prv_seen;
	}
}

std::vector<int> brute_force(int B, int P) {
	cerr << "Brute forcing " << B << ' ' << P << '\n';
	seen.clear();
	vals.clear();
	best = std::vector<int>(B);
	for (int i = 0; i < B; i++) best[i] = (1 << i);
	cerr << "found"; for (int v : best) {cerr << ' ' << v;} cerr << '\n';
	//std::reverse(best.begin(), best.end());

	seen.insert(0);
	dfs(B, P);
	cerr << "best " << B << ' ' << P << ":"; for (int v : best) {cerr << ' ' << v;} cerr << '\n';
	return best;
}

void check(int B, int P, std::vector<int> res) {
	cerr << "checking " << B << ' ' << P << '\n';
	assert(int(res.size()) == B);
	std::vector<int> vals; vals.reserve(1 << B);
	for (int msk = 0; msk < (1 << B); msk++) {
		if (__builtin_popcount(msk) <= P) {
			int sum = 0;
			for (int i = 0; i < B; i++) {
				if (msk & (1 << i)) sum += res[i];
			}
			vals.push_back(sum);
		}
	}
	std::sort(vals.begin(), vals.end());
	assert(std::unique(vals.begin(), vals.end()) == vals.end());
}

class PoisonedApples {

public:

	std::vector<int> buy_optimal(int B, int P) {
		assert(0 <= P && P <= B && B <= 8);
		if (P == 0) {
			return std::vector<int>(B, 0);
		}
		if (P == 1) {
			std::vector<int> res(B);
			std::iota(res.begin(), res.end(), 1);
			return res;
		}
		assert(2 <= P && P <= B && B <= 8);
		std::array<std::array<std::vector<int>, 9>, 9> solved;
		solved[1][0] = std::vector<int>({0});
		solved[1][1] = std::vector<int>({1});
		solved[2][0] = std::vector<int>({0, 0});
		solved[2][1] = std::vector<int>({1, 2});
		solved[2][2] = std::vector<int>({1, 2});
		solved[3][0] = std::vector<int>({0, 0, 0});
		solved[3][1] = std::vector<int>({1, 2, 3});
		solved[3][2] = std::vector<int>({1, 2, 4});
		solved[3][3] = std::vector<int>({1, 2, 4});
		solved[4][0] = std::vector<int>({0, 0, 0, 0});
		solved[4][1] = std::vector<int>({1, 2, 3, 4});
		solved[4][2] = std::vector<int>({1, 2, 4, 7});
		solved[4][3] = std::vector<int>({3, 5, 6, 7});
		solved[4][4] = std::vector<int>({3, 5, 6, 7});
		solved[5][0] = std::vector<int>({0, 0, 0, 0, 0});
		solved[5][1] = std::vector<int>({1, 2, 3, 4, 5});
		solved[5][2] = std::vector<int>({1, 2, 4, 7, 12});
		solved[5][3] = std::vector<int>({3, 6, 11, 12, 13});
		solved[5][4] = std::vector<int>({3, 6, 11, 12, 13});
		solved[5][5] = std::vector<int>({3, 6, 11, 12, 13});
		solved[6][0] = std::vector<int>({0, 0, 0, 0, 0, 0});
		solved[6][1] = std::vector<int>({1, 2, 3, 4, 5, 6});
		solved[6][2] = std::vector<int>({1, 2, 4, 8, 13, 18});
		solved[6][3] = std::vector<int>({3, 6, 12, 22, 23, 24});
		solved[6][4] = std::vector<int>({11, 17, 20, 22, 23, 24});
		solved[6][5] = std::vector<int>({11, 17, 20, 22, 23, 24});
		solved[6][6] = std::vector<int>({11, 17, 20, 22, 23, 24});
		solved[7][0] = std::vector<int>({0, 0, 0, 0, 0, 0, 0});
		solved[7][1] = std::vector<int>({1, 2, 3, 4, 5, 6, 7});
		solved[7][2] = std::vector<int>({1, 2, 4, 8, 14, 19, 24});
		solved[7][3] = std::vector<int>({3, 6, 12, 22, 41, 42, 43});
		solved[7][4] = std::vector<int>({20, 31, 37, 40, 42, 43, 44});
		solved[7][5] = std::vector<int>({20, 31, 37, 40, 42, 43, 44});
		solved[7][6] = std::vector<int>({20, 31, 37, 40, 42, 43, 44});
		solved[7][7] = std::vector<int>({20, 31, 37, 40, 42, 43, 44});
		solved[8][0] = std::vector<int>({0, 0, 0, 0, 0, 0, 0, 0});
		solved[8][1] = std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8});
		solved[8][2] = std::vector<int>({1, 2, 4, 8, 15, 24, 29, 34});
		solved[8][3] = std::vector<int>({1, 2, 30, 40, 52, 59, 74, 78}); // ???
		for (int b = 1; b <= 8; b++) {
			for (int p = 0; p <= b; p++) {
				if (p <= 3)
				check(b, p, solved[b][p]);
			}
		}
		return solved[B][P];
		//return brute_force(B, P);
	}

	vector <int> buy(int B, int A, int P) {
		std::vector<int> res = buy_optimal(B, P);
		if (*std::max_element(res.begin(), res.end()) <= A) return res;
		else return {};
	}
};

void my_tests() {
	// FILL IN TESTS
	for (int B = 8; B <= 8; B++) {
		for (int P = 4; P <= B; P++) {
			brute_force(B, P);
		}
	}
}

}  // namespace ecnerwala

using ecnerwala::PoisonedApples;

// vim:ft=cpp

#include <bits/stdc++.h>

using std::cerr;

std::map<std::pair<int, int>, bool> mem;
bool solve_rec(int N, int s) {
	{
		auto it = mem.find({N, s});
		if (it != mem.end()) return it->second;
	}
	bool res = false;
	for (int msk = 1; msk < (1 << N); msk++) {
		if (__builtin_popcount(msk) & 1) continue;
		int filt = s & msk;
		if (__builtin_popcount(filt) * 2 != __builtin_popcount(msk)) continue;
		// filt should be the low half of msk
		int hi = filt ^ msk;
		if (filt >= (1 << __builtin_ctz(hi))) continue;
		cerr << "move " << N << ' ' << s << ' ' << msk << '\n';
		if (solve_rec(N, s ^ msk)) {
			res = true;
			break;
		}
	}
	mem.insert({{N, s}, res});
	return res;
}

void slow(int N) {
	for (int msk = 0; msk < (1 << N); msk++) {
		bool res = solve_rec(N, msk);
		for (int i = 0; i < N; i++) { cerr << ((msk >> i) & 1); }
		cerr << ' ' << (res ? "win" : "lose");
		cerr << '\n';
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	for (int N = 1; N <= 6; N++) {
		slow(N);
	}

	return 0;
}

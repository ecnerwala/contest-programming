#include <bits/stdc++.h>

using std::cerr;

std::map<std::pair<int, int>, bool> mem;
bool solve_rec(int N, int s) {
	if (N == 0) return true;
	if ((s & 1) == 0) return solve_rec(N-1, s >> 1);
	if ((s >> (N-1)) & 1) return solve_rec(N-1, s & ((1 << (N-1)) - 1));
	{
		auto it = mem.find({N, s});
		if (it != mem.end()) return it->second;
	}
	bool has_move = false;
	bool res = false;
	for (int msk = 1; msk < (1 << N); msk++) {
		if (__builtin_popcount(msk) & 1) continue;
		int filt = s & msk;
		if (__builtin_popcount(filt) * 2 != __builtin_popcount(msk)) continue;
		// filt should be the low half of msk
		int hi = filt ^ msk;
		if (filt >= (1 << __builtin_ctz(hi))) continue;
		has_move = true;
		if (!solve_rec(N, s ^ msk)) {
			cerr << "move ";
			for (int i = 0; i < N; i++) cerr << ((s >> i) & 1);
			cerr << " -> ";
			for (int i = 0; i < N; i++) cerr << (((s^msk) >> i) & 1);
			cerr << '\n';
			res = true;
			break;
		}
	}
	if (!has_move) res = true;
	mem.insert({{N, s}, res});
	return res;
}

void slow(int N) {
	for (int msk = 0; msk < (1 << N); msk++) {
		if (((msk >> 0) & 1) == 0) continue;
		if (((msk >> (N-1)) & 1) == 1) continue;
		bool res = solve_rec(N, msk);
		//for (int i = 0; i < N; i++) { cerr << ((msk >> i) & 1); } cerr << ' ' << (res ? "win" : "lose") << '\n';
	}
}

// subsequences of the good thing are
// 0000001000011110111111
// +++++++____++++_++++++
// ++++++_++++++++_++++++
// ++++++_++++____+++++++

std::optional<std::vector<int>> get_move(const std::string& S) {
	int N = int(S.size());
	int split = 0;
	for (char c : S) {
		if (c == '0') split++;
	}
	int num_bad = 0;
	for (int i = 0; i < N; i++) {
		if (S[i] != "01"[i >= split]) num_bad++;
	}
	// split+i, split-1-i
	std::vector<int> res; res.reserve(N);
	for (int l = split-1, r = split; l >= 0 && r < N; l--, r++) {
		char vl = S[l], vr = S[r];
		if (vl == '1' && vr == '0' && num_bad == 2) return std::nullopt;
		if (vl == '1' || vr == '0') {
			// this is good
			for (int z = 0; z < N; z++) {
				if (l < z && z < r) continue;
				if (z == l && vl == '1') continue;
				if (z == r && vr == '0') continue;
				res.push_back(z);
			}
			return res;
		}
	}
	assert(false);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	//for (int N = 1; N <= 8; N++) { slow(N); }

	// only losing position is the symmetric ones after pruning

	int T; cin >> T;
	while (T--) {
		std::string S; cin >> S; if (S == "-1") exit(0);
		if (get_move(S)) {
			cout << "First" << '\n' << flush;
		} else {
			cout << "Second" << '\n' << flush;
			cin >> S; if (S == "-1") exit(0);
		}
		while (!std::is_sorted(S.begin(), S.end())) {
			auto r = get_move(S);
			assert(r);
			auto& v = *r;
			cout << v.size();
			for (int a : v) { cout << ' ' << a+1; }
			cout << '\n' << flush;
			cin >> S; if (S == "-1") exit(0);
		}
	}

	return 0;
}

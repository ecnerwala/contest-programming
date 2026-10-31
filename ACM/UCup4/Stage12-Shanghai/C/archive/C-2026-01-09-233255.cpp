#include <bits/stdc++.h>

std::optional<std::vector<std::array<int, 2>>> solve(std::vector<int> P) {
	int N = int(P.size());
	assert(!(N & 1));

	std::vector<int> buf(N);

	std::vector<std::array<int, 2>> ops;
	auto do_op = [&](int l, int r) -> bool {
		assert(!((r - l) & 1));
		assert((r - l) >= 4);
		std::copy(P.begin() + l, P.begin() + r, buf.begin());
		std::sort(buf.begin(), buf.begin() + (r-l));
		int a = 0, b = (r-l)/2;
		int mid = buf[b];
		bool changed = false;
		for (int i = l; i < r; i++) {
			int nxt = buf[(P[i] < mid ? a : b)++];
			if (P[i] != nxt) changed = true;
			P[i] = nxt;
		}
		assert(a == (r-l)/2);
		assert(b == r-l);
		if (changed) ops.push_back({l, r});
		return changed;
	};

	assert(N >= 4);
	while (true) {
		//for (auto v : P) { std::cerr << v << ' '; } std::cerr << '\n';
		do_op(0, N);
		if (std::is_sorted(P.begin(), P.end())) {
			return ops;
		}
		if (N == 4) return std::nullopt;

		assert(N >= 6);
		if (N <= 6) {
			bool did = false;
			for (int i = 0; i <= N; i++) {
				for (int j = i+4; j <= N; j += 2) {
					if (do_op(i, j)) did = true;
				}
			}
			if (!did) return std::nullopt;
			continue;
		}

		std::vector<int> prefs(N+1);
		for (int i = 0; i < N; i++) prefs[i+1] = prefs[i] + (P[i] < N/2 ? +1 : -1);
		//for (auto p : prefs) { std::cerr << p << ' '; } std::cerr << '\n';

		std::pair<int, int> max_pref{0, 0};
		for (int i = 0; i <= N; i ++) {
			int pref = prefs[i];
			max_pref = std::max(max_pref, {pref, i});
		}
		if (max_pref.first >= 4 || max_pref.first == 2) {
			// do the operation
			int z = max_pref.first & 1;
			int i = max_pref.second;

			if (i >= N-i) do_op(0, i + z);
			else do_op(i - z, N);
		} else if (max_pref.first == 3) {
			assert(N >= 8); // if N == 6, we're already sorted
			int i = max_pref.second;
			if (prefs[1] != 1) {
				do_op(0, i + 1);
				assert(P[0] < N/2);
				continue;
			}
			if (prefs[N-1] != 1) {
				do_op(i - 1, N);
				assert(P[N-1] >= N/2);
				continue;
			}
			assert(prefs[1] == 1 && prefs[N-1] == 1);
			if (i >= N-i) do_op(1, i);
			else do_op(i, N-1);
		} else if (max_pref.first == 1) {
			// we're stuck
			if (*std::min_element(prefs.begin(), prefs.end()) == 0) return std::nullopt;
			// look for some far-ish 1
			// there's a 1 and -1 somewhere

			if (N == 6) {
				// fixed N==6 already
				assert(false);
			}

			assert(N >= 8);
			assert(prefs[1] == 1 || prefs[1] == -1);
			assert(prefs[N-1] == 1 || prefs[N-1] == -1);
			if (prefs[1] != prefs[N-1]) {
				do_op(1, N-1);
			}
			// we know we have both 1 and -1, so find any
			int i = 1;
			while (prefs[i] != -prefs[1]) i += 2;
			if (i >= N-i) do_op(1, i);
			else do_op(i, N-1);
		} else {
			assert(max_pref.first == 0);
			// all prefixes <= 1
			// if we have a 1 and a -1 far enough apart, we can get a 2
			// if we have only 0 and 1, then we're screwed
			if (*std::min_element(prefs.begin(), prefs.end()) == -1) return std::nullopt;
			int i = 0;
			while (i < N && prefs[i] != -2) i++;
			// we're all 0 and -1
			assert(i != N);

			assert(prefs[i] == -2);
			// give ourselves a +1
			if (i >= N-i) do_op(0, i);
			else do_op(i, N);
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	if (true) {
		std::mt19937 mt(48);
		for (int z = 0; z < 100000; z++) {
			int N = std::uniform_int_distribution<int>(2, 20)(mt) * 2;
			std::vector<int> P(N); std::iota(P.begin(), P.end(), 0);
			std::shuffle(P.begin(), P.end(), mt);
			cerr << "try " << N << '\n';
			//for (auto p : P) { cerr << p << ' '; } cerr << '\n';
			auto r = solve(P);
			cerr << (r ? int(r->size()) : -1) << '\n';
		}
		cerr << "tests done" << '\n';
	}

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> P(N);
		for (auto& p : P) { cin >> p; p--; }

		auto res = solve(P);
		if (!res) {
			cout << -1 << '\n';
		} else {
			cout << res->size() << '\n';
			for (auto [l, r] : *res) {
				cout << l+1 << ' ' << r << '\n';
			}
		}
	}

	return 0;
}
